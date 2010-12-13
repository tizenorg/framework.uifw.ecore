#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>

#include "Ecore.h"
#include "Ecore_Input.h"

#include "Ecore_Input_Evas.h"
#include "ecore_input_evas_private.h"

int _ecore_input_evas_log_dom = -1;

typedef struct _Ecore_Input_Window Ecore_Input_Window;
struct _Ecore_Input_Window
{
   Evas *evas;
   void *window;
   Ecore_Event_Mouse_Move_Cb move_mouse;
   Ecore_Event_Multi_Move_Cb move_multi;
   Ecore_Event_Multi_Down_Cb down_multi;
   Ecore_Event_Multi_Up_Cb up_multi;
   int ignore_event;
};

static int _ecore_event_evas_init_count = 0;
static Ecore_Event_Handler *ecore_event_evas_handlers[8];
static Eina_Hash *_window_hash = NULL;

EAPI void
ecore_event_evas_modifier_lock_update(Evas *e, unsigned int modifiers)
{
   if (modifiers & ECORE_EVENT_MODIFIER_SHIFT)
     evas_key_modifier_on(e, "Shift");
   else evas_key_modifier_off(e, "Shift");

   if (modifiers & ECORE_EVENT_MODIFIER_CTRL)
     evas_key_modifier_on(e, "Control");
   else evas_key_modifier_off(e, "Control");

   if (modifiers & ECORE_EVENT_MODIFIER_ALT)
     evas_key_modifier_on(e, "Alt");
   else evas_key_modifier_off(e, "Alt");

   if (modifiers & ECORE_EVENT_MODIFIER_WIN)
     {
        evas_key_modifier_on(e, "Super");
        evas_key_modifier_on(e, "Hyper");
     }
   else
     {
        evas_key_modifier_off(e, "Super");
        evas_key_modifier_off(e, "Hyper");
     }

   if (modifiers & ECORE_EVENT_LOCK_SCROLL)
     evas_key_lock_on(e, "Scroll_Lock");
   else evas_key_lock_off(e, "Scroll_Lock");

   if (modifiers & ECORE_EVENT_LOCK_NUM)
     evas_key_lock_on(e, "Num_Lock");
   else evas_key_lock_off(e, "Num_Lock");

   if (modifiers & ECORE_EVENT_LOCK_CAPS)
     evas_key_lock_on(e, "Caps_Lock");
   else evas_key_lock_off(e, "Caps_Lock");
}

EAPI void
ecore_event_window_register(Ecore_Window id, void *window, Evas *evas,
                            Ecore_Event_Mouse_Move_Cb move_mouse,
                            Ecore_Event_Multi_Move_Cb move_multi,
                            Ecore_Event_Multi_Down_Cb down_multi,
                            Ecore_Event_Multi_Up_Cb up_multi)
{
   Ecore_Input_Window *w;

   w = calloc(1, sizeof(Ecore_Input_Window));
   if (!w) return;

   w->evas = evas;
   w->window = window;
   w->move_mouse = move_mouse;
   w->move_multi = move_multi;
   w->down_multi = down_multi;
   w->up_multi = up_multi;
   w->ignore_event = 0;

   eina_hash_add(_window_hash, &id, w);

   evas_key_modifier_add(evas, "Shift");
   evas_key_modifier_add(evas, "Control");
   evas_key_modifier_add(evas, "Alt");
   evas_key_modifier_add(evas, "Meta");
   evas_key_modifier_add(evas, "Hyper");
   evas_key_modifier_add(evas, "Super");
   evas_key_lock_add(evas, "Caps_Lock");
   evas_key_lock_add(evas, "Num_Lock");
   evas_key_lock_add(evas, "Scroll_Lock");
}

EAPI void
ecore_event_window_unregister(Ecore_Window id)
{
   eina_hash_del(_window_hash, &id, NULL);
}

EAPI void *
ecore_event_window_match(Ecore_Window id)
{
   Ecore_Input_Window *lookup;

   lookup = eina_hash_find(_window_hash, &id);
   if (lookup) return lookup->window;
   return NULL;
}

EAPI void
ecore_event_window_ignore_events(Ecore_Window id, int ignore_event)
{
   Ecore_Input_Window *lookup;

   lookup = eina_hash_find(_window_hash, &id);
   if (!lookup) return;
   lookup->ignore_event = ignore_event;
}

static Ecore_Input_Window*
_ecore_event_window_match(Ecore_Window id)
{
   Ecore_Input_Window *lookup;

   lookup = eina_hash_find(_window_hash, &id);
   if (!lookup) return NULL;
   if (lookup->ignore_event) return NULL; /* Pass on event. */
   return lookup;
}

static Eina_Bool
_ecore_event_evas_key(Ecore_Event_Key *e, Ecore_Event_Press press)
{
   Ecore_Input_Window *lookup;

   lookup = _ecore_event_window_match(e->event_window);
   if (!lookup) return ECORE_CALLBACK_RENEW;
   ecore_event_evas_modifier_lock_update(lookup->evas, e->modifiers);
   if (press == ECORE_DOWN)
     evas_event_feed_key_down(lookup->evas, e->keyname, e->key, e->string, e->compose, e->timestamp, NULL);
   else
     evas_event_feed_key_up(lookup->evas, e->keyname, e->key, e->string, e->compose, e->timestamp, NULL);
   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_ecore_event_evas_mouse_button(Ecore_Event_Mouse_Button *e, Ecore_Event_Press press)
{
   Ecore_Input_Window *lookup;
   Evas_Button_Flags flags = EVAS_BUTTON_NONE;

   lookup = _ecore_event_window_match(e->event_window);
   if (!lookup) return ECORE_CALLBACK_RENEW;
   if (e->double_click) flags |= EVAS_BUTTON_DOUBLE_CLICK;
   if (e->triple_click) flags |= EVAS_BUTTON_TRIPLE_CLICK;
   if (e->multi.device == 0)
     {
        ecore_event_evas_modifier_lock_update(lookup->evas, e->modifiers);
        if (press == ECORE_DOWN)
          evas_event_feed_mouse_down(lookup->evas, e->buttons, flags, 
                                     e->timestamp, NULL);
        else
          evas_event_feed_mouse_up(lookup->evas, e->buttons, flags, 
                                   e->timestamp, NULL);
     }
   else
     {
        if (press == ECORE_DOWN)
          {
             if (lookup->down_multi)
                lookup->down_multi(lookup->window, e->multi.device, 
                                   e->x, e->y, e->multi.radius, 
                                   e->multi.radius_x, e->multi.radius_y, 
                                   e->multi.pressure, e->multi.angle, 
                                   e->multi.x, e->multi.y, flags, 
                                   e->timestamp);
             else
                evas_event_feed_multi_down(lookup->evas, e->multi.device, 
                                           e->x, e->y, e->multi.radius, 
                                           e->multi.radius_x, e->multi.radius_y, 
                                           e->multi.pressure, e->multi.angle, 
                                           e->multi.x, e->multi.y, flags, 
                                           e->timestamp, NULL);
          }
        else
          {
             if (lookup->up_multi)
                lookup->up_multi(lookup->window, e->multi.device, 
                                 e->x, e->y, e->multi.radius, 
                                 e->multi.radius_x, e->multi.radius_y, 
                                 e->multi.pressure, e->multi.angle, 
                                 e->multi.x, e->multi.y, flags, 
                                 e->timestamp);
             else
                evas_event_feed_multi_up(lookup->evas, e->multi.device, 
                                         e->x, e->y, e->multi.radius, 
                                         e->multi.radius_x, e->multi.radius_y, 
                                         e->multi.pressure, e->multi.angle, 
                                         e->multi.x, e->multi.y, flags, 
                                         e->timestamp, NULL);
          }
     }
   return ECORE_CALLBACK_RENEW;
}

EAPI Eina_Bool
ecore_event_evas_mouse_move(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Mouse_Move *e;
   Ecore_Input_Window *lookup;

   e = event;
   lookup = _ecore_event_window_match(e->event_window);
   if (!lookup) return ECORE_CALLBACK_RENEW;
   if (e->multi.device == 0)
     {
        ecore_event_evas_modifier_lock_update(lookup->evas, e->modifiers);
        if (lookup->move_mouse)
           lookup->move_mouse(lookup->window, e->x, e->y, e->timestamp);
        else
           evas_event_feed_mouse_move(lookup->evas, e->x, e->y, e->timestamp, 
                                      NULL);
     }
   else
     {
        if (lookup->move_multi)
           lookup->move_multi(lookup->window, e->multi.device, 
                              e->x, e->y, e->multi.radius, 
                              e->multi.radius_x, e->multi.radius_y, 
                              e->multi.pressure, e->multi.angle, 
                              e->multi.x, e->multi.y, e->timestamp);
        else
           evas_event_feed_multi_move(lookup->evas, e->multi.device, 
                                      e->x, e->y, e->multi.radius, 
                                      e->multi.radius_x, e->multi.radius_y, 
                                      e->multi.pressure, e->multi.angle, 
                                      e->multi.x, e->multi.y, e->timestamp, 
                                      NULL);
     }
   return ECORE_CALLBACK_RENEW;
}

EAPI Eina_Bool
ecore_event_evas_mouse_button_down(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   return _ecore_event_evas_mouse_button((Ecore_Event_Mouse_Button *)event, ECORE_DOWN);
}

EAPI Eina_Bool
ecore_event_evas_mouse_button_up(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   return _ecore_event_evas_mouse_button((Ecore_Event_Mouse_Button *)event, ECORE_UP);
}

static Eina_Bool
_ecore_event_evas_mouse_io(Ecore_Event_Mouse_IO *e, Ecore_Event_IO io)
{
   Ecore_Input_Window *lookup;

   lookup = _ecore_event_window_match(e->event_window);
   if (!lookup) return ECORE_CALLBACK_RENEW;
   ecore_event_evas_modifier_lock_update(lookup->evas, e->modifiers);
   switch (io)
     {
      case ECORE_IN:
         evas_event_feed_mouse_in(lookup->evas, e->timestamp, NULL);
         break;
      case ECORE_OUT:
         evas_event_feed_mouse_out(lookup->evas, e->timestamp, NULL);
         break;
      default:
         break;
     }

   lookup->move_mouse(lookup->window, e->x, e->y, e->timestamp);
   return ECORE_CALLBACK_RENEW;
}

EAPI Eina_Bool
ecore_event_evas_key_down(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   return _ecore_event_evas_key((Ecore_Event_Key *)event, ECORE_DOWN);
}

EAPI Eina_Bool
ecore_event_evas_key_up(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   return _ecore_event_evas_key((Ecore_Event_Key *)event, ECORE_UP);
}

EAPI Eina_Bool
ecore_event_evas_mouse_wheel(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Mouse_Wheel *e;
   Ecore_Input_Window *lookup;

   e = event;
   lookup = _ecore_event_window_match(e->event_window);
   if (!lookup) return ECORE_CALLBACK_RENEW;
   ecore_event_evas_modifier_lock_update(lookup->evas, e->modifiers);
   evas_event_feed_mouse_wheel(lookup->evas, e->direction, e->z, e->timestamp, NULL);
   return ECORE_CALLBACK_RENEW;
}

EAPI Eina_Bool
ecore_event_evas_mouse_in(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   return _ecore_event_evas_mouse_io((Ecore_Event_Mouse_IO *)event, ECORE_IN);
}

EAPI Eina_Bool
ecore_event_evas_mouse_out(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   return _ecore_event_evas_mouse_io((Ecore_Event_Mouse_IO *)event, ECORE_OUT);
}

EAPI int
ecore_event_evas_init(void)
{
   if (++_ecore_event_evas_init_count !=  1)
     return _ecore_event_evas_init_count;

   _ecore_input_evas_log_dom = eina_log_domain_register
     ("ecore_input_evas",  ECORE_INPUT_EVAS_DEFAULT_LOG_COLOR);
   if (_ecore_input_evas_log_dom < 0)
     {
        EINA_LOG_ERR("Impossible to create a log domain for the ecore input evas_module.");
        return --_ecore_event_evas_init_count;
     }

   if (!ecore_init())
     {
        return --_ecore_event_evas_init_count;
     }

   if (!ecore_event_init())
     {
        goto shutdown_ecore;
     }

   ecore_event_evas_handlers[0] = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN,
                                                          ecore_event_evas_key_down,
                                                          NULL);
   ecore_event_evas_handlers[1] = ecore_event_handler_add(ECORE_EVENT_KEY_UP,
                                                          ecore_event_evas_key_up,
                                                          NULL);
   ecore_event_evas_handlers[2] = ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_DOWN,
                                                          ecore_event_evas_mouse_button_down,
                                                          NULL);
   ecore_event_evas_handlers[3] = ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_UP,
                                                          ecore_event_evas_mouse_button_up,
                                                          NULL);
   ecore_event_evas_handlers[4] = ecore_event_handler_add(ECORE_EVENT_MOUSE_MOVE,
                                                          ecore_event_evas_mouse_move,
                                                          NULL);
   ecore_event_evas_handlers[5] = ecore_event_handler_add(ECORE_EVENT_MOUSE_WHEEL,
                                                          ecore_event_evas_mouse_wheel,
                                                          NULL);
   ecore_event_evas_handlers[6] = ecore_event_handler_add(ECORE_EVENT_MOUSE_IN,
                                                          ecore_event_evas_mouse_in,
                                                          NULL);
   ecore_event_evas_handlers[7] = ecore_event_handler_add(ECORE_EVENT_MOUSE_OUT,
                                                          ecore_event_evas_mouse_out,
                                                          NULL);
   
   _window_hash = eina_hash_pointer_new(free);
   
   return _ecore_event_evas_init_count;
   
   shutdown_ecore:
   ecore_shutdown();
   
   return --_ecore_event_evas_init_count;
}

EAPI int
ecore_event_evas_shutdown(void)
{
   size_t i;
   
   if (--_ecore_event_evas_init_count != 0)
     return _ecore_event_evas_init_count;
   
   eina_hash_free(_window_hash);
   _window_hash = NULL;
   for (i = 0; i < sizeof(ecore_event_evas_handlers) / sizeof(Ecore_Event_Handler *); i++)
     {
        ecore_event_handler_del(ecore_event_evas_handlers[i]);
        ecore_event_evas_handlers[i] = NULL;
     }
   
   ecore_event_shutdown();
   ecore_shutdown();
   
   eina_log_domain_unregister(_ecore_input_evas_log_dom);
   _ecore_input_evas_log_dom = -1;
   
   return _ecore_event_evas_init_count;
}
