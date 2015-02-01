#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_private.h"

struct _Ecore_Idle_Enterer
{
   EINA_INLIST;
                 ECORE_MAGIC;
   Ecore_Task_Cb func;
   void         *data;
   int           references;
   Eina_Bool     delete_me : 1;
};
GENERIC_ALLOC_SIZE_DECLARE(Ecore_Idle_Enterer);

static Ecore_Idle_Enterer *idle_enterers = NULL;
static Ecore_Idle_Enterer *idle_enterer_current = NULL;
static int idle_enterers_delete_me = 0;

static void *
_ecore_idle_enterer_del(Ecore_Idle_Enterer *idle_enterer);

/**
 * @addtogroup Ecore_Idle_Group
 *
 * @{
 */

/**
 * Add an idle enterer handler.
 * @param   func The function to call when entering an idle state.
 * @param   data The data to be passed to the @p func call
 * @return  A handle to the idle enterer callback if successful.  Otherwise,
 *          NULL is returned.
 * @note The function func will be called every time the main loop is entering
 * idle state, as long as it returns 1 (or ECORE_CALLBACK_RENEW). A return of 0
 * (or ECORE_CALLBACK_CANCEL) deletes the idle enterer.
 */
EAPI Ecore_Idle_Enterer *
ecore_idle_enterer_add(Ecore_Task_Cb func,
                       const void   *data)
{
   Ecore_Idle_Enterer *ie = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();

   if (!func) goto unlock;
   ie = ecore_idle_enterer_calloc(1);
   if (!ie) goto unlock;
   ECORE_MAGIC_SET(ie, ECORE_MAGIC_IDLE_ENTERER);
   ie->func = func;
   ie->data = (void *)data;
   idle_enterers = (Ecore_Idle_Enterer *)eina_inlist_append(EINA_INLIST_GET(idle_enterers), EINA_INLIST_GET(ie));
unlock:
   _ecore_unlock();
   return ie;
}

/**
 * Add an idle enterer handler at the start of the list so it gets called earlier than others.
 * @param   func The function to call when entering an idle state.
 * @param   data The data to be passed to the @p func call
 * @return  A handle to the idle enterer callback if successful.  Otherwise,
 *          NULL is returned.
 * @note The function func will be called every time the main loop is entering
 * idle state, as long as it returns 1 (or ECORE_CALLBACK_RENEW). A return of 0
 * (or ECORE_CALLBACK_CANCEL) deletes the idle enterer.
 */
EAPI Ecore_Idle_Enterer *
ecore_idle_enterer_before_add(Ecore_Task_Cb func,
                              const void   *data)
{
   Ecore_Idle_Enterer *ie = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();

   if (!func) goto unlock;
   ie = ecore_idle_enterer_calloc(1);
   if (!ie) goto unlock;
   ECORE_MAGIC_SET(ie, ECORE_MAGIC_IDLE_ENTERER);
   ie->func = func;
   ie->data = (void *)data;
   idle_enterers = (Ecore_Idle_Enterer *)eina_inlist_prepend(EINA_INLIST_GET(idle_enterers), EINA_INLIST_GET(ie));
unlock:
   _ecore_unlock();
   return ie;
}

/**
 * Delete an idle enterer callback.
 * @param   idle_enterer The idle enterer to delete
 * @return  The data pointer passed to the idler enterer callback on success.
 *          NULL otherwise.
 */
EAPI void *
ecore_idle_enterer_del(Ecore_Idle_Enterer *idle_enterer)
{
   void *data;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   if (!ECORE_MAGIC_CHECK(idle_enterer, ECORE_MAGIC_IDLE_ENTERER))
     {
        ECORE_MAGIC_FAIL(idle_enterer, ECORE_MAGIC_IDLE_ENTERER,
                         "ecore_idle_enterer_del");
        return NULL;
     }
   _ecore_lock();
   data = _ecore_idle_enterer_del(idle_enterer);
   _ecore_unlock();
   return data;
}

/**
 * @}
 */

static void *
_ecore_idle_enterer_del(Ecore_Idle_Enterer *idle_enterer)
{
   EINA_SAFETY_ON_TRUE_RETURN_VAL(idle_enterer->delete_me, NULL);
   idle_enterer->delete_me = 1;
   idle_enterers_delete_me = 1;
   return idle_enterer->data;
}

void
_ecore_idle_enterer_shutdown(void)
{
   Ecore_Idle_Enterer *ie;
   while ((ie = idle_enterers))
     {
        idle_enterers = (Ecore_Idle_Enterer *)eina_inlist_remove(EINA_INLIST_GET(idle_enterers), EINA_INLIST_GET(idle_enterers));
        ECORE_MAGIC_SET(ie, ECORE_MAGIC_NONE);
        ecore_idle_enterer_mp_free(ie);
     }
   idle_enterers_delete_me = 0;
   idle_enterer_current = NULL;
}

void
_ecore_idle_enterer_call(void)
{
   if (!idle_enterer_current)
     {
        /* regular main loop, start from head */
         idle_enterer_current = idle_enterers;
     }
   else
     {
        /* recursive main loop, continue from where we were */
         idle_enterer_current =
           (Ecore_Idle_Enterer *)EINA_INLIST_GET(idle_enterer_current)->next;
     }

   while (idle_enterer_current)
     {
        Ecore_Idle_Enterer *ie = (Ecore_Idle_Enterer *)idle_enterer_current;
        if (!ie->delete_me)
          {
             ie->references++;
             if (!_ecore_call_task_cb(ie->func, ie->data))
               {
                  if (!ie->delete_me) _ecore_idle_enterer_del(ie);
               }
             ie->references--;
          }
        if (idle_enterer_current) /* may have changed in recursive main loops */
          idle_enterer_current =
            (Ecore_Idle_Enterer *)EINA_INLIST_GET(idle_enterer_current)->next;
     }
   if (idle_enterers_delete_me)
     {
        Ecore_Idle_Enterer *l;
        int deleted_idler_enterers_in_use = 0;

        for (l = idle_enterers; l; )
          {
             Ecore_Idle_Enterer *ie = l;
             l = (Ecore_Idle_Enterer *)EINA_INLIST_GET(l)->next;
             if (ie->delete_me)
               {
                  if (ie->references)
                    {
                       deleted_idler_enterers_in_use++;
                       continue;
                    }

                  idle_enterers = (Ecore_Idle_Enterer *)eina_inlist_remove(EINA_INLIST_GET(idle_enterers), EINA_INLIST_GET(ie));
                  ECORE_MAGIC_SET(ie, ECORE_MAGIC_NONE);
                  ecore_idle_enterer_mp_free(ie);
               }
          }
        if (!deleted_idler_enterers_in_use)
          idle_enterers_delete_me = 0;
     }
}

int
_ecore_idle_enterer_exist(void)
{
   if (idle_enterers) return 1;
   return 0;
}

