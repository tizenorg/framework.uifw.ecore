#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Ecore.h>
#include <Ecore_Input.h>
#include <Ecore_Input_Evas.h>
#if defined(BUILD_ECORE_EVAS_SOFTWARE_SDL) || defined(BUILD_ECORE_EVAS_OPENGL_SDL)
# include <Ecore_Sdl.h>
# ifdef BUILD_ECORE_EVAS_OPENGL_SDL
#  include <Evas_Engine_GL_SDL.h>
# endif
#endif

#include <stdlib.h>
#include <string.h>

#if defined(BUILD_ECORE_EVAS_SOFTWARE_SDL) || defined(BUILD_ECORE_EVAS_OPENGL_SDL)
#include <SDL/SDL.h>
#endif

#include "ecore_evas_private.h"
#include "Ecore_Evas.h"

/*
 * SDL only handle one window at a time. That's by definition, there is nothing wrong here.
 *
 */
#if defined(BUILD_ECORE_EVAS_SOFTWARE_SDL) || defined(BUILD_ECORE_EVAS_OPENGL_SDL)

/* static char *ecore_evas_default_display = "0"; */
/* static Ecore_List *ecore_evas_input_devices = NULL; */

static int                      _ecore_evas_init_count = 0;

static Ecore_Evas               *sdl_ee = NULL;
static Ecore_Event_Handler      *ecore_evas_event_handlers[4] = {
   NULL, NULL, NULL, NULL
};

static const char               *ecore_evas_sdl_default = "EFL SDL";
static int                      _ecore_evas_fps_debug = 0;
static Ecore_Poller             *ecore_evas_event;

static Ecore_Evas *
_ecore_evas_sdl_match(void)
{
   return sdl_ee;
}

static void *
_ecore_evas_sdl_switch_buffer(void *data, void *dest __UNUSED__)
{
   SDL_Flip(data);
   return ((SDL_Surface*)data)->pixels;
}

static Eina_Bool
_ecore_evas_sdl_event_got_focus(void *data __UNUSED__, int type __UNUSED__, void *event __UNUSED__)
{
   Ecore_Evas                   *ee;

   ee = _ecore_evas_sdl_match();

   if (!ee) return ECORE_CALLBACK_PASS_ON;
   /* pass on event */
   ee->prop.focused = 1;
   evas_focus_in(ee->evas);
   if (ee->func.fn_focus_in) ee->func.fn_focus_in(ee);
   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_ecore_evas_sdl_event_lost_focus(void *data __UNUSED__, int type __UNUSED__, void *event __UNUSED__)
{
   Ecore_Evas                   *ee;

   ee = _ecore_evas_sdl_match();

   if (!ee) return ECORE_CALLBACK_PASS_ON;
   /* pass on event */
   ee->prop.focused = 0;
   evas_focus_out(ee->evas);
   if (ee->func.fn_focus_out) ee->func.fn_focus_out(ee);
   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_ecore_evas_sdl_event_video_resize(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Sdl_Event_Video_Resize *e;
   Ecore_Evas                   *ee;
   int                           rmethod;

   e = event;
   ee = _ecore_evas_sdl_match();

   if (!ee) return ECORE_CALLBACK_PASS_ON; /* pass on event */

   rmethod = evas_output_method_get(ee->evas);
   if (rmethod == evas_render_method_lookup("buffer"))
     {
        Evas_Engine_Info_Buffer *einfo;

        einfo = (Evas_Engine_Info_Buffer *) evas_engine_info_get(ee->evas);
        if (einfo)
          {
             einfo->info.depth_type = EVAS_ENGINE_BUFFER_DEPTH_RGB32;
             einfo->info.switch_data = SDL_SetVideoMode(e->w, e->h, 32,
                                                        (ee->prop.hwsurface ? SDL_HWSURFACE : SDL_SWSURFACE)
                                                        | (ee->prop.fullscreen ? SDL_FULLSCREEN : 0)
                                                        | (ee->alpha ? SDL_SRCALPHA : 0)
                                                        | SDL_DOUBLEBUF);
             if (!einfo->info.switch_data)
               {
                  return EINA_FALSE;
               }

             SDL_SetAlpha(einfo->info.switch_data, SDL_SRCALPHA, 0);
             SDL_FillRect(einfo->info.switch_data, NULL, 0);

             einfo->info.dest_buffer = ((SDL_Surface*)einfo->info.switch_data)->pixels;
             einfo->info.dest_buffer_row_bytes = e->w * sizeof (int);
             einfo->info.use_color_key = 0;
             einfo->info.alpha_threshold = 0;
             einfo->info.func.new_update_region = NULL;
             einfo->info.func.free_update_region = NULL;
             einfo->info.func.switch_buffer = _ecore_evas_sdl_switch_buffer;
             if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *) einfo))
               {
                  return EINA_FALSE;
               }
          }
     }

   ee->w = e->w;
   ee->h = e->h;

   evas_output_size_set(ee->evas, e->w, e->h);
   evas_output_viewport_set(ee->evas, 0, 0, e->w, e->h);

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_ecore_evas_sdl_event_video_expose(void *data __UNUSED__, int type __UNUSED__, void *event __UNUSED__)
{
   Ecore_Evas                   *ee;
   int                          w;
   int                          h;

   ee = _ecore_evas_sdl_match();

   if (!ee) return ECORE_CALLBACK_PASS_ON;
   evas_output_size_get(ee->evas, &w, &h);
   evas_damage_rectangle_add(ee->evas, 0, 0, w, h);

   return ECORE_CALLBACK_PASS_ON;
}

static int
_ecore_evas_render(Ecore_Evas *ee)
{
   Eina_List *updates;

   updates = evas_render_updates(ee->evas);
   if (updates)
     {
        evas_render_updates_free(updates);
        _ecore_evas_idle_timeout_update(ee);
     }
   return updates ? 1 : 0;
}

static int
_ecore_evas_sdl_render(Ecore_Evas *ee)
{
   int rend = 0;
   Eina_List *ll;
   Ecore_Evas *ee2;

   EINA_LIST_FOREACH(ee->sub_ecore_evas, ll, ee2)
     {
        if (ee2->func.fn_pre_render) ee2->func.fn_pre_render(ee2);
        if (ee2->engine.func->fn_render)
          rend |= ee2->engine.func->fn_render(ee2);
        if (ee2->func.fn_post_render) ee2->func.fn_post_render(ee2);
     }

   if (ee->func.fn_pre_render) ee->func.fn_pre_render(ee);

   if (ee->prop.avoid_damage) rend = _ecore_evas_render(ee);
   else if ((ee->visible) ||
            ((ee->should_be_visible) && (ee->prop.fullscreen)) ||
            ((ee->should_be_visible) && (ee->prop.override)))
     rend |= _ecore_evas_render(ee);
   else
     evas_norender(ee->evas);

   if (ee->func.fn_post_render) ee->func.fn_post_render(ee);
   return rend;
}

static Eina_Bool
_ecore_evas_sdl_event(void *data __UNUSED__)
{
   ecore_sdl_feed_events();
   return ECORE_CALLBACK_RENEW;
}

static int
_ecore_evas_sdl_init(int w __UNUSED__, int h __UNUSED__)
{
   _ecore_evas_init_count++;
   if (_ecore_evas_init_count > 1) return _ecore_evas_init_count;

#ifndef _WIN32
   if (getenv("ECORE_EVAS_FPS_DEBUG")) _ecore_evas_fps_debug = 1;
#endif /* _WIN32 */
   // this is pretty bad: poller? and set poll time? pol time is meant to be
   // adjustable for things like polling battery state, or amoutn of spare
   // memory etc.
   //
   ecore_evas_event = ecore_poller_add(ECORE_POLLER_CORE, 1, _ecore_evas_sdl_event, NULL);
   ecore_poller_poll_interval_set(ECORE_POLLER_CORE, 0.006);
#ifndef _WIN32
   if (_ecore_evas_fps_debug) _ecore_evas_fps_debug_init();
#endif /* _WIN32 */

   ecore_event_evas_init();

   ecore_evas_event_handlers[0] = ecore_event_handler_add(ECORE_SDL_EVENT_GOT_FOCUS, _ecore_evas_sdl_event_got_focus, NULL);
   ecore_evas_event_handlers[1] = ecore_event_handler_add(ECORE_SDL_EVENT_LOST_FOCUS, _ecore_evas_sdl_event_lost_focus, NULL);
   ecore_evas_event_handlers[2] = ecore_event_handler_add(ECORE_SDL_EVENT_RESIZE, _ecore_evas_sdl_event_video_resize, NULL);
   ecore_evas_event_handlers[3] = ecore_event_handler_add(ECORE_SDL_EVENT_EXPOSE, _ecore_evas_sdl_event_video_expose, NULL);

   return _ecore_evas_init_count;
}

static int
_ecore_evas_sdl_shutdown(void)
{
   _ecore_evas_init_count--;
   if (_ecore_evas_init_count == 0)
     {
        unsigned int i;

        for (i = 0; i < sizeof (ecore_evas_event_handlers) / sizeof (Ecore_Event_Handler*); i++)
          ecore_event_handler_del(ecore_evas_event_handlers[i]);
        ecore_event_evas_shutdown();
        ecore_poller_del(ecore_evas_event);
        ecore_evas_event = NULL;
#ifndef _WIN32
        if (_ecore_evas_fps_debug) _ecore_evas_fps_debug_shutdown();
#endif /* _WIN32 */
     }
   if (_ecore_evas_init_count < 0) _ecore_evas_init_count = 0;
   return _ecore_evas_init_count;
}

static void
_ecore_evas_sdl_free(Ecore_Evas *ee)
{
   if (sdl_ee == ee) sdl_ee = NULL;

   ecore_event_window_unregister(0);
   _ecore_evas_sdl_shutdown();
   ecore_sdl_shutdown();
}

static void
_ecore_evas_resize(Ecore_Evas *ee, int w, int h)
{
   int rmethod;

   if ((w == ee->w) && (h == ee->h)) return;
   ee->w = w;
   ee->h = h;

   rmethod = evas_output_method_get(ee->evas);
   if (rmethod == evas_render_method_lookup("buffer"))
     {
        Evas_Engine_Info_Buffer *einfo;

        einfo = (Evas_Engine_Info_Buffer *) evas_engine_info_get(ee->evas);
        if (einfo)
          {
             einfo->info.depth_type = EVAS_ENGINE_BUFFER_DEPTH_RGB32;
             einfo->info.switch_data = SDL_SetVideoMode(w, h, 32,
                                                        (ee->prop.hwsurface ? SDL_HWSURFACE : SDL_SWSURFACE)
                                                        | (ee->prop.fullscreen ? SDL_FULLSCREEN : 0)
                                                        | (ee->alpha ? SDL_SRCALPHA : 0)
                                                        | SDL_DOUBLEBUF);
             if (!einfo->info.switch_data)
               {
                  return ;
               }

             SDL_SetAlpha(einfo->info.switch_data, SDL_SRCALPHA, 0);
             SDL_FillRect(einfo->info.switch_data, NULL, 0);

             einfo->info.dest_buffer = ((SDL_Surface*)einfo->info.switch_data)->pixels;
             einfo->info.dest_buffer_row_bytes = w * sizeof (int);
             einfo->info.use_color_key = 0;
             einfo->info.alpha_threshold = 0;
             einfo->info.func.new_update_region = NULL;
             einfo->info.func.free_update_region = NULL;
             einfo->info.func.switch_buffer = _ecore_evas_sdl_switch_buffer;
             if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *) einfo))
               {
                  return ;
               }
          }
     }

   evas_output_size_set(ee->evas, ee->w, ee->h);
   evas_output_viewport_set(ee->evas, 0, 0, ee->w, ee->h);
   evas_damage_rectangle_add(ee->evas, 0, 0, ee->w, ee->h);

   if (ee->func.fn_resize) ee->func.fn_resize(ee);
}

static void
_ecore_evas_move_resize(Ecore_Evas *ee, int x __UNUSED__, int y __UNUSED__, int w, int h)
{
   if ((w == ee->w) && (h == ee->h)) return;
   ee->w = w;
   ee->h = h;

   evas_output_size_set(ee->evas, ee->w, ee->h);
   evas_output_viewport_set(ee->evas, 0, 0, ee->w, ee->h);
   evas_damage_rectangle_add(ee->evas, 0, 0, ee->w, ee->h);

   if (ee->func.fn_resize) ee->func.fn_resize(ee);
}

static void
_ecore_evas_show(Ecore_Evas *ee)
{
   if (ee->prop.focused) return;
   ee->prop.focused = 1;
   evas_event_feed_mouse_in(ee->evas, (unsigned int)((unsigned long long)(ecore_time_get() * 1000.0) & 0xffffffff), NULL);
}

static void
_ecore_evas_object_cursor_del(void *data, Evas *e __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Ecore_Evas *ee;

   ee = data;
   if (ee) ee->prop.cursor.object = NULL;
}

static void
_ecore_evas_object_cursor_set(Ecore_Evas *ee, Evas_Object *obj, int layer, int hot_x, int hot_y)
{
   int x, y;

   if (ee->prop.cursor.object) evas_object_del(ee->prop.cursor.object);

   if (!obj)
     {
        ee->prop.cursor.object = NULL;
        ee->prop.cursor.layer = 0;
        ee->prop.cursor.hot.x = 0;
        ee->prop.cursor.hot.y = 0;
        return;
     }

   ee->prop.cursor.object = obj;
   ee->prop.cursor.layer = layer;
   ee->prop.cursor.hot.x = hot_x;
   ee->prop.cursor.hot.y = hot_y;
   evas_pointer_output_xy_get(ee->evas, &x, &y);
   evas_object_layer_set(ee->prop.cursor.object, ee->prop.cursor.layer);
   evas_object_move(ee->prop.cursor.object,
                    x - ee->prop.cursor.hot.x,
                    y - ee->prop.cursor.hot.y);
   evas_object_pass_events_set(ee->prop.cursor.object, 1);
   if (evas_pointer_inside_get(ee->evas))
     evas_object_show(ee->prop.cursor.object);

   evas_object_event_callback_add(obj, EVAS_CALLBACK_DEL, _ecore_evas_object_cursor_del, ee);
}

static Ecore_Evas_Engine_Func _ecore_sdl_engine_func =
{
   _ecore_evas_sdl_free,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   _ecore_evas_resize,
   _ecore_evas_move_resize,
   NULL,
   NULL,
   _ecore_evas_show,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   _ecore_evas_object_cursor_set,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL, //transparent
   NULL, // profiles_set

   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,

   NULL, // render
   NULL, // screen_geometry_get
   NULL  // screen_dpi_get
};

static Ecore_Evas*
_ecore_evas_internal_sdl_new(int rmethod, const char* name, int w, int h, int fullscreen, int hwsurface, int noframe, int alpha)
{
   Ecore_Evas           *ee;

   if (!name)
     name = ecore_evas_sdl_default;

   ee = calloc(1, sizeof(Ecore_Evas));
   if (!ee) return NULL;

   ECORE_MAGIC_SET(ee, ECORE_MAGIC_EVAS);

   ee->engine.func = (Ecore_Evas_Engine_Func *)&_ecore_sdl_engine_func;

   ee->driver = "sdl";
   if (name) ee->name = strdup(name);

   if (w < 1) w = 1;
   if (h < 1) h = 1;
   ee->visible = 1;
   ee->w = w;
   ee->h = h;

   ee->prop.max.w = 0;
   ee->prop.max.h = 0;
   ee->prop.layer = 0;
   ee->prop.focused = 1;
   ee->prop.borderless = 1;
   ee->prop.override = 1;
   ee->prop.maximized = 1;
   ee->prop.fullscreen = fullscreen;
   ee->prop.withdrawn = 0;
   ee->prop.sticky = 0;
   ee->prop.window = 0;
   ee->alpha = alpha;
   ee->prop.hwsurface = hwsurface;

   /* init evas here */
   ee->evas = evas_new();
   evas_data_attach_set(ee->evas, ee);
   evas_output_method_set(ee->evas, rmethod);

   evas_output_size_set(ee->evas, w, h);
   evas_output_viewport_set(ee->evas, 0, 0, w, h);

   if (rmethod == evas_render_method_lookup("buffer"))
     {
        Evas_Engine_Info_Buffer *einfo;

        einfo = (Evas_Engine_Info_Buffer *) evas_engine_info_get(ee->evas);
        if (einfo)
          {
             SDL_Init(SDL_INIT_NOPARACHUTE);

             if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
               {
                  ERR("SDL_Init failed with %s", SDL_GetError());
                  SDL_Quit();
                  return NULL;
               }

             einfo->info.depth_type = EVAS_ENGINE_BUFFER_DEPTH_RGB32;
             einfo->info.switch_data = SDL_SetVideoMode(w, h, 32,
                                                        (hwsurface ? SDL_HWSURFACE : SDL_SWSURFACE)
                                                        | (fullscreen ? SDL_FULLSCREEN : 0)
                                                        | (noframe ? SDL_NOFRAME : 0)
                                                        | (alpha ? SDL_SRCALPHA : 0)
                                                        | SDL_DOUBLEBUF);
             if (!einfo->info.switch_data)
               {
                  ERR("SDL_SetVideoMode failed !");
                  ecore_evas_free(ee);
                  return NULL;
               }

             SDL_SetAlpha(einfo->info.switch_data, SDL_SRCALPHA, 0);
             SDL_FillRect(einfo->info.switch_data, NULL, 0);

             einfo->info.dest_buffer = ((SDL_Surface*)einfo->info.switch_data)->pixels;
             einfo->info.dest_buffer_row_bytes = w * sizeof (int);
             einfo->info.use_color_key = 0;
             einfo->info.alpha_threshold = 0;
             einfo->info.func.new_update_region = NULL;
             einfo->info.func.free_update_region = NULL;
             einfo->info.func.switch_buffer = _ecore_evas_sdl_switch_buffer;
             if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *) einfo))
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
     }
   else if (rmethod == evas_render_method_lookup("gl_sdl"))
     {
#ifdef BUILD_ECORE_EVAS_OPENGL_SDL
        Evas_Engine_Info_GL_SDL *einfo;

        einfo = (Evas_Engine_Info_GL_SDL *) evas_engine_info_get(ee->evas);
        if (einfo)
          {
             einfo->flags.fullscreen = fullscreen;
             einfo->flags.noframe = noframe;
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
#endif
     }
   else
     {
        ERR("evas_engine_info_set() init engine '%s' failed.", ee->driver);
        ecore_evas_free(ee);
        return NULL;
     }

   if (!ecore_sdl_init(name))
     {
        evas_free(ee->evas);
        if (ee->name) free(ee->name);
        free(ee);
        return NULL;
     }

   _ecore_evas_sdl_init(w, h);

   ecore_event_window_register(0, ee, ee->evas,
                               (Ecore_Event_Mouse_Move_Cb)_ecore_evas_mouse_move_process,
                               (Ecore_Event_Multi_Move_Cb)_ecore_evas_mouse_multi_move_process,
                               (Ecore_Event_Multi_Down_Cb)_ecore_evas_mouse_multi_down_process,
                               (Ecore_Event_Multi_Up_Cb)_ecore_evas_mouse_multi_up_process);

   SDL_ShowCursor(SDL_ENABLE);

   ee->engine.func->fn_render = _ecore_evas_sdl_render;
   _ecore_evas_register(ee);

   sdl_ee = ee;
   return ee;
}
#endif

#ifdef BUILD_ECORE_EVAS_SOFTWARE_SDL
EAPI Ecore_Evas*
ecore_evas_sdl_new(const char* name, int w, int h, int fullscreen, int hwsurface, int noframe, int alpha)
{
   Ecore_Evas          *ee;
   int                  rmethod;

   rmethod = evas_render_method_lookup("buffer");
   if (!rmethod) return NULL;

   ee = _ecore_evas_internal_sdl_new(rmethod, name, w, h, fullscreen, hwsurface, noframe, alpha);
   return ee;
}
#else
EAPI Ecore_Evas*
ecore_evas_sdl_new(const char* name __UNUSED__, int w __UNUSED__, int h __UNUSED__, int fullscreen __UNUSED__, int hwsurface __UNUSED__, int noframe __UNUSED__, int alpha __UNUSED__)
{
   ERR("OUTCH !");
   return NULL;
}
#endif

EAPI Ecore_Evas*
ecore_evas_sdl16_new(const char* name __UNUSED__, int w __UNUSED__, int h __UNUSED__, int fullscreen __UNUSED__, int hwsurface __UNUSED__, int noframe __UNUSED__, int alpha __UNUSED__)
{
   ERR("OUTCH !");
   return NULL;
}

#ifdef BUILD_ECORE_EVAS_OPENGL_SDL
EAPI Ecore_Evas*
ecore_evas_gl_sdl_new(const char* name, int w, int h, int fullscreen, int noframe)
{
   Ecore_Evas          *ee;
   int                  rmethod;

   rmethod = evas_render_method_lookup("gl_sdl");
   if (!rmethod) return NULL;

   ee = _ecore_evas_internal_sdl_new(rmethod, name, w, h, fullscreen, 0, noframe, 0);
   if (ee) ee->driver = "gl_sdl";
   return ee;
}
#else
EAPI Ecore_Evas*
ecore_evas_gl_sdl_new(const char* name __UNUSED__, int w __UNUSED__, int h __UNUSED__, int fullscreen __UNUSED__, int noframe __UNUSED__)
{
   ERR("OUTCH !");
   return NULL;
}
#endif

