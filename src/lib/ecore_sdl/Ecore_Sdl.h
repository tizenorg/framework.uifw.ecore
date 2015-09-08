#ifndef _ECORE_SDL_H
#define _ECORE_SDL_H

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_ECORE_SDL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_ECORE_SDL_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */

/**
 * @internal
 * @file
 * @brief Ecore SDL system functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

EAPI extern int ECORE_SDL_EVENT_GOT_FOCUS;
EAPI extern int ECORE_SDL_EVENT_LOST_FOCUS;
EAPI extern int ECORE_SDL_EVENT_RESIZE;
EAPI extern int ECORE_SDL_EVENT_EXPOSE;

typedef struct _Ecore_Sdl_Event_Key_Down Ecore_Sdl_Event_Key_Down;
struct _Ecore_Sdl_Event_Key_Down /** SDL Key Down event */
{
   const char      *keyname; /**< The name of the key that is pressed */
   const char      *keycompose; /**< The UTF-8 string conversion, if any */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Key_Up Ecore_Sdl_Event_Key_Up;
struct _Ecore_Sdl_Event_Key_Up /** SDL Key Up event */
{
   const char      *keyname; /**< The name of the key that is released */
   const char      *keycompose; /**< The UTF-8 string conversion, if any */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Button_Down Ecore_Sdl_Event_Mouse_Button_Down;
struct _Ecore_Sdl_Event_Mouse_Button_Down /** SDL Mouse Down event */
{
   int             button; /**< Mouse button that is pressed (1 - 32) */
   int             x; /**< Mouse co-ordinates when the mouse button is pressed */
   int             y; /**< Mouse co-ordinates when the mouse button is pressed */
   int             double_click : 1; /**< Set if click is a double click */
   int             triple_click : 1; /**< Set if click is a triple click  */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Button_Up Ecore_Sdl_Event_Mouse_Button_Up;
struct _Ecore_Sdl_Event_Mouse_Button_Up /** SDL Mouse Up event */
{
   int             button; /**< Mouse button that is released (1 - 32) */
   int             x; /**< Mouse co-ordinates when the mouse button is raised */
   int             y; /**< Mouse co-ordinates when the mouse button is raised */
   int             double_click : 1; /**< Set if click is a double click */
   int             triple_click : 1; /**< Set if click is a triple click  */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Move Ecore_Sdl_Event_Mouse_Move;
struct _Ecore_Sdl_Event_Mouse_Move /** SDL Mouse Move event */ 
{
   int             x; /**< Mouse co-ordinates of the location where the mouse cursor moved to */
   int             y; /**< Mouse co-ordinates of the location where the mouse cursor moved to */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Wheel Ecore_Sdl_Event_Mouse_Wheel;
struct _Ecore_Sdl_Event_Mouse_Wheel /** SDL Mouse Wheel event */
{
   int             x,y;
   int             direction; /**< @c 0 = vertical, @c 1 = horizontal */
   int             wheel; /**< values are @c 1 (left/up), @c -1 (right/down) */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Video_Resize Ecore_Sdl_Event_Video_Resize;
struct _Ecore_Sdl_Event_Video_Resize
{
   int             w;
   int             h;
};

EAPI int        ecore_sdl_init(const char *name);
EAPI int        ecore_sdl_shutdown(void);
EAPI void       ecore_sdl_feed_events(void);

#ifdef __cplusplus
}
#endif

#endif
