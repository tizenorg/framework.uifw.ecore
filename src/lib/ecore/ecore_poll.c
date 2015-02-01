#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "Ecore.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __linux
#include <math.h>
#include <sys/timerfd.h>
#endif

#include "ecore_private.h"

/* Common declarations */

/* General fields for different backend type,
 some type of base class but made by agregation,
 TODO move some specific fields inside Backend */
typedef struct
{
   int min_interval;
   int interval_incr;
   int at_tick;
   int just_added_poller;
   int poller_delete_count;
   int poller_walking;
   double poll_interval;
   double poll_cur_interval;
   struct timespec slast_tick;
   double last_tick;
   Ecore_Poller *pollers[16];
   unsigned short poller_counters[16];
}  _Backend_Data;

typedef struct
{
   void (* set_poll_interval) ( _Backend_Data *data,
      double poll_time);

   double (* get_poll_interval) (_Backend_Data *data);

   Ecore_Poller * (* add) (_Backend_Data *data,
      int interval, Ecore_Task_Cb func,
      const void *user_data);

   Eina_Bool (* set_poller_interval) (_Backend_Data *data,
      Ecore_Poller *poller, int interval);

   int (* get_poller_interval) (_Backend_Data *data, Ecore_Poller *poller);

   void * (*delete) (_Backend_Data *data, Ecore_Poller *poller);

   void (* shutdown) (_Backend_Data *data);

   _Backend_Data data;
} Ecore_Poller_Backend;

static Ecore_Poller_Backend *backends[ECORE_POLLER_TYPE_MAX];

struct _Ecore_Poller
{
   EINA_INLIST;
   ECORE_MAGIC;
   int           ibit;
   unsigned char delete_me : 1;
   Ecore_Task_Cb func;
   void         *data;
   Ecore_Poller_Type type;
};

GENERIC_ALLOC_SIZE_DECLARE(Ecore_Poller);

/* TIMERFD part */
#ifdef __linux
static Ecore_Fd_Handler *fdhandler;
/* TODO we could get it from fdhandler */
static int timerfd;

/*TODO To common place */
#ifndef SEC_TO_NSEC
   #define SEC_TO_NSEC(x)         ((x)*1000000000)
#endif

#ifndef NSEC_TO_SEC
   #define NSEC_TO_SEC(x)         ((x)/1000000000)
#endif


#define TFD_ATTEMPTS 1
#define TFD_TIMER_DEFERRABLE (2 << 1)

static int flags = TFD_TIMER_ABSTIME | TFD_TIMER_DEFERRABLE;

#endif /* __linux */
/* TIMERFD part */

/* common part */
static void
_common_poller_next_tick_eval(_Backend_Data *bdata,
   void (* timer_finalize) (void),
   void (* timer_charge) (double interval, _Backend_Data *bdata),
   void (* timer_recharge) (double interval, _Backend_Data *bdata),
   bool (* timer_charged) (void))
{
   int i;
   double interval;
   const bool was_charged = timer_charged();

   bdata->min_interval = -1;
   for (i = 0; i < 15; i++)
     {
        if (bdata->pollers[i])
          {
             bdata->min_interval = i;
             break;
          }
     }
   if (bdata->min_interval < 0)
     {
        /* no pollers */
         timer_finalize();
         return;
     }
   bdata->interval_incr = (1 << bdata->min_interval);
   interval = bdata->interval_incr * bdata->poll_interval;

   timer_charge(interval, bdata);
   /* we are at the tick callback - so no need to do inter-tick adjustments
    * so we can fasttrack this as t -= last_tick in theory is 0.0 (though
    * in practice it will be a very very very small value. also the tick
    * callback will adjust the timer interval at the end anyway
    * if timer was charged at this iteration was_charged should be fasle
    * */
   if (!bdata->at_tick && interval != bdata->poll_cur_interval && was_charged)
      {
         double t = ecore_time_get();
         t -= bdata->last_tick; /* time since we last ticked */
     /* delete the timer and reset it to tick off in the new
      * time interval. at the tick this will be adjusted */
         timer_recharge(interval - t, bdata);
      }
   bdata->poll_cur_interval = interval;
}

static Eina_Bool
_common_poller_cb(void *data,
	void ( * _next_tick_eval) (_Backend_Data *bdata),
        Eina_Bool (* _timer_interval_adjust) (double interval))
{
   int i;
   Ecore_Poller *poller, *l;
   _Backend_Data *bdata = (_Backend_Data *)data;
   int changes = 0;

   bdata->at_tick++;
   bdata->last_tick = ecore_time_get();
   /* we have 16 counters - each increments every time the poller counter
    * "ticks". it increments by the minimum interval (which can be 1, 2, 4,
    * 7, 16 etc. up to 32768) */
   for (i = 0; i < 15; i++)
     {
        bdata->poller_counters[i] += bdata->interval_incr;
        /* wrap back to 0 if we exceed out loop count for the counter */
        if (bdata->poller_counters[i] >=
          (1 << i)) bdata->poller_counters[i] = 0;
     }

   bdata->just_added_poller = 0;
   /* walk the pollers now */
   bdata->poller_walking++;
   for (i = 0; i < 15; i++)
     {
        /* if the counter is @ 0 - this means that counter "went off" this
         * tick interval, so run all pollers hooked to that counter */
          if (bdata->poller_counters[i] == 0)
            {
               EINA_INLIST_FOREACH(bdata->pollers[i], poller)
                 {
                    if (poller->delete_me)
                       continue;

                    if (!poller->func(poller->data))
                       {
                          if (!poller->delete_me)
                             {
                                poller->delete_me = 1;
                                bdata->poller_delete_count++;
                             }
                       }
                 }
            }
     }
   bdata->poller_walking--;

   /* handle deletes afterwards */
   if (bdata->poller_delete_count > 0)
     {
        /* FIXME: walk all pollers and remove deleted ones */
         for (i = 0; i < 15; i++)
           {
              for (l = bdata->pollers[i]; l; )
                {
                   poller = l;
                   l = (Ecore_Poller *)EINA_INLIST_GET(l)->next;
                   if (poller->delete_me)
                     {
                        bdata->pollers[i] = (Ecore_Poller *)eina_inlist_remove(EINA_INLIST_GET(bdata->pollers[i]), EINA_INLIST_GET(poller));
                        ecore_poller_mp_free(poller);
                        bdata->poller_delete_count--;
                        changes++;
                        if (bdata->poller_delete_count <= 0) break;
                     }
                }
              if (bdata->poller_delete_count <= 0) break;
           }
     }
   /* if we deleted or added any pollers, then we need to re-evaluate our
    * minimum poll interval */
   if ((changes > 0) || (bdata->just_added_poller > 0))
     _next_tick_eval(bdata);

   bdata->just_added_poller = 0;
   bdata->poller_delete_count = 0;

   bdata->at_tick--;

   return _timer_interval_adjust(bdata->poll_cur_interval);
}

static Eina_Bool
_common_poller_poller_interval_set(_Backend_Data *bdata, Ecore_Poller *poller, int interval, void ( * _next_tick_eval) (_Backend_Data *bdata))
{
   int ibit;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(EINA_FALSE);
   if (!ECORE_MAGIC_CHECK(poller, ECORE_MAGIC_POLLER))
     {
        ECORE_MAGIC_FAIL(poller, ECORE_MAGIC_POLLER,
                         "ecore_poller_poller_interval_set");
        return EINA_FALSE;
     }

   /* interval MUST be a power of 2, so enforce it */
   if (interval < 1) interval = 1;
   ibit = -1;
   while (interval != 0)
     {
        ibit++;
        interval >>= 1;
     }
   /* only allow up to 32768 - i.e. ibit == 15, so limit it */
   if (ibit > 15) ibit = 15;
   /* if interval specified is the same as interval set, return true without wasting time */
   if (poller->ibit == ibit)
     return EINA_TRUE;
   bdata->pollers[poller->ibit] = (Ecore_Poller *)eina_inlist_remove(EINA_INLIST_GET(bdata->pollers[poller->ibit]), EINA_INLIST_GET(poller));
   poller->ibit = ibit;
   bdata->pollers[poller->ibit] = (Ecore_Poller *)eina_inlist_prepend(EINA_INLIST_GET(bdata->pollers[poller->ibit]), EINA_INLIST_GET(poller));
   if (bdata->poller_walking)
     bdata->just_added_poller++;
   else
     _next_tick_eval(bdata);
   return EINA_TRUE;
}

static Ecore_Poller *
_common_poller_add(_Backend_Data *bdata,
                 int               interval,
                 Ecore_Task_Cb     func,
                 const void       *data,
                 void ( * _next_tick_eval) (_Backend_Data *bdata)
)
{
   Ecore_Poller *poller;
   int ibit;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   if (!func) return NULL;
   if (interval < 1) interval = 1;

   poller = ecore_poller_calloc(1);
   if (!poller) return NULL;
   ECORE_MAGIC_SET(poller, ECORE_MAGIC_POLLER);

   /* interval MUST be a power of 2, so enforce it */
   if (interval < 1) interval = 1;
   ibit = -1;
   while (interval != 0)
     {
        ibit++;
        interval >>= 1;
     }
   /* only allow up to 32768 - i.e. ibit == 15, so limit it */
   if (ibit > 15) ibit = 15;

   poller->ibit = ibit;
   poller->func = func;
   poller->data = (void *)data;
   bdata->pollers[poller->ibit] = (Ecore_Poller *)eina_inlist_prepend(EINA_INLIST_GET(bdata->pollers[poller->ibit]), EINA_INLIST_GET(poller));
   if (bdata->poller_walking)
     bdata->just_added_poller++;
   else
     _next_tick_eval(bdata);
   return poller;
}

static void
_common_poller_core_shutdown(_Backend_Data *bdata,
   void (* timer_finalize) (void))
{
   int i;
   Ecore_Poller *poller;

   for (i = 0; i < 15; i++)
     {
        while ((poller = bdata->pollers[i]))
          {
             bdata->pollers[i] = (Ecore_Poller *)eina_inlist_remove (
               EINA_INLIST_GET(bdata->pollers[i]),
               EINA_INLIST_GET(bdata->pollers[i]));
             ecore_poller_mp_free(poller);
          }
     }

   if (timer_finalize)
      timer_finalize();
}

static double
_common_poller_poll_interval_get(_Backend_Data *bdata)
{
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0.0);
   return bdata->poll_interval;
}

static int
_common_poller_poller_interval_get(_Backend_Data *bdata __UNUSED__, Ecore_Poller *poller)
{
   int ibit, interval = 1;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
   if (!ECORE_MAGIC_CHECK(poller, ECORE_MAGIC_POLLER))
     {
        ECORE_MAGIC_FAIL(poller, ECORE_MAGIC_POLLER,
                         "ecore_poller_poller_interval_get");
        return 0;
     }

   ibit = poller->ibit;
   while (ibit != 0)
     {
        ibit--;
        interval <<= 1;
     }
   return interval;
}

static void *
_common_poller_del(_Backend_Data *bdata, Ecore_Poller *poller,
   void ( * _next_tick_eval) (_Backend_Data *bdata))
{
   void *data;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   if (!ECORE_MAGIC_CHECK(poller, ECORE_MAGIC_POLLER))
     {
        ECORE_MAGIC_FAIL(poller, ECORE_MAGIC_POLLER,
                         "ecore_poller_del");
        return NULL;
     }

   /* we are walking the poller list - a bad idea to remove from it while
    * walking it, so just flag it as delete_me and come back to it after
    * the loop has finished */
   if (bdata->poller_walking > 0)
     {
        bdata->poller_delete_count++;
        poller->delete_me = 1;
        return poller->data;
     }
   /* not in loop so safe - delete immediately */
   data = poller->data;
   bdata->pollers[poller->ibit] = (Ecore_Poller *)eina_inlist_remove(EINA_INLIST_GET(bdata->pollers[poller->ibit]), EINA_INLIST_GET(poller));
   ecore_poller_mp_free(poller);
   _next_tick_eval(bdata);
   return data;
}

/* common part */

/* CORE part */

static Ecore_Timer *timer;
static Eina_Bool _ecore_poller_cb_timer(void *data);

static void
_ecore_timer_finalize(void)
{
   if (!timer) return;

   ecore_timer_del(timer);
   timer = NULL;
}

static void
_ecore_timer_charge(double interval, _Backend_Data *bdata)
{
   if (!timer)
      timer = ecore_timer_add(interval, _ecore_poller_cb_timer, bdata);
}

static void
_ecore_timer_recharge(double interval, _Backend_Data *bdata)
{
   if (!timer) return;

   ecore_timer_del(timer);
   timer = ecore_timer_add(interval,
                              _ecore_poller_cb_timer, bdata);
}

static Eina_Bool
_ecore_interval_adjust(double interval)
{
   /* if the timer was deleted then there is no point returning 1 - ambiguous
    * if we do as it implies keep running me" but we have been deleted
    * anyway */
   if (!timer) return ECORE_CALLBACK_CANCEL;

   /* adjust interval */
   ecore_timer_interval_set(timer, interval);
   return ECORE_CALLBACK_RENEW;
}

static bool
_ecore_timer_charged(void)
{
   return timer;
}

static void
_ecore_poller_next_tick_eval(_Backend_Data *bdata)
{
   _common_poller_next_tick_eval(bdata, _ecore_timer_finalize, _ecore_timer_charge,
	_ecore_timer_recharge, _ecore_timer_charged);
}

static Eina_Bool
_ecore_poller_cb_timer(void *data)
{
   return _common_poller_cb(data, _ecore_poller_next_tick_eval, _ecore_interval_adjust);
}

static void
_ecore_poller_poll_interval_set(_Backend_Data *bdata, double poll_time)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   bdata->poll_interval = poll_time;
   _ecore_poller_next_tick_eval(bdata);
}

static Eina_Bool
_ecore_poller_poller_interval_set(_Backend_Data *bdata, Ecore_Poller *poller, int interval)
{
   return _common_poller_poller_interval_set(bdata, poller, interval,
      _ecore_poller_next_tick_eval);
}

static void *
_ecore_poller_del(_Backend_Data *bdata, Ecore_Poller *poller)
{
   return _common_poller_del(bdata, poller, _ecore_poller_next_tick_eval);
}

static void
_ecore_poller_core_shutdown(_Backend_Data *bdata)
{
   _common_poller_core_shutdown(bdata, NULL);
}

static Ecore_Poller *
_ecore_poller_add(_Backend_Data *bdata,
                 int               interval,
                 Ecore_Task_Cb     func,
                 const void       *data)
{
   Ecore_Poller *poller = _common_poller_add(bdata, interval, func, data,
      _ecore_poller_next_tick_eval);
   EINA_SAFETY_ON_NULL_RETURN_VAL(poller, NULL);
   poller->type = ECORE_POLLER_CORE;
   return poller;
}

/* end CORE part */

/* TIMERFD part */

#ifdef __linux

static Eina_Bool _lazy_poller_cb_handler(void *data,
   Ecore_Fd_Handler __UNUSED__ * fd_handler);

static void
_timespec_ns_add(struct timespec *spec, double nsecs)
{
   double new_nsec = NSEC_TO_SEC(spec->tv_nsec + nsecs);
   double new_sec;
   new_nsec = modf(new_nsec, &new_sec);
   spec->tv_sec += new_sec;
   spec->tv_nsec += new_nsec;
}

/**
 * @desc start/repetion timer interval
 * @param new_value output itimerspec
 * @param interval - time interval in microseconds
 */
static int
_itimespec_prepare(struct itimerspec *new_value, const double interval)
{
   struct timespec now;
   int ret;
   double secs;
   double usecs;

   if (!new_value)
     {
        ERR("Please provide valid pointer!");
        return -EINVAL;
     }

   usecs = modf(interval, &secs);

   ret = clock_gettime(CLOCK_MONOTONIC, &now);
   if (ret < 0) return -errno;

   new_value->it_value.tv_nsec = now.tv_nsec;
   new_value->it_value.tv_sec = now.tv_sec;

   usecs = SEC_TO_NSEC(usecs);
   _timespec_ns_add(&new_value->it_value, usecs);
   new_value->it_value.tv_sec +=(int)secs;

   new_value->it_interval.tv_sec = (int)secs;
   new_value->it_interval.tv_nsec = usecs;
   return 0;
}

static void
_fdhandler_interval_set(double interval, _Backend_Data __UNUSED__ *bdata)
{
   int ret;
   int flags = TFD_TIMER_ABSTIME;
   struct itimerspec new_value;

   _itimespec_prepare(&new_value, interval);

   /* use flags which was defined at add time,
    * due it couldn't be changed w/o reboot */
   ret = timerfd_settime(timerfd, flags, &new_value, NULL);

   if (ret < 0)
     {
       ERR("Couldn't set timer timerfd time %s", strerror(errno));
     }
}

static void
_fdhandler_add(double interval,
   _Backend_Data  *bdata)
{
   struct itimerspec new_value;
   int ret, i;

   if (timerfd && fdhandler) return;

   timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
      /* calculate new_value */
   ret = _itimespec_prepare(&new_value, interval);
   if (ret < 0)
     {
        ERR("Couldn't calculate new timer value %s", strerror(ret));
        goto release_fd;
     }

   for (i = 0; i < TFD_ATTEMPTS; ++i)
     {
        ret = timerfd_settime(timerfd, flags, &new_value, NULL);
        if (ret < 0)
            flags &= ~TFD_TIMER_DEFERRABLE;
        else
          break;
     }

   if (ret < 0)
     {
        ERR("Couldn't set timerfd time %s", strerror(errno));
        goto release_fd;
     }

   fdhandler = ecore_main_fd_handler_add(timerfd,
                                           ECORE_FD_READ | ECORE_FD_ERROR,
                                           _lazy_poller_cb_handler,
                                           bdata, NULL, NULL);
   if (fdhandler == 0)
     {
        ERR("Couldn't add ecore fd handler");
        goto release_fd;
     }

   return;

release_fd:
   close(timerfd);
}

static void
_fdhandler_finalize(void)
{
   /* no pollers */
   if (!fdhandler) return;

   ecore_main_fd_handler_del(fdhandler);
   close(timerfd);
   fdhandler = NULL;
}

static bool
_fdhandler_charged(void)
{
   return fdhandler && timerfd;
}

static void
_lazy_poller_next_tick_eval(_Backend_Data *bdata)
{
    _common_poller_next_tick_eval(bdata, _fdhandler_finalize, _fdhandler_add,
       _fdhandler_interval_set, _fdhandler_charged);
}

static Eina_Bool
_lazy_interval_adjust(double interval)
{
   /* if the timer was deleted then there is no point returning 1 - ambiguous
    * if we do as it implies keep running me" but we have been deleted
    * anyway */
   if (!fdhandler) return ECORE_CALLBACK_CANCEL;

   /* adjust interval */
   _fdhandler_interval_set(interval, NULL);

   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_lazy_poller_cb_handler(void *data, Ecore_Fd_Handler __UNUSED__ * fd_handler)
{
   return _common_poller_cb(data, _lazy_poller_next_tick_eval, _lazy_interval_adjust);
}

static void
_lazy_poller_poll_interval_set(_Backend_Data *bdata, double poll_time)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   bdata->poll_interval = poll_time;
   _lazy_poller_next_tick_eval(bdata);
}

static Ecore_Poller *
_lazy_poller_add(_Backend_Data *bdata,
                 int               interval,
                 Ecore_Task_Cb     func,
                 const void       *data)
{
   Ecore_Poller *poller = _common_poller_add(bdata, interval, func, data,
     _lazy_poller_next_tick_eval);
   EINA_SAFETY_ON_NULL_RETURN_VAL(poller, NULL);
   poller->type = ECORE_POLLER_LAZY;
   return poller;
}

static Eina_Bool
_lazy_poller_poller_interval_set(_Backend_Data *bdata, Ecore_Poller *poller, int interval)
{
   return _common_poller_poller_interval_set(bdata, poller, interval,
      _lazy_poller_next_tick_eval);
}

static void *
_lazy_poller_del(_Backend_Data *bdata, Ecore_Poller *poller)
{
   return _common_poller_del(bdata, poller, _lazy_poller_next_tick_eval);
}

static void
_lazy_poller_core_shutdown(_Backend_Data *bdata)
{
   _common_poller_core_shutdown(bdata, _fdhandler_finalize);
}

/* Without register just static for all */
static Ecore_Poller_Backend ecore_poller_lazy =
{
  .set_poll_interval = _lazy_poller_poll_interval_set,
  .get_poll_interval = _common_poller_poll_interval_get,
  .add = _lazy_poller_add,
  .set_poller_interval = _lazy_poller_poller_interval_set,
  .get_poller_interval = _common_poller_poller_interval_get,
  .delete =  _lazy_poller_del,
  .shutdown = _lazy_poller_core_shutdown,
  .data = {
    .min_interval = -1,
    .interval_incr = 0,
    .at_tick = 0,
    .just_added_poller = 0,
    .poller_delete_count = 0,
    .poller_walking = 0,
    .poll_interval = 0.125,
    .poll_cur_interval = 0.0,
    .last_tick = 0.0,
    .pollers = {
       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    },
    .poller_counters = {
       0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0
    }
  },
};

#endif /* __linux */

/* TIMERFD part */

/**
 * @}
 */
void
_ecore_poller_shutdown(void)
{
   int i;
   for (i = 0; i < ECORE_POLLER_TYPE_MAX; ++i)
     {
        if (backends[i])
           continue;
        backends[i]->shutdown(&backends[i]->data);
     }
}

/* main part */
static Ecore_Poller_Backend *
_ecore_poller_backend_by_type(Ecore_Poller_Type type)
{
   if (type < ECORE_POLLER_TYPE_MAX)
      return backends[type];

   ERR("Unknown poller");
   return NULL;
}

static Ecore_Poller_Backend *
_ecore_poller_backend_by_poller(Ecore_Poller *poller)
{
   return _ecore_poller_backend_by_type(poller->type);
}

EAPI void
ecore_poller_poll_interval_set(Ecore_Poller_Type type,
                               double            poll_time)
{
   Ecore_Poller_Backend *backend = _ecore_poller_backend_by_type(type);
   if (!backend) return;

   backend->set_poll_interval(&backend->data, poll_time);
}

EAPI double
ecore_poller_poll_interval_get(Ecore_Poller_Type type)
{
   Ecore_Poller_Backend *backend = _ecore_poller_backend_by_type(type);
   return backend ? backend->get_poll_interval(&backend->data) : 0;
}

EAPI Ecore_Poller *
ecore_poller_add(Ecore_Poller_Type type,
                 int               interval,
                 Ecore_Task_Cb     func,
                 const void       *data)
{
   Ecore_Poller_Backend *backend = _ecore_poller_backend_by_type(type);
   return backend ? backend->add(&backend->data, interval, func, data) : NULL;
}

EAPI Eina_Bool
ecore_poller_poller_interval_set(Ecore_Poller *poller,
                                 int           interval)
{
   Ecore_Poller_Backend *backend = _ecore_poller_backend_by_poller(poller);
   return backend ?
      backend->set_poller_interval(&backend->data, poller, interval) : EINA_FALSE;
}

EAPI int
ecore_poller_poller_interval_get(Ecore_Poller *poller)
{
   Ecore_Poller_Backend *backend = _ecore_poller_backend_by_poller(poller);
   return backend ? backend->get_poller_interval(&backend->data, poller) :
      0;
}

EAPI void *
ecore_poller_del(Ecore_Poller *poller)
{
   Ecore_Poller_Backend *backend = _ecore_poller_backend_by_poller(poller);
   return backend ? backend->delete(&backend->data, poller) : NULL;
}

/* Without register just static for all */
static Ecore_Poller_Backend ecore_poller_core =
{
  .set_poll_interval = _ecore_poller_poll_interval_set,
  .get_poll_interval = _common_poller_poll_interval_get,
  .add = _ecore_poller_add,
  .set_poller_interval = _ecore_poller_poller_interval_set,
  .get_poller_interval = _common_poller_poller_interval_get,
  .delete =  _ecore_poller_del,
  .shutdown = _ecore_poller_core_shutdown,
  .data = {
    .min_interval = -1,
    .interval_incr = 0,
    .at_tick = 0,
    .just_added_poller = 0,
    .poller_delete_count = 0,
    .poller_walking = 0,
    .poll_interval = 0.125,
    .poll_cur_interval = 0.0,
    .last_tick = 0.0,
    .pollers = {
       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
       NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    },
    .poller_counters = {
       0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0
    }
  },
};

static Ecore_Poller_Backend *backends[ECORE_POLLER_TYPE_MAX] = {
   [ECORE_POLLER_CORE] = &ecore_poller_core,
#ifdef __linux
   [ECORE_POLLER_LAZY] = &ecore_poller_lazy,
#endif
};

