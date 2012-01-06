#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>

#include <Ecore.h>
#include "ecore_private.h"
#ifdef BUILD_ECORE_EVAS_DIRECTFB
#include <Ecore_DirectFB.h>
#endif

#include "ecore_evas_private.h"
#include "Ecore_Evas.h"

#ifdef BUILD_ECORE_EVAS_DIRECTFB
static int _ecore_evas_init_count = 0;
static Ecore_Event_Handler *ecore_evas_event_handlers[13];

static Eina_Hash *ecore_evases_hash = NULL;

static int
_ecore_evas_directfb_render(Ecore_Evas *ee)
{
   Eina_List *updates, *ll;
   Ecore_Evas *ee2;
   int rend = 0;

   EINA_LIST_FOREACH(ee->sub_ecore_evas, ll, ee2)
     {
        if (ee2->func.fn_pre_render) ee2->func.fn_pre_render(ee2);
        if (ee2->engine.func->fn_render)
          rend |= ee2->engine.func->fn_render(ee2);
        if (ee2->func.fn_post_render) ee2->func.fn_post_render(ee2);
     }

   if (ee->func.fn_pre_render) ee->func.fn_pre_render(ee);
   updates = evas_render_updates(ee->evas);
   if (updates)
     {
        evas_render_updates_free(updates);
        _ecore_evas_idle_timeout_update(ee);
     }
   if (ee->func.fn_post_render) ee->func.fn_post_render(ee);

   return updates ? 1 : rend;
}

static char *
_ecore_evas_directfb_winid_str_get(Ecore_X_Window win)
{
   const char *vals = "qWeRtYuIoP5$&<~";
   static char id[9];
   unsigned int val;
   val = (unsigned int)win;
   id[0] = vals[(val >> 28) & 0xf];
   id[1] = vals[(val >> 24) & 0xf];
   id[2] = vals[(val >> 20) & 0xf];
   id[3] = vals[(val >> 16) & 0xf];
   id[4] = vals[(val >> 12) & 0xf];
   id[5] = vals[(val >>  8) & 0xf];
   id[6] = vals[(val >>  4) & 0xf];
   id[7] = vals[(val      ) & 0xf];
   id[8] = 0;
   return id;
}

static Ecore_Evas *
_ecore_evas_directfb_match(DFBWindowID win)
{
   Ecore_Evas *ee;

   ee = eina_hash_find(ecore_evases_hash, _ecore_evas_directfb_winid_str_get(win));
   return ee;
}

static Eina_Bool
_ecore_evas_directfb_event_key_down(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Key_Down *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   evas_event_feed_key_down(ee->evas, e->name, e->name, e->string,
                            e->key_compose, e->time, NULL);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_key_up(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Key_Up *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   evas_event_feed_key_up(ee->evas, e->name, e->name, e->string,
                          e->key_compose, e->time, NULL);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_motion(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Motion *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   _ecore_evas_mouse_move_process(ee, e->x, e->y, e->time);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_button_down(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Button_Down *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   // _ecore_evas_mouse_move_process(ee, e->x, e->y, e->time);
   evas_event_feed_mouse_down(ee->evas, e->button, EVAS_BUTTON_NONE, e->time, NULL);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_button_up(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Button_Up *e;
   Evas_Button_Flags flags = EVAS_BUTTON_NONE;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   //_ecore_evas_mouse_move_process(ee, e->x, e->y, e->time);
   evas_event_feed_mouse_up(ee->evas, e->button, flags, e->time, NULL);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_enter(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Enter *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   evas_event_feed_mouse_in(ee->evas, e->time, NULL);
   //_ecore_evas_mouse_move_process(ee, e->x, e->y, e->time);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_leave(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Leave *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   evas_event_feed_mouse_out(ee->evas, e->time, NULL);
   //_ecore_evas_mouse_move_process(ee, e->x, e->y, e->time);
   if (ee->func.fn_mouse_out) ee->func.fn_mouse_out(ee);
   if (ee->prop.cursor.object) evas_object_hide(ee->prop.cursor.object);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_wheel(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Wheel *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   evas_event_feed_mouse_wheel(ee->evas, e->direction, e->z, e->time, NULL);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_got_focus(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Got_Focus *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   ee->prop.focused = 1;
   return EINA_TRUE;
}

static Eina_Bool
_ecore_evas_directfb_event_lost_focus(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Evas *ee;
   Ecore_DirectFB_Event_Lost_Focus *e;

   e = event;
   ee = _ecore_evas_directfb_match(e->win);

   if (!ee) return EINA_TRUE; /* pass on event */
   ee->prop.focused = 0;
   return EINA_TRUE;
}

int
_ecore_evas_directfb_shutdown(void)
{
   _ecore_evas_init_count--;
   if (_ecore_evas_init_count == 0)
     {
        int i;

        for (i = 0; i < 8; i++)
          ecore_event_handler_del(ecore_evas_event_handlers[i]);
     }
   if (_ecore_evas_init_count < 0) _ecore_evas_init_count = 0;
   return _ecore_evas_init_count;
}





int
_ecore_evas_directfb_init(void)
{
   _ecore_evas_init_count++;
   if (_ecore_evas_init_count > 1) return _ecore_evas_init_count;

   ecore_evas_event_handlers[0]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_KEY_DOWN, _ecore_evas_directfb_event_key_down, NULL);
   ecore_evas_event_handlers[1]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_KEY_UP, _ecore_evas_directfb_event_key_up, NULL);
   ecore_evas_event_handlers[2]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_BUTTON_DOWN, _ecore_evas_directfb_event_button_down, NULL);
   ecore_evas_event_handlers[3]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_BUTTON_UP, _ecore_evas_directfb_event_button_up, NULL);
   ecore_evas_event_handlers[4]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_MOTION, _ecore_evas_directfb_event_motion, NULL);
   ecore_evas_event_handlers[5]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_ENTER, _ecore_evas_directfb_event_enter, NULL);
   ecore_evas_event_handlers[6]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_LEAVE, _ecore_evas_directfb_event_leave, NULL);
   ecore_evas_event_handlers[7]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_WHEEL, _ecore_evas_directfb_event_wheel, NULL);
   ecore_evas_event_handlers[8]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_GOT_FOCUS, _ecore_evas_directfb_event_got_focus, NULL);
   ecore_evas_event_handlers[9]  = ecore_event_handler_add(ECORE_DIRECTFB_EVENT_LOST_FOCUS, _ecore_evas_directfb_event_lost_focus, NULL);
   ecore_evas_event_handlers[10]  = NULL;
   ecore_evas_event_handlers[11]  = NULL;
   ecore_evas_event_handlers[12]  = NULL;

   return _ecore_evas_init_count;
}

/* engine functions */
/********************/

static void
_ecore_evas_directfb_free(Ecore_Evas *ee)
{
   eina_hash_del(ecore_evases_hash, _ecore_evas_directfb_winid_str_get(ee->engine.directfb.window->id), ee);
   ecore_directfb_window_free(ee->engine.directfb.window);
   _ecore_evas_directfb_shutdown();
   ecore_directfb_shutdown();
}

static void
_ecore_evas_directfb_move(Ecore_Evas *ee, int x, int y)
{
   ecore_directfb_window_move(ee->engine.directfb.window, x, y);
}

static void
_ecore_evas_directfb_resize(Ecore_Evas *ee, int w, int h)
{
   ee->req.w = w;
   ee->req.h = h;
   if ((w == ee->w) && (h == ee->h)) return;
   ecore_directfb_window_resize(ee->engine.directfb.window, w, h);
   ee->w = w;
   ee->h = h;
   if ((ee->rotation == 90) || (ee->rotation == 270))
     {
        evas_output_size_set(ee->evas, ee->h, ee->w);
        evas_output_viewport_set(ee->evas, 0, 0, ee->h, ee->w);
     }
   else
     {
        evas_output_size_set(ee->evas, ee->w, ee->h);
        evas_output_viewport_set(ee->evas, 0, 0, ee->w, ee->h);
     }
}

static void
_ecore_evas_directfb_focus_set(Ecore_Evas *ee, int on __UNUSED__)
{
   ecore_directfb_window_focus(ee->engine.directfb.window);
}

static void
_ecore_evas_directfb_hide(Ecore_Evas *ee)
{
   ecore_directfb_window_hide(ee->engine.directfb.window);
   ee->should_be_visible = 0;
}

static void
_ecore_evas_directfb_show(Ecore_Evas *ee)
{
   ecore_directfb_window_show(ee->engine.directfb.window);
   ee->should_be_visible = 1;
}

static void
_ecore_evas_directfb_shaped_set(Ecore_Evas *ee, int shaped)
{
   if (((ee->shaped) && (shaped)) || ((!ee->shaped) && (!shaped)))
     return;
   ee->shaped = shaped;
   if(ee->shaped)
     ecore_directfb_window_shaped_set(ee->engine.directfb.window, 1);
   else
     ecore_directfb_window_shaped_set(ee->engine.directfb.window, 0);

}

static void
_ecore_evas_object_cursor_del(void *data, Evas *e __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Ecore_Evas *ee;

   ee = data;
   if (ee)
     ee->prop.cursor.object = NULL;
}

static void
_ecore_evas_directfb_object_cursor_set(Ecore_Evas *ee, Evas_Object *obj, int layer, int hot_x, int hot_y)
{
   int x, y;

   if (ee->prop.cursor.object) evas_object_del(ee->prop.cursor.object);

   if (!obj)
     {
        ee->prop.cursor.object = NULL;
        ee->prop.cursor.layer = 0;
        ee->prop.cursor.hot.x = 0;
        ee->prop.cursor.hot.y = 0;
        ecore_directfb_window_cursor_show(ee->engine.directfb.window, 1);
        return;

     }

   ee->prop.cursor.object = obj;
   ee->prop.cursor.layer = layer;
   ee->prop.cursor.hot.x = hot_x;
   ee->prop.cursor.hot.y = hot_y;

   ecore_directfb_window_cursor_show(ee->engine.directfb.window, 0);

   evas_pointer_output_xy_get(ee->evas, &x, &y);
   evas_object_layer_set(ee->prop.cursor.object, ee->prop.cursor.layer);
   evas_object_move(ee->prop.cursor.object,x - ee->prop.cursor.hot.x,y - ee->prop.cursor.hot.y);
   evas_object_pass_events_set(ee->prop.cursor.object, 1);
   if (evas_pointer_inside_get(ee->evas))
     evas_object_show(ee->prop.cursor.object);

   evas_object_event_callback_add(obj, EVAS_CALLBACK_DEL, _ecore_evas_object_cursor_del, ee);
}

static void
_ecore_evas_directfb_fullscreen_set(Ecore_Evas *ee, int on)
{
   Evas_Engine_Info_DirectFB *einfo;
   int w;
   int h;
   int resized = 0;

   if (((ee->prop.fullscreen) && (on)) || ((!ee->prop.fullscreen) && (!on)))
     return;

   if (on)
     ecore_directfb_window_fullscreen_set(ee->engine.directfb.window, 1);
   else
     ecore_directfb_window_fullscreen_set(ee->engine.directfb.window, 0);
   /* set the new size of the evas */
   ecore_directfb_window_size_get(ee->engine.directfb.window, &w, &h);
   if( (ee->w != w) || (ee->h != h))
     {
        resized = 1;
        ee->w = w;
        ee->h = h;
        ee->req.w = ee->w;
        ee->req.h = ee->h;
        if ((ee->rotation == 90) || (ee->rotation == 270))
          {
             evas_output_size_set(ee->evas, ee->h, ee->w);
             evas_output_viewport_set(ee->evas, 0, 0, ee->h, ee->w);
          }
        else
          {
             evas_output_size_set(ee->evas, ee->w, ee->h);
             evas_output_viewport_set(ee->evas, 0, 0, ee->w, ee->h);
          }
     }
   einfo = (Evas_Engine_Info_DirectFB *)evas_engine_info_get(ee->evas);
   if (einfo)
     {
        einfo->info.surface = ee->engine.directfb.window->surface;
        if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
          {
             ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
          }
     }
   ee->prop.fullscreen = on;
   if (resized)
     {
        if(ee->func.fn_resize) ee->func.fn_resize(ee);
     }
}

static void *
_ecore_evas_directfb_window_get(const Ecore_Evas *ee)
{
   return ee->engine.directfb.window;
}
#endif

#ifdef BUILD_ECORE_EVAS_DIRECTFB
static Ecore_Evas_Engine_Func _ecore_directfb_engine_func =
{
   _ecore_evas_directfb_free,        /* free an ecore_evas */
     NULL,                                /* cb resize */
     NULL,                                /* cb move */
     NULL,                                /* cb show */
     NULL,                                /* cb hide */
     NULL,                                /* cb delete request */
     NULL,                                /* cb destroy */
     NULL,                                /* cb focus in */
     NULL,                                /* cb focus out */
     NULL,                              /* cb sticky */
     NULL,                              /* cb unsticky */
     NULL,                                /* cb mouse in */
     NULL,                                /* cb mouse out */
     NULL,                                /* cb pre render */
     NULL,                                /* cb post render */
     _ecore_evas_directfb_move,                /* move */
     NULL,                              /* managed move */
     _ecore_evas_directfb_resize,        /* resize */
     NULL,                                /* move resize */
     NULL,//_ecore_evas_directfb_rotation_set,/* rotation */
     _ecore_evas_directfb_shaped_set,   /* shaped */
     _ecore_evas_directfb_show,                /* show */
     _ecore_evas_directfb_hide,                /* hide */
     NULL,                                /* raise */
     NULL,                                /* lower */
     NULL,                                /* activate */
     NULL,                                /* title set */
     NULL,                                /* name class set */
     NULL,                                /* size min */
     NULL,                                /* size max */
     NULL,                                /* size base */
     NULL,                                /* size step */
     _ecore_evas_directfb_object_cursor_set, /* set cursor to an evas object */
     NULL,                                /* layer set */
     _ecore_evas_directfb_focus_set,        /* focus */
     NULL,                                /* iconified */
     NULL,                                /* borderless */
     NULL,                                /* override */
     NULL,                                /* maximized */
     _ecore_evas_directfb_fullscreen_set,/* fullscreen */
     NULL,                                /* avoid damage */
     NULL,                                /* withdrawn */
     NULL,                                /* sticky */
     NULL,                              /* ignore events */
     NULL,                              /* alpha */
     NULL, //transparent

     NULL, // render
     NULL  // screen_geometry_get
};
#endif

/* api */
/*******/

#ifdef BUILD_ECORE_EVAS_DIRECTFB
EAPI Ecore_Evas *
ecore_evas_directfb_new(const char *disp_name, int windowed, int x, int y, int w, int h)
{
   Evas_Engine_Info_DirectFB *einfo;
   Ecore_Evas *ee;
   Ecore_DirectFB_Window *window;
   int rmethod;

   rmethod = evas_render_method_lookup("directfb");
   if (!rmethod) return NULL;
   if (!ecore_directfb_init(disp_name)) return NULL;
   ee = calloc(1, sizeof(Ecore_Evas));
   if (!ee) return NULL;

   ECORE_MAGIC_SET(ee, ECORE_MAGIC_EVAS);
   _ecore_evas_directfb_init();
   ee->engine.func = (Ecore_Evas_Engine_Func *)&_ecore_directfb_engine_func;

   ee->driver = "directfb";
   if (disp_name) ee->name = strdup(disp_name);

   if (w < 1) w = 1;
   if (h < 1) h = 1;

   ee->rotation = 0;
   ee->visible = 1;
   ee->x = x;
   ee->y = y;
   ee->w = w;
   ee->h = h;
   ee->req.x = ee->x;
   ee->req.y = ee->y;
   ee->req.w = ee->w;
   ee->req.h = ee->h;
   ee->prop.layer = 1;
   ee->prop.fullscreen = 0;

   /* init evas here */
   ee->evas = evas_new();
   evas_data_attach_set(ee->evas, ee);
   evas_output_method_set(ee->evas, rmethod);
   evas_output_size_set(ee->evas, w, h);
   evas_output_viewport_set(ee->evas, 0, 0, w, h);
   einfo = (Evas_Engine_Info_DirectFB *)evas_engine_info_get(ee->evas);

   window = ecore_directfb_window_new(x,y,w,h);
   ee->engine.directfb.window = window;
   if (einfo)
     {
        einfo->info.dfb = ecore_directfb_interface_get();
        einfo->info.surface = window->surface;
        if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
          {
             ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
             ecore_evas_free(ee);
             return NULL;
          }
     }
   else
     {
        ERR("evas_engine_info_set() init engine '%s' failed.", ee->driver);
        ecore_evas_free(ee);
        return NULL;
     }

   ee->engine.func->fn_render = _ecore_evas_directfb_render;
   _ecore_evas_register(ee);

   if (!ecore_evases_hash)
     ecore_evases_hash = eina_hash_string_superfast_new(NULL);
   eina_hash_add(ecore_evases_hash, _ecore_evas_directfb_winid_str_get(ee->engine.directfb.window->id), ee);

   return ee;
}
#else
EAPI Ecore_Evas *
ecore_evas_directfb_new(const char *disp_name __UNUSED__, int windowed __UNUSED__, int x __UNUSED__, int y __UNUSED__, int w __UNUSED__, int h __UNUSED__)
{
   return NULL;
}
#endif

#ifdef BUILD_ECORE_EVAS_DIRECTFB
EAPI Ecore_DirectFB_Window *
ecore_evas_directfb_window_get(const Ecore_Evas *ee)
{
   return (Ecore_DirectFB_Window *) _ecore_evas_directfb_window_get(ee);
}
#else
EAPI Ecore_DirectFB_Window *
ecore_evas_directfb_window_get(const Ecore_Evas *ee __UNUSED__)
{
  return NULL;
}
#endif
