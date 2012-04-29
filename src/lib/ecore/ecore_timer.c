#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "Ecore.h"
#include "ecore_private.h"

#ifdef WANT_ECORE_TIMER_DUMP
# include <string.h>
# include <execinfo.h>
# define ECORE_TIMER_DEBUG_BT_NUM 64
typedef void (*Ecore_Timer_Bt_Func)();
#endif

struct _Ecore_Timer
{
   EINA_INLIST;
                       ECORE_MAGIC;
   double              in;
   double              at;
   double              pending;
   Ecore_Task_Cb       func;
   void               *data;

#ifdef WANT_ECORE_TIMER_DUMP
   Ecore_Timer_Bt_Func timer_bt[ECORE_TIMER_DEBUG_BT_NUM];
   int                 timer_bt_num;
#endif

   int                 references;
   unsigned char       delete_me : 1;
   unsigned char       just_added : 1;
   unsigned char       frozen : 1;
};
GENERIC_ALLOC_SIZE_DECLARE(Ecore_Timer);

static void _ecore_timer_set(Ecore_Timer  *timer,
                             double        at,
                             double        in,
                             Ecore_Task_Cb func,
                             void         *data);
#ifdef WANT_ECORE_TIMER_DUMP
static int _ecore_timer_cmp(const void *d1,
                            const void *d2);
#endif

static int timers_added = 0;
static int timers_delete_me = 0;
static Ecore_Timer *timers = NULL;
static Ecore_Timer *timer_current = NULL;
static Ecore_Timer *suspended = NULL;
static double last_check = 0.0;
static double precision = 10.0 / 1000000.0;

/**
 * @addtogroup Ecore_Timer_Group
 *
 * @{
 */

/**
 * Retrieves the current precision used by timer infrastructure.
 * @return Current precision.
 * @see ecore_timer_precision_set()
 */
EAPI double
ecore_timer_precision_get(void)
{
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0.0);
   return precision;
}

/**
 * @brief Sets the precision to be used by timer infrastructure.
 *
 * @param value allowed introduced timeout delay, in seconds.
 *
 * This sets the precision for @b all timers. The precision determines how much
 * of an difference from the requested interval is acceptable. One common reason
 * to use this function is to @b increase the allowed timeout and thus @b
 * decrease precision of the timers, this is because less precise the timers
 * result in the system waking up less often and thus consuming less resources.
 *
 * Be aware that kernel may delay delivery even further, these delays
 * are always possible due other tasks having higher priorities or
 * other scheduler policies.
 *
 * Example:
 *  We have 2 timers, one that expires in a 2.0s and another that
 *  expires in 2.1s, if precision is 0.1s, then the Ecore will request
 *  for the next expire to happen in 2.1s and not 2.0s and another one
 *  of 0.1 as it would before.
 *
 * @note Ecore is smart enough to see if there are timers in the
 * precision range, if it does not, in our example if no second timer
 * in (T + precision) existed, then it would use the minimum timeout.
 */
EAPI void
ecore_timer_precision_set(double value)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   _ecore_lock();

   if (value < 0.0)
     {
        ERR("Precision %f less than zero, ignored", value);
        goto unlock;
     }
   precision = value;

unlock:
   _ecore_unlock();
}

/**
 * Creates a timer to call the given function in the given period of time.
 * @param   in   The interval in seconds.
 * @param   func The given function.  If @p func returns 1, the timer is
 *               rescheduled for the next interval @p in.
 * @param   data Data to pass to @p func when it is called.
 * @return  A timer object on success.  @c NULL on failure.
 *
 * This function adds a timer and returns its handle on success and NULL on
 * failure. The function @p func will be called every @p in seconds. The
 * function will be passed the @p data pointer as its parameter.
 *
 * When the timer @p func is called, it must return a value of either 1
 * (or ECORE_CALLBACK_RENEW) or 0 (or ECORE_CALLBACK_CANCEL).
 * If it returns 1, it will be called again at the next tick, or if it returns
 * 0 it will be deleted automatically making any references/handles for it
 * invalid.
 */
EAPI Ecore_Timer *
ecore_timer_add(double        in,
                Ecore_Task_Cb func,
                const void   *data)
{
   double now;
   Ecore_Timer *timer = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();
   if (!func) goto unlock;
   if (in < 0.0) in = 0.0;
   timer = ecore_timer_calloc(1);
   if (!timer) goto unlock;
   ECORE_MAGIC_SET(timer, ECORE_MAGIC_TIMER);
   now = ecore_time_get();

#ifdef WANT_ECORE_TIMER_DUMP
   timer->timer_bt_num = backtrace((void **)(timer->timer_bt),
                                   ECORE_TIMER_DEBUG_BT_NUM);
#endif

   _ecore_timer_set(timer, now + in, in, func, (void *)data);
unlock:
   _ecore_unlock();
   return timer;
}

/**
 * Creates a timer to call the given function in the given period of time.
 * @param   in   The interval in seconds from current loop time.
 * @param   func The given function.  If @p func returns 1, the timer is
 *               rescheduled for the next interval @p in.
 * @param   data Data to pass to @p func when it is called.
 * @return  A timer object on success.  @c NULL on failure.
 *
 * This is the same as ecore_timer_add(), but "now" is the time from
 * ecore_loop_time_get() not ecore_time_get() as ecore_timer_add() uses. See
 * ecore_timer_add() for more details.
 */
EAPI Ecore_Timer *
ecore_timer_loop_add(double        in,
                     Ecore_Task_Cb func,
                     const void   *data)
{
   Ecore_Timer *timer;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();
   timer = _ecore_timer_loop_add(in, func, data);
   _ecore_unlock();

   return timer;
}

/**
 * Delete the specified timer from the timer list.
 * @param   timer The timer to delete.
 * @return  The data pointer set for the timer when @ref ecore_timer_add was
 *          called.  @c NULL is returned if the function is unsuccessful.
 *
 * Note: @p timer must be a valid handle. If the timer function has already
 * returned 0, the handle is no longer valid (and does not need to be delete).
 */
EAPI void *
ecore_timer_del(Ecore_Timer *timer)
{
   void *data = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_del");
        goto unlock;
     }

   data = _ecore_timer_del(timer);

unlock:
   _ecore_unlock();
   return data;
}

/**
 * Change the interval the timer ticks of. If set during
 * a timer call, this will affect the next interval.
 *
 * @param   timer The timer to change.
 * @param   in    The interval in seconds.
 */
EAPI void
ecore_timer_interval_set(Ecore_Timer *timer,
                         double       in)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_interval_set");
        goto unlock;
     }
   timer->in = in;
unlock:
   _ecore_unlock();
}

/**
 * Get the interval the timer ticks on.
 *
 * @param   timer The timer to retrieve the interval from
 * @return  The interval on success. -1 on failure.
 */
EAPI double
ecore_timer_interval_get(Ecore_Timer *timer)
{
   double interval;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0.0);
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_interval_get");
        interval = -1.0;
        goto unlock;
     }

   interval = timer->in;
unlock:
   _ecore_unlock();
   return interval;
}

/**
 * Add some delay for the next occurrence of a timer.
 * This doesn't affect the interval of a timer.
 *
 * @param   timer The timer to change.
 * @param   add   The dalay to add to the next iteration.
 */
EAPI void
ecore_timer_delay(Ecore_Timer *timer,
                  double       add)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_delay");
        return;
     }

   _ecore_lock();
   _ecore_timer_delay(timer, add);
   _ecore_unlock();
}

/**
 * Reset a timer to its full interval
 * This doesn't affect the interval of a timer
 * @param timer The timer
 * @since 1.2
 * @note This is equivalent to (but faster than)
 * @code
 * ecore_timer_delay(timer, ecore_timer_interval_get(timer) - ecore_timer_pending_get(timer));
 * @endcode
 */
EAPI void
ecore_timer_reset(Ecore_Timer *timer)
{
   double now, add;
   EINA_MAIN_LOOP_CHECK_RETURN;
   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         __func__);
        return;
     }
   _ecore_lock();
   now = ecore_time_get();

   if (timer->frozen)
     add = timer->pending;
   else
     add = timer->at - now;
   _ecore_timer_delay(timer, timer->in - add);
   _ecore_unlock();
}

/**
 * Get the pending time regarding a timer.
 *
 * @param        timer The timer to learn from.
 * @return The pending time.
 * @ingroup        Ecore_Timer_Group
 */
EAPI double
ecore_timer_pending_get(Ecore_Timer *timer)
{
   double now;
   double ret = 0.0;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0.0);
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_pending_get");
        goto unlock;
     }

   now = ecore_time_get();

   if (timer->frozen)
     ret = timer->pending;
   else
     ret = timer->at - now;
unlock:
   _ecore_unlock();
   return ret;
}

/**
 * Pauses a running timer.
 *
 * @param timer The timer to be paused.
 *
 * The timer callback won't be called while the timer is paused. The remaining
 * time until the timer expires will be saved, so the timer can be resumed with
 * that same remaining time to expire, instead of expiring instantly.  Use
 * ecore_timer_thaw() to resume it.
 *
 * @note Nothing happens if the timer was already paused.
 *
 * @see ecore_timer_thaw()
 */
EAPI void
ecore_timer_freeze(Ecore_Timer *timer)
{
   double now;

   EINA_MAIN_LOOP_CHECK_RETURN;
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_freeze");
        goto unlock;
     }

   /* Timer already frozen */
   if (timer->frozen)
     goto unlock;

   timers = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(timers), EINA_INLIST_GET(timer));
   suspended = (Ecore_Timer *)eina_inlist_prepend(EINA_INLIST_GET(suspended), EINA_INLIST_GET(timer));

   now = ecore_time_get();

   timer->pending = timer->at - now;
   timer->at = 0.0;
   timer->frozen = 1;
unlock:
   _ecore_unlock();
}

/**
 * Resumes a frozen (paused) timer.
 *
 * @param timer The timer to be resumed.
 *
 * The timer will be resumed from its previous relative position in time. That
 * means, if it had X seconds remaining until expire when it was paused, it will
 * be started now with those same X seconds remaining to expire again. But
 * notice that the interval time won't be touched by this call or by
 * ecore_timer_freeze().
 *
 * @see ecore_timer_freeze()
 */
EAPI void
ecore_timer_thaw(Ecore_Timer *timer)
{
   double now;

   EINA_MAIN_LOOP_CHECK_RETURN;
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(timer, ECORE_MAGIC_TIMER))
     {
        ECORE_MAGIC_FAIL(timer, ECORE_MAGIC_TIMER,
                         "ecore_timer_thaw");
        goto unlock;
     }

   /* Timer not frozen */
   if (!timer->frozen)
     goto unlock;

   suspended = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(suspended), EINA_INLIST_GET(timer));
   now = ecore_time_get();

   _ecore_timer_set(timer, timer->pending + now, timer->in, timer->func, timer->data);
unlock:
   _ecore_unlock();
}

EAPI char *
ecore_timer_dump(void)
{
#ifdef WANT_ECORE_TIMER_DUMP
   Eina_Strbuf *result;
   char *out;
   Ecore_Timer *tm;
   Eina_List *tmp = NULL;
   int living_timer = 0;
   int unknow_timer = 0;

   EINA_MAIN_LOOP_CHECK_RETURN(NULL);
   _ecore_lock();
   result = eina_strbuf_new();

   EINA_INLIST_FOREACH(timers, tm)
     tmp = eina_list_sorted_insert(tmp, _ecore_timer_cmp, tm);

   EINA_LIST_FREE(tmp, tm)
     {
        char **strings;
        int j;

        if (!tm->frozen && !tm->delete_me)
          living_timer++;

        strings = backtrace_symbols((void **)tm->timer_bt, tm->timer_bt_num);
        if (tm->timer_bt_num <= 0 || strings == NULL)
          {
             unknow_timer++;
             continue;
          }

        eina_strbuf_append_printf(result, "*** timer: %f ***\n", tm->in);
        if (tm->frozen)
          eina_strbuf_append(result, "FROZEN\n");
        if (tm->delete_me)
          eina_strbuf_append(result, "DELETED\n");
        for (j = 0; j < tm->timer_bt_num; j++)
          eina_strbuf_append_printf(result, "%s\n", strings[j]);

        free(strings);
     }

   eina_strbuf_append_printf(result, "\n***\nThere is %i living timer.\nWe did lost track of %i timers.\n", living_timer, unknow_timer);

   out = eina_strbuf_string_steal(result);
   eina_strbuf_free(result);
   _ecore_unlock();

   return out;
#else
   return NULL;
#endif
}

/**
 * @}
 */

Ecore_Timer *
_ecore_timer_loop_add(double        in,
                      Ecore_Task_Cb func,
                      const void   *data)
{
   double now;
   Ecore_Timer *timer = NULL;

   if (!func) return timer;
   if (in < 0.0) in = 0.0;
   timer = ecore_timer_calloc(1);
   if (!timer) return timer;
   ECORE_MAGIC_SET(timer, ECORE_MAGIC_TIMER);
   now = ecore_loop_time_get();

#ifdef WANT_ECORE_TIMER_DUMP
   timer->timer_bt_num = backtrace((void **)(timer->timer_bt),
                                   ECORE_TIMER_DEBUG_BT_NUM);
#endif
   _ecore_timer_set(timer, now + in, in, func, (void *)data);
   return timer;
}

EAPI void
_ecore_timer_delay(Ecore_Timer *timer,
                   double       add)
{
   if (timer->frozen)
     {
        timer->pending += add;
     }
   else
     {
        timers = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(timers), EINA_INLIST_GET(timer));
        _ecore_timer_set(timer, timer->at + add, timer->in, timer->func, timer->data);
     }
}

void *
_ecore_timer_del(Ecore_Timer *timer)
{
   if (timer->frozen && !timer->references)
     {
        void *data = timer->data;

        suspended = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(suspended), EINA_INLIST_GET(timer));

        if (timer->delete_me)
          timers_delete_me--;

        ecore_timer_mp_free(timer);
        return data;
     }

   EINA_SAFETY_ON_TRUE_RETURN_VAL(timer->delete_me, NULL);
   timer->delete_me = 1;
   timers_delete_me++;
   return timer->data;
}

void
_ecore_timer_shutdown(void)
{
   Ecore_Timer *timer;

   while ((timer = timers))
     {
        timers = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(timers), EINA_INLIST_GET(timers));
        ECORE_MAGIC_SET(timer, ECORE_MAGIC_NONE);
        ecore_timer_mp_free(timer);
     }

   while ((timer = suspended))
     {
        suspended = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(suspended), EINA_INLIST_GET(suspended));
        ECORE_MAGIC_SET(timer, ECORE_MAGIC_NONE);
        ecore_timer_mp_free(timer);
     }

   timer_current = NULL;
}

void
_ecore_timer_cleanup(void)
{
   Ecore_Timer *l;
   int in_use = 0, todo = timers_delete_me, done = 0;

   if (!timers_delete_me) return;
   for (l = timers; l; )
     {
        Ecore_Timer *timer = l;

        l = (Ecore_Timer *)EINA_INLIST_GET(l)->next;
        if (timer->delete_me)
          {
             if (timer->references)
               {
                  in_use++;
                  continue;
               }
             timers = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(timers), EINA_INLIST_GET(timer));
             ECORE_MAGIC_SET(timer, ECORE_MAGIC_NONE);
             ecore_timer_mp_free(timer);
             timers_delete_me--;
             done++;
             if (timers_delete_me == 0) return;
          }
     }
   for (l = suspended; l; )
     {
        Ecore_Timer *timer = l;

        l = (Ecore_Timer *)EINA_INLIST_GET(l)->next;
        if (timer->delete_me)
          {
             if (timer->references)
               {
                  in_use++;
                  continue;
               }
             suspended = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(suspended), EINA_INLIST_GET(timer));
             ECORE_MAGIC_SET(timer, ECORE_MAGIC_NONE);
             ecore_timer_mp_free(timer);
             timers_delete_me--;
             done++;
             if (timers_delete_me == 0) return;
          }
     }

   if ((!in_use) && (timers_delete_me))
     {
        ERR("%d timers to delete, but they were not found!"
            "Stats: todo=%d, done=%d, pending=%d, in_use=%d. "
            "reset counter.",
            timers_delete_me, todo, done, todo - done, in_use);
        timers_delete_me = 0;
     }
}

void
_ecore_timer_enable_new(void)
{
   Ecore_Timer *timer;

   if (!timers_added) return;
   timers_added = 0;
   EINA_INLIST_FOREACH(timers, timer) timer->just_added = 0;
}

int
_ecore_timers_exists(void)
{
   Ecore_Timer *timer = timers;

   while ((timer) && (timer->delete_me))
     timer = (Ecore_Timer *)EINA_INLIST_GET(timer)->next;

   return !!timer;
}

static inline Ecore_Timer *
_ecore_timer_first_get(void)
{
   Ecore_Timer *timer = timers;

   while ((timer) && ((timer->delete_me) || (timer->just_added)))
     timer = (Ecore_Timer *)EINA_INLIST_GET(timer)->next;

   return timer;
}

static inline Ecore_Timer *
_ecore_timer_after_get(Ecore_Timer *base)
{
   Ecore_Timer *timer = (Ecore_Timer *)EINA_INLIST_GET(base)->next;
   Ecore_Timer *valid_timer = NULL;
   double maxtime = base->at + precision;

   while ((timer) && (timer->at < maxtime))
     {
        if (!((timer->delete_me) || (timer->just_added)))
          valid_timer = timer;
        timer = (Ecore_Timer *)EINA_INLIST_GET(timer)->next;
     }

   return valid_timer;
}

double
_ecore_timer_next_get(void)
{
   double now;
   double in;
   Ecore_Timer *first, *second;

   first = _ecore_timer_first_get();
   if (!first) return -1;

   second = _ecore_timer_after_get(first);
   if (second) first = second;

   now = ecore_loop_time_get();
   in = first->at - now;
   if (in < 0) in = 0;
   return in;
}

static inline void
_ecore_timer_reschedule(Ecore_Timer *timer,
                        double       when)
{
   if ((timer->delete_me) || (timer->frozen)) return;

   timers = (Ecore_Timer *)eina_inlist_remove(EINA_INLIST_GET(timers), EINA_INLIST_GET(timer));

   /* if the timer would have gone off more than 15 seconds ago,
    * assume that the system hung and set the timer to go off
    * timer->in from now. this handles system hangs, suspends
    * and more, so ecore will only "replay" the timers while
    * the system is suspended if it is suspended for less than
    * 15 seconds (basically). this also handles if the process
    * is stopped in a debugger or IO and other handling gets
    * really slow within the main loop.
    */
   if ((timer->at + timer->in) < (when - 15.0))
     _ecore_timer_set(timer, when + timer->in, timer->in, timer->func, timer->data);
   else
     _ecore_timer_set(timer, timer->at + timer->in, timer->in, timer->func, timer->data);
}

/* assume that we hold the ecore lock when entering this function */
void
_ecore_timer_expired_timers_call(double when)
{
   /* call the first expired timer until no expired timers exist */
    while (_ecore_timer_expired_call(when)) ;
}

/* assume that we hold the ecore lock when entering this function */
int
_ecore_timer_expired_call(double when)
{
   if (!timers) return 0;
   if (last_check > when)
     {
        Ecore_Timer *timer;
        /* User set time backwards */
        EINA_INLIST_FOREACH(timers, timer) timer->at -= (last_check - when);
     }
   last_check = when;

   if (!timer_current)
     {
        /* regular main loop, start from head */
         timer_current = timers;
     }
   else
     {
        /* recursive main loop, continue from where we were */
         Ecore_Timer *timer_old = timer_current;
         timer_current = (Ecore_Timer *)EINA_INLIST_GET(timer_current)->next;
         _ecore_timer_reschedule(timer_old, when);
     }

   while (timer_current)
     {
        Ecore_Timer *timer = timer_current;

        if (timer->at > when)
          {
             timer_current = NULL; /* ended walk, next should restart. */
             return 0;
          }

        if ((timer->just_added) || (timer->delete_me))
          {
             timer_current = (Ecore_Timer *)EINA_INLIST_GET(timer_current)->next;
             continue;
          }

        timer->references++;
        if (!_ecore_call_task_cb(timer->func, timer->data))
          {
             if (!timer->delete_me) _ecore_timer_del(timer);
          }
        timer->references--;

        if (timer_current) /* may have changed in recursive main loops */
          timer_current = (Ecore_Timer *)EINA_INLIST_GET(timer_current)->next;

        _ecore_timer_reschedule(timer, when);
     }
   return 0;
}

static void
_ecore_timer_set(Ecore_Timer  *timer,
                 double        at,
                 double        in,
                 Ecore_Task_Cb func,
                 void         *data)
{
   Ecore_Timer *t2;

   timers_added = 1;
   timer->at = at;
   timer->in = in;
   timer->func = func;
   timer->data = data;
   timer->just_added = 1;
   timer->frozen = 0;
   timer->pending = 0.0;
   if (timers)
     {
        EINA_INLIST_REVERSE_FOREACH(EINA_INLIST_GET(timers), t2)
          {
             if (timer->at > t2->at)
               {
                  timers = (Ecore_Timer *)eina_inlist_append_relative(EINA_INLIST_GET(timers), EINA_INLIST_GET(timer), EINA_INLIST_GET(t2));
                  return;
               }
          }
     }
   timers = (Ecore_Timer *)eina_inlist_prepend(EINA_INLIST_GET(timers), EINA_INLIST_GET(timer));
}

#ifdef WANT_ECORE_TIMER_DUMP
static int
_ecore_timer_cmp(const void *d1,
                 const void *d2)
{
   const Ecore_Timer *t1 = d1;
   const Ecore_Timer *t2 = d2;

   return (int)((t1->in - t2->in) * 100);
}
#endif
