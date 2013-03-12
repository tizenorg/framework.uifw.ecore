#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_private.h"

struct _Ecore_Idler
{
   EINA_INLIST;
                 ECORE_MAGIC;
   Ecore_Task_Cb func;
   void         *data;
   int           references;
   Eina_Bool     delete_me : 1;
};
GENERIC_ALLOC_SIZE_DECLARE(Ecore_Idler);

static Ecore_Idler *idlers = NULL;
static Ecore_Idler *idler_current = NULL;
static int idlers_delete_me = 0;

static void *
_ecore_idler_del(Ecore_Idler *idler);

EAPI Ecore_Idler *
ecore_idler_add(Ecore_Task_Cb func,
                const void   *data)
{
   Ecore_Idler *ie = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();
   if (!func) goto unlock;
   ie = ecore_idler_calloc(1);
   if (!ie) goto unlock;
   ECORE_MAGIC_SET(ie, ECORE_MAGIC_IDLER);
   ie->func = func;
   ie->data = (void *)data;
   idlers = (Ecore_Idler *)eina_inlist_append(EINA_INLIST_GET(idlers), EINA_INLIST_GET(ie));
unlock:
   _ecore_unlock();
   return ie;
}

EAPI void *
ecore_idler_del(Ecore_Idler *idler)
{
   void *data = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   if (!ECORE_MAGIC_CHECK(idler, ECORE_MAGIC_IDLER))
     {
        ECORE_MAGIC_FAIL(idler, ECORE_MAGIC_IDLER,
                         "ecore_idler_del");
        return NULL;
     }

   _ecore_lock();
   data = _ecore_idler_del(idler);
   _ecore_unlock();
   return data;
}

/**
 * @}
 */

/**
 * @}
 */

static void *
_ecore_idler_del(Ecore_Idler *idler)
{
   EINA_SAFETY_ON_TRUE_RETURN_VAL(idler->delete_me, NULL);
   idler->delete_me = 1;
   idlers_delete_me = 1;
   return idler->data;
}

void
_ecore_idler_shutdown(void)
{
   Ecore_Idler *ie;
   while ((ie = idlers))
     {
        idlers = (Ecore_Idler *)eina_inlist_remove(EINA_INLIST_GET(idlers), EINA_INLIST_GET(idlers));
        ECORE_MAGIC_SET(ie, ECORE_MAGIC_NONE);
        ecore_idler_mp_free(ie);
     }
   idlers_delete_me = 0;
   idler_current = NULL;
}

int
_ecore_idler_all_call(void)
{
   if (!idler_current)
     {
        /* regular main loop, start from head */
         idler_current = idlers;
     }
   else
     {
        /* recursive main loop, continue from where we were */
         idler_current = (Ecore_Idler *)EINA_INLIST_GET(idler_current)->next;
     }

   while (idler_current)
     {
        Ecore_Idler *ie = (Ecore_Idler *)idler_current;
        if (!ie->delete_me)
          {
             ie->references++;
             if (!_ecore_call_task_cb(ie->func, ie->data))
               {
                  if (!ie->delete_me) _ecore_idler_del(ie);
               }
             ie->references--;
          }
        if (idler_current) /* may have changed in recursive main loops */
          idler_current = (Ecore_Idler *)EINA_INLIST_GET(idler_current)->next;
     }
   if (idlers_delete_me)
     {
        Ecore_Idler *l;
        int deleted_idlers_in_use = 0;
        for (l = idlers; l; )
          {
             Ecore_Idler *ie = l;
             l = (Ecore_Idler *)EINA_INLIST_GET(l)->next;
             if (ie->delete_me)
               {
                  if (ie->references)
                    {
                       deleted_idlers_in_use++;
                       continue;
                    }

                  idlers = (Ecore_Idler *)eina_inlist_remove(EINA_INLIST_GET(idlers), EINA_INLIST_GET(ie));
                  ECORE_MAGIC_SET(ie, ECORE_MAGIC_NONE);
                  ecore_idler_mp_free(ie);
               }
          }
        if (!deleted_idlers_in_use)
          idlers_delete_me = 0;
     }
   if (idlers) return 1;
   return 0;
}

int
_ecore_idler_exist(void)
{
   if (idlers) return 1;
   return 0;
}

