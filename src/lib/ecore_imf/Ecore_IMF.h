#ifndef _ECORE_IMF_H
#define _ECORE_IMF_H

#include <Evas.h>
#include <Ecore_IMF_Input_Panel_Key.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_ECORE_IMF_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_ECORE_IMF_BUILD */
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

/* Faked 'bool'.  */
#ifndef __cplusplus
#ifndef bool
#define bool int
#define FALSE 0
#define TRUE 1
#endif
#endif

/**
 * @file Ecore_IMF.h
 * @brief The file that provides the ISF IMCONTROL Structures and APIs
 * @version   1.0
 * @ingroup   Ecore_IMF_Context_IMControl_Group
 */

#ifdef __cplusplus
extern "C" {
#endif

   // will be deleted soon
   /* ecore_imf_context_ise_state_add_listener() flag */
   typedef enum 
     {
	ISE_STATE_EVENT,                /**< ISE STATE Event */
	ISE_MODE_EVENT,                 /**< ISE MODE Event */
	ISE_LANGUAGE_EVENT,             /**< ISE LANGUAGE Event */
	ISE_SHIFT_MODE_EVENT,           /**< ISE SHIFT MODE */
	ISE_PREEDIT_MODE_EVENT,         /**< ISE PREEDIT MODE */
	ISE_COMPLETION_MODE_EVENT,      /**< ISE COMPLETION MODE */
	ISE_CUSTOM_INPUT_MODE_EVENT,    /**< ISE CUSTOM INPUT MODE */

	ISE_PRIVATE_CONTEXT_01,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_02,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_03,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_04,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_05,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_06,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_07,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_08,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_09,  /**< ISE PRIVATE CONTEXT */
	ISE_PRIVATE_CONTEXT_10,  /**< ISE PRIVATE CONTEXT */
	ISE_EVENT_INVALID
     } ISE_EVENT;

   /* ecore_imf_context_input_panel_event_callback_add() flag */
   typedef enum 
     {
	ECORE_IMF_INPUT_PANEL_STATE_EVENT,              /**< Input Panel STATE Event */
	ECORE_IMF_INPUT_PANEL_MODE_EVENT,               /**< Input Panel MODE Event */
	ECORE_IMF_INPUT_PANEL_LANGUAGE_EVENT,           /**< Input Panel LANGUAGE Event */
	ECORE_IMF_INPUT_PANEL_SHIFT_MODE_EVENT,         /**< Input Panel SHIFT MODE */
	ECORE_IMF_INPUT_PANEL_PREEDIT_MODE_EVENT,       /**< Input Panel PREEDIT MODE */
	ECORE_IMF_INPUT_PANEL_COMPLETION_MODE_EVENT,    /**< Input Panel COMPLETION MODE */
	ECORE_IMF_INPUT_PANEL_CUSTOM_INPUT_MODE_EVENT,  /**< Input Panel CUSTOM INPUT MODE */

	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_01,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_02,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_03,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_04,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_05,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_06,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_07,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_08,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_09,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_PRIVATE_CONTEXT_10,       /**< Input Panel PRIVATE CONTEXT */
	ECORE_IMF_INPUT_PANEL_EVENT_INVALID
     } Ecore_IMF_Input_Panel_Event;

   // will be deleted soon. Use Ecore_IMF_Input_Panel_State instead.
   typedef enum 
     {
	ISE_STATE_SHOW,         /**< Show ISE */
	ISE_STATE_SHOW_ALWAYS,  /**< Show ISE always*/
	ISE_STATE_HIDE,         /**< Hide ISE */
	ISE_STATE_INVALID
     } ISE_STATE;

   typedef enum 
     {
	ECORE_IMF_INPUT_PANEL_STATE_SHOW,         /**< Show Input panel */
	ECORE_IMF_INPUT_PANEL_STATE_HIDE,         /**< Hide Input panel */
	ECORE_IMF_INPUT_PANEL_STATE_INVALID
     } Ecore_IMF_Input_Panel_State;

   // will be deleted soon, Use Ecore_IMF_Input_Mode instead.
   typedef enum 
     {
	ISE_MODE_NATIVE,        /**< Follow current locale mode */
	ISE_MODE_ENGLISH,       /**< English mode*/
	ISE_MODE_NUMBER,        /**< Number mode*/
	ISE_MODE_INVALID
     } ISE_MODE;

   typedef enum 
     {
	ECORE_IMF_INPUT_PANEL_LAYOUT_NORMAL,          /**< Default 4x4 layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_NUMBER,          /**< Number layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_EMAIL,           /**< Email layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_URL,             /**< URL layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_PHONENUMBER,     /**< Phone Number layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_IP,              /**< IP layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_MONTH,           /**< Month layout */	
	ECORE_IMF_INPUT_PANEL_LAYOUT_NUMBERONLY,      /**< Number Only layout */
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_1 = 100,  /* Reserved for future use */
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_2,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_3,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_4,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_5,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_6,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_7,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_8,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_9,
	ECORE_IMF_INPUT_PANEL_LAYOUT_CUSTOM_10,	
	ECORE_IMF_INPUT_PANEL_LAYOUT_INVALID
     } Ecore_IMF_Input_Panel_Layout;

   typedef enum
     {
	ECORE_IMF_INPUT_PANEL_LANG_AUTOMATIC,  /**< Automatic */
	ECORE_IMF_INPUT_PANEL_LANG_ALPHABET,   /**< Alphabet */
     } Ecore_IMF_Input_Panel_Lang;

   typedef enum
     {
	ECORE_IMF_KEYBOARD_LANG_NATIVE,    /**< Native */
	ECORE_IMF_KEYBOARD_LANG_ALPHABET,  /**< Alphabet */
     } Ecore_IMF_Keyboard_Lang;

   typedef enum 
     {
	ECORE_IMF_AUTOCORRECTION_DEFAULT,
	ECORE_IMF_AUTOCORRECTION_NO,
	ECORE_IMF_AUTOCORRECTION_YES,
	ECORE_IMF_AUTOCORRECTION_INVALID
     } Ecore_IMF_Autocorrection;

   // will be deleted soon
   typedef struct 
     {
	bool fUseImEffect;  /**< whether use Im effect or not */	
     } ISE_STYLE;

   typedef enum 
     {
	ECORE_IMF_INPUT_PANEL_ORIENT_NONE,
	ECORE_IMF_INPUT_PANEL_ORIENT_90_CW, /* Clockwise */
	ECORE_IMF_INPUT_PANEL_ORIENT_180,
	ECORE_IMF_INPUT_PANEL_ORIENT_90_CCW /* CounterClockwise */
     } Ecore_IMF_Input_Panel_Orient;

   typedef struct 
     {
	int layout_idx;
	int key_idx;
	Eina_Bool disabled;
     } Disable_Key_Item;

   typedef struct 
     {
	int layout_idx;
	int key_idx;
	int type;
	char data[128]; // label or image path
	int key_value;
	char key_string[32];
     } Private_Key_Item;

   /* Events sent by the Input Method */
   typedef struct _Ecore_IMF_Event_Preedit_Start      Ecore_IMF_Event_Preedit_Start;
   typedef struct _Ecore_IMF_Event_Preedit_End        Ecore_IMF_Event_Preedit_End;
   typedef struct _Ecore_IMF_Event_Preedit_Changed    Ecore_IMF_Event_Preedit_Changed;
   typedef struct _Ecore_IMF_Event_Commit             Ecore_IMF_Event_Commit;
   typedef struct _Ecore_IMF_Event_Delete_Surrounding Ecore_IMF_Event_Delete_Surrounding;

   /* Events to filter */
   typedef struct _Ecore_IMF_Event_Mouse_Down         Ecore_IMF_Event_Mouse_Down;
   typedef struct _Ecore_IMF_Event_Mouse_Up           Ecore_IMF_Event_Mouse_Up;
   typedef struct _Ecore_IMF_Event_Mouse_In           Ecore_IMF_Event_Mouse_In;
   typedef struct _Ecore_IMF_Event_Mouse_Out          Ecore_IMF_Event_Mouse_Out;
   typedef struct _Ecore_IMF_Event_Mouse_Move         Ecore_IMF_Event_Mouse_Move;
   typedef struct _Ecore_IMF_Event_Mouse_Wheel        Ecore_IMF_Event_Mouse_Wheel;
   typedef struct _Ecore_IMF_Event_Key_Down           Ecore_IMF_Event_Key_Down;
   typedef struct _Ecore_IMF_Event_Key_Up             Ecore_IMF_Event_Key_Up;
   typedef union  _Ecore_IMF_Event                    Ecore_IMF_Event;

   typedef struct _Ecore_IMF_Context                  Ecore_IMF_Context;                  /**< An Input Method Context */
   typedef struct _Ecore_IMF_Context_Class            Ecore_IMF_Context_Class;            /**< An Input Method Context class */
   typedef struct _Ecore_IMF_Context_Info             Ecore_IMF_Context_Info;             /**< An Input Method Context info */

   EAPI extern int ECORE_IMF_EVENT_PREEDIT_START;
   EAPI extern int ECORE_IMF_EVENT_PREEDIT_END;
   EAPI extern int ECORE_IMF_EVENT_PREEDIT_CHANGED;
   EAPI extern int ECORE_IMF_EVENT_COMMIT;
   EAPI extern int ECORE_IMF_EVENT_DELETE_SURROUNDING;

   typedef enum
     {
	ECORE_IMF_EVENT_MOUSE_DOWN,
	ECORE_IMF_EVENT_MOUSE_UP,
	ECORE_IMF_EVENT_MOUSE_IN,
	ECORE_IMF_EVENT_MOUSE_OUT,
	ECORE_IMF_EVENT_MOUSE_MOVE,
	ECORE_IMF_EVENT_MOUSE_WHEEL,
	ECORE_IMF_EVENT_KEY_DOWN,
	ECORE_IMF_EVENT_KEY_UP
     } Ecore_IMF_Event_Type;

   typedef enum
     {
	ECORE_IMF_KEYBOARD_MODIFIER_NONE  = 0,      /**< No active modifiers */
	ECORE_IMF_KEYBOARD_MODIFIER_CTRL  = 1 << 0, /**< "Control" is pressed */
	ECORE_IMF_KEYBOARD_MODIFIER_ALT   = 1 << 1, /**< "Alt" is pressed */
	ECORE_IMF_KEYBOARD_MODIFIER_SHIFT = 1 << 2, /**< "Shift" is pressed */
	ECORE_IMF_KEYBOARD_MODIFIER_WIN   = 1 << 3  /**< "Win" (between "Ctrl" and "Alt") is pressed */
     } Ecore_IMF_Keyboard_Modifiers;

   typedef enum
     {
	ECORE_IMF_KEYBOARD_LOCK_NONE      = 0,      /**< No locks are active */
	ECORE_IMF_KEYBOARD_LOCK_NUM       = 1 << 0, /**< "Num" lock is active */
	ECORE_IMF_KEYBOARD_LOCK_CAPS      = 1 << 1, /**< "Caps" lock is active */
	ECORE_IMF_KEYBOARD_LOCK_SCROLL    = 1 << 2  /**< "Scroll" lock is active */
     } Ecore_IMF_Keyboard_Locks;

   typedef enum
     {
	ECORE_IMF_MOUSE_NONE              = 0,      /**< A single click */
	ECORE_IMF_MOUSE_DOUBLE_CLICK      = 1 << 0, /**< A double click */
	ECORE_IMF_MOUSE_TRIPLE_CLICK      = 1 << 1  /**< A triple click */
     } Ecore_IMF_Mouse_Flags;

   typedef enum
     {
	ECORE_IMF_INPUT_MODE_ALPHA        = 1 << 0,
	ECORE_IMF_INPUT_MODE_NUMERIC      = 1 << 1,
	ECORE_IMF_INPUT_MODE_SPECIAL      = 1 << 2,
	ECORE_IMF_INPUT_MODE_HEXA         = 1 << 3,
	ECORE_IMF_INPUT_MODE_TELE         = 1 << 4,
	ECORE_IMF_INPUT_MODE_FULL         = (ECORE_IMF_INPUT_MODE_ALPHA | ECORE_IMF_INPUT_MODE_NUMERIC | ECORE_IMF_INPUT_MODE_SPECIAL),
	ECORE_IMF_INPUT_MODE_INVISIBLE    = 1 << 29,
	ECORE_IMF_INPUT_MODE_AUTOCAP      = 1 << 30
     } Ecore_IMF_Input_Mode;

   struct _Ecore_IMF_Event_Preedit_Start
     {
	Ecore_IMF_Context *ctx;
     };

   struct _Ecore_IMF_Event_Preedit_End
     {
	Ecore_IMF_Context *ctx;
     };

   struct _Ecore_IMF_Event_Preedit_Changed
     {
	Ecore_IMF_Context *ctx;
     };

   struct _Ecore_IMF_Event_Commit
     {
	Ecore_IMF_Context *ctx;
	char *str;
     };

   struct _Ecore_IMF_Event_Delete_Surrounding
     {
	Ecore_IMF_Context *ctx;
	int offset;
	int n_chars;
     };

   struct _Ecore_IMF_Event_Mouse_Down
     {
	int button;                             /**< The button which has been pressed */
	struct {
	     int x, y;
	} output;
	struct {
	     int x, y;
	} canvas;
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	Ecore_IMF_Mouse_Flags        flags;     /**< The flags corresponding the mouse click (single, double or triple click) */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   struct _Ecore_IMF_Event_Mouse_Up
     {
	int button;                             /**< The button which has been pressed */
	struct {
	     int x, y;
	} output;
	struct {
	     int x, y;
	} canvas;
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	Ecore_IMF_Mouse_Flags        flags;     /**< The flags corresponding the mouse click (single, double or triple click) */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   struct _Ecore_IMF_Event_Mouse_In
     {
	int buttons;
	struct {
	     int x, y;
	} output;
	struct {
	     int x, y;
	} canvas;
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   struct _Ecore_IMF_Event_Mouse_Out
     {
	int buttons;
	struct {
	     int x, y;
	} output;
	struct {
	     int x, y;
	} canvas;
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   struct _Ecore_IMF_Event_Mouse_Move
     {
	int buttons;
	struct {
	     struct {
		  int x, y;
	     } output;
	     struct {
		  int x, y;
	     } canvas;
	} cur, prev;
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   struct _Ecore_IMF_Event_Mouse_Wheel
     {
	int direction;                         /* 0 = default up/down wheel */
	int z;                                 /* ...,-2,-1 = down, 1,2,... = up */
	struct {
	     int x, y;
	} output;
	struct {
	     int x, y;
	} canvas;
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   struct _Ecore_IMF_Event_Key_Down
     {
	const char                  *keyname;   /**< The string name of the key pressed */
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	const char                  *key;       /**< The logical key : (eg shift+1 == exclamation) */
	const char                  *string;    /**< A UTF8 string if this keystroke has produced a visible string to be ADDED */
	const char                  *compose;   /**< A UTF8 string if this keystroke has modified a string in the middle of being composed - this string replaces the previous one */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
   };

   struct _Ecore_IMF_Event_Key_Up
     {
	const char                  *keyname;   /**< The string name of the key pressed */
	Ecore_IMF_Keyboard_Modifiers modifiers; /**< The keyboard modifiers active when the event has been emitted */
	Ecore_IMF_Keyboard_Locks     locks;     /**< The keyboard locks active when the event has been emitted */
	const char                  *key;       /**< The logical key : (eg shift+1 == exclamation) */
	const char                  *string;    /**< A UTF8 string if this keystroke has produced a visible string to be ADDED */
	const char                  *compose;   /**< A UTF8 string if this keystroke has modified a string in the middle of being composed - this string replaces the previous one */
	unsigned int                 timestamp; /**< The timestamp when the event occured */
     };

   union _Ecore_IMF_Event
     {
	Ecore_IMF_Event_Mouse_Down mouse_down;
	Ecore_IMF_Event_Mouse_Up mouse_up;
	Ecore_IMF_Event_Mouse_In mouse_in;
	Ecore_IMF_Event_Mouse_Out mouse_out;
	Ecore_IMF_Event_Mouse_Move mouse_move;
	Ecore_IMF_Event_Mouse_Wheel mouse_wheel;
	Ecore_IMF_Event_Key_Down key_down;
	Ecore_IMF_Event_Key_Up key_up;
     };

   struct _Ecore_IMF_Context_Class
     {
	void (*add)                 (Ecore_IMF_Context *ctx);
	void (*del)                 (Ecore_IMF_Context *ctx);
	void (*client_window_set)   (Ecore_IMF_Context *ctx, void *window);
	void (*client_canvas_set)   (Ecore_IMF_Context *ctx, void *canvas);
	void (*show)                (Ecore_IMF_Context *ctx);
	void (*hide)                (Ecore_IMF_Context *ctx);
	void (*preedit_string_get)  (Ecore_IMF_Context *ctx, char **str, int *cursor_pos);
	void (*focus_in)            (Ecore_IMF_Context *ctx);
	void (*focus_out)           (Ecore_IMF_Context *ctx);
	void (*reset)               (Ecore_IMF_Context *ctx);
	void (*cursor_position_set) (Ecore_IMF_Context *ctx, int cursor_pos);
	void (*use_preedit_set)     (Ecore_IMF_Context *ctx, int use_preedit);
	void (*input_mode_set)      (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Mode input_mode);
	int  (*filter_event)        (Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event);

	/* Input Panel Control APIs */
//	int (*input_panel_show)	    (Ecore_IMF_Context *ctx);
//	int (*input_panel_hide)	    (Ecore_IMF_Context *ctx);
	int (*control_panel_show)   (Ecore_IMF_Context *ctx);
	int (*control_panel_hide)   (Ecore_IMF_Context *ctx);

	int (*input_panel_layout_set)         (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Layout layout);
	Ecore_IMF_Input_Panel_Layout (*input_panel_layout_get)         (Ecore_IMF_Context *ctx);

	int (*input_panel_language_set)       (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Lang lang);
	Ecore_IMF_Input_Panel_Lang (*input_panel_language_get) (Ecore_IMF_Context *ctx); 

	int (*input_panel_imdata_set)         (Ecore_IMF_Context *ctx, const char* data, int len);
	int (*input_panel_imdata_get)         (Ecore_IMF_Context *ctx, char* data, int *len);

	void (*input_panel_use_effect_set)      (Ecore_IMF_Context *ctx, Eina_Bool use_effect);
	int (*input_panel_orient_set)     (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Orient orientation);
	
	void (*input_panel_move) (Ecore_IMF_Context *ctx, int x, int y);
	int (*input_panel_geometry_get)       (Ecore_IMF_Context *ctx, int *x, int *y, int *w, int *h);
	int (*input_panel_private_key_set)    (Ecore_IMF_Context *ctx, int layout_index, int key_index, const char *img_path, const char* label, const char* value);
	void (*input_panel_key_disabled_set)  (Ecore_IMF_Context *ctx, int layout_index, int key_index, Eina_Bool disabled);
	
	int (*input_panel_reset)              (Ecore_IMF_Context *ctx); /* Same as reset to default property*/
	Ecore_IMF_Input_Panel_State (*input_panel_state_get)          (Ecore_IMF_Context *ctx);

	/* CallBack APIs  */
	void (*input_panel_event_callback_add) (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Event type, void (*pEventCallBackFunc) (void *data, Ecore_IMF_Context *ctx, int value), void *data);
	void (*input_panel_event_callback_del) (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Event type, void (*pEventCallbackFunc) (void *data, Ecore_IMF_Context *ctx, int value));

	/* ISF related APIs */
	int (*ise_get_ise_language)       (Ecore_IMF_Context *ctx, const char* input_panel_name, char*** langlist);
	int (*ise_set_isf_language)       (Ecore_IMF_Context *ctx, const char* lang);
	int (*ise_get_active_isename)     (Ecore_IMF_Context *ctx, char* name);       /**< will be deprecated */
	int (*ise_set_active_ise_by_name) (Ecore_IMF_Context *ctx, const char* name); /**< will be deprecated */
	int (*ise_set_active_ise_by_uuid) (Ecore_IMF_Context *ctx, const char* uuid); /**< will be deprecated */
	int (*ise_get_iselist)            (Ecore_IMF_Context *ctx, char*** iselist);  /**< will be deprecated */
     };

   struct _Ecore_IMF_Context_Info
     {
	const char *id;              /* ID */
	const char *description;     /* Human readable description */
	const char *default_locales; /* Languages for which this context is the default, separated by : */
	const char *canvas_type;     /* The canvas type used by the input method. Eg.: evas */
	int         canvas_required; /* Whether the canvas usage is required for this input method */
     };

   EAPI int                           ecore_imf_init(void);
   EAPI int                           ecore_imf_shutdown(void);

   EAPI void                          ecore_imf_module_register(const Ecore_IMF_Context_Info *info, Ecore_IMF_Context *(*imf_module_create)(void), Ecore_IMF_Context *(*imf_module_exit)(void));

   EAPI Eina_List                    *ecore_imf_context_available_ids_get(void);
   EAPI Eina_List                    *ecore_imf_context_available_ids_by_canvas_type_get(const char *canvas_type);
   EAPI const char                   *ecore_imf_context_default_id_get(void);
   EAPI const char                   *ecore_imf_context_default_id_by_canvas_type_get(const char *canvas_type);
   EAPI const Ecore_IMF_Context_Info *ecore_imf_context_info_by_id_get(const char *id);

   EAPI Ecore_IMF_Context            *ecore_imf_context_add(const char *id);
   EAPI const Ecore_IMF_Context_Info *ecore_imf_context_info_get(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_del(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_client_window_set(Ecore_IMF_Context *ctx, void *window);
   EAPI void*                         ecore_imf_context_client_window_get(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_client_canvas_set(Ecore_IMF_Context *ctx, void *canvas);
   EAPI void*                         ecore_imf_context_client_canvas_get(Ecore_IMF_Context *ctx);
   EINA_DEPRECATED EAPI void          ecore_imf_context_show(Ecore_IMF_Context *ctx);
   EINA_DEPRECATED EAPI void          ecore_imf_context_hide(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_preedit_string_get(Ecore_IMF_Context *ctx, char **str, int *cursor_pos);
   EAPI void                          ecore_imf_context_focus_in(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_focus_out(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_reset(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_cursor_position_set(Ecore_IMF_Context *ctx, int cursor_pos);
   EAPI void                          ecore_imf_context_use_preedit_set(Ecore_IMF_Context *ctx, int use_preedit);
   EAPI void                          ecore_imf_context_retrieve_surrounding_callback_set(Ecore_IMF_Context *ctx, int (*func)(void *data, Ecore_IMF_Context *ctx, char **text, int *cursor_pos), const void *data);
   EINA_DEPRECATED EAPI void          ecore_imf_context_input_mode_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Mode input_mode);
   EINA_DEPRECATED EAPI Ecore_IMF_Input_Mode          ecore_imf_context_input_mode_get(Ecore_IMF_Context *ctx);
   EAPI int                           ecore_imf_context_filter_event(Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event);

   /* plugin specific functions */
   EAPI Ecore_IMF_Context            *ecore_imf_context_new(const Ecore_IMF_Context_Class *ctxc);
   EAPI void                          ecore_imf_context_data_set(Ecore_IMF_Context *ctx, void *data);
   EAPI void                         *ecore_imf_context_data_get(Ecore_IMF_Context *ctx);
   EAPI int                           ecore_imf_context_surrounding_get(Ecore_IMF_Context *ctx, char **text, int *cursor_pos);
   EAPI void                          ecore_imf_context_preedit_start_event_add(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_preedit_end_event_add(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_preedit_changed_event_add(Ecore_IMF_Context *ctx);
   EAPI void                          ecore_imf_context_commit_event_add(Ecore_IMF_Context *ctx, const char *str);
   EAPI void                          ecore_imf_context_delete_surrounding_event_add(Ecore_IMF_Context *ctx, int offset, int n_chars);

   /**
    * Show the current active ISE with given state.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void entry_application(appdata * ad)
    * {
    *     Evas_Object *en;
    *     en = elm_entry_add(ad->win_main);
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     if(imf_context)
    *     {    
    *         // show input panel window
    *         ecore_imf_context_input_panel_show (imf_context);
    *     }
    * }
    *
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_show(Ecore_IMF_Context *ctx);

   /**
    * Hide the current active ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
    * {
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(obj);
    *
    *     if(imf_context)
    *     {    
    *         ecore_imf_context_input_panel_hide (imf_context);      
    *     }
    * }
    *
    * static void entry_application(appdata * ad)
    * {
    *     Evas_Object *en;
    *     en = elm_entry_add(ad->win_main);
    *     evas_object_smart_callback_add(en, "clicked", button_clicked_cb, NULL);
    * }
    *
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_hide(Ecore_IMF_Context *ctx);

   /**
    * Show control panel.
    * @param ctx context used to map real loaded APIs
    * @ingroup Ecore_IMF_Context_IMControl_Group
    *
    * ISE control panel shows ISE list and provides setup of each ISE
    */
   EAPI void ecore_imf_context_control_panel_show (Ecore_IMF_Context *ctx);

   /**
    * Hide control panel.
    * @param ctx context used to map real loaded APIs
    * @ingroup Ecore_IMF_Context_IMControl_Group
    *
    * ISE control panel shows ISE list and provides setup of each ISE
    */
   EAPI void ecore_imf_context_control_panel_hide (Ecore_IMF_Context *ctx);

   /**
    * Set ISE Language before show ISE.
    * @param ctx context used to map real loaded APIs
    * @param lang see Ecore_IMF_Input_Panel_Lang
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * THIS API IS NOT SUPPORTED NOW
    */    
   EAPI void ecore_imf_context_input_panel_language_set (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Lang lang);

   /**
    * Get ISE Language of current active ISE.
    * @param ctx context used to map real loaded APIs
    * @return Ecore_IMF_Input_Panel_Lang
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * THIS API IS NOT SUPPORTED NOW
    */
   EAPI Ecore_IMF_Input_Panel_Lang ecore_imf_context_input_panel_language_get (Ecore_IMF_Context *ctx);

   /**
    * Set keyboard language.
    * @param ctx context used to map real loaded APIs
    * @param lang see Ecore_IMF_Input_Panel_Lang
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * THIS API IS NOT SUPPORTED NOW
    */    
   EAPI void ecore_imf_context_keyboard_language_set (Ecore_IMF_Context *ctx, Ecore_IMF_Keyboard_Lang lang);

   /**
    * Get keyboard language.
    * @param ctx context used to map real loaded APIs
    * @return Ecore_IMF_Input_Panel_Lang
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * THIS API IS NOT SUPPORTED NOW
    */    
   EAPI Ecore_IMF_Keyboard_Lang ecore_imf_context_keyboard_language_get (Ecore_IMF_Context *ctx);
   
   /**
    * Get ISE Language of given ISE. -- Not supported for now --
    * @ingroup Ecore_IMF_Context_IMControl_Group
    */
   EAPI int ecore_imf_context_ise_get_ise_language (Ecore_IMF_Context *ctx, const char* ise_name, char ***langlist);

   /**
    * Set ISF Language. -- Not supported for now --
    * @ingroup Ecore_IMF_Context_IMControl_Group
    */
   EAPI void ecore_imf_context_ise_set_isf_language (Ecore_IMF_Context *ctx, const char* lang);

   /**
    * Set ISE Specific data before show ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param data data pointer
    * @param len data length
    * @ingroup Ecore_IMF_Context_IMControl_Group
    *
    * this API is used by applications to deliver specific data to ISE. 
    * the data format MUST be negotiated by both application and ISE negotiate.
    *
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     char *im_data="application sample imdata";
    *     Evas_Object *en;
    *
    *     en = elm_entry_add(ad->layout_main);
    *     elm_layout_content_set(ad->layout_main, "entry", en);
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_imdata_set (imf_context, im_data, strlen(im_data)+1);
    *     }
    * }
    * @endcode
    */    
   EAPI void ecore_imf_context_input_panel_imdata_set       (Ecore_IMF_Context *ctx, const char * data, int len);

   /**
    * Get ISE Specific data of current active ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param data data pointer
    * @param len data length
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     int len = 256;
    *     char *im_data = (char*) malloc (len);
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(ad->entry);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_imdata_get (imf_context, im_data, &len);
    *     }
    *
    *     free(im_data);
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_imdata_get       (Ecore_IMF_Context *ctx, char *data, int *len);

   /**
    * Set ISE whether animation effect is shown or not.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param use_effect whether animation effect is shown or not
    * @ingroup Ecore_IMF_Context_IMControl_Group
    *
    * this API is used by applications to deliver specific data to ISE. 
    * the data format MUST be negotiated by both application and ISE negotiate.
    *
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     Evas_Object *en;
    *
    *     en = elm_entry_add(ad->layout_main);
    *     elm_layout_content_set(ad->layout_main, "entry", en);
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_use_effect_set (imf_context, EINA_FALSE); // turn off the animation effect when input panel is appeared.
    *     }
    * }
    * @endcode
    */    
   EAPI void ecore_imf_context_input_panel_use_effect_set           (Ecore_IMF_Context *ctx, Eina_Bool use_effect);

   /**
    * Get whether ISE supports animation effect or not when it is shown or hidden.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param use_effect whether animation effect is shown or not
    * @ingroup Ecore_IMF_Context_IMControl_Group
    *
    * this API is used by applications to deliver specific data to ISE. 
    * the data format MUST be negotiated by both application and ISE negotiate.
    *
    * @code
    * #include <Ecore_IMF.h>
    *
    * static Eina_Bool get_effect_info(struct appdata *ad)
    * {
    *     Eina_Bool use_effect = EINA_TRUE;
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(ad->entry);
    *     
    *     if (imf_context)
    *     {    
    *         use_effect = ecore_imf_context_input_panel_use_effect_get (imf_context); 
    *     }
    *
    *     return use_effect;
    * }
    * @endcode
    */    
   EAPI Eina_Bool ecore_imf_context_input_panel_use_effect_get      (Ecore_IMF_Context *ctx);

   /**
    * Get ISE position of current active ISE. 
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param x top-left x co-ordinate of rectangle;
    * @param y top-left y co-ordinate of rectangle;
    * @param w width of rectangle ;
    * @param h height of rectangle;
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * int get_geometry(struct appdata *ad)
    * {
    *    Ecore_IMF_Context *imf_context = NULL;
    *    int x, y, w, h;   
    *    imf_context = elm_entry_imf_context_get (ad->entry);
    *    if (imf_context)
    *    {
    *        ecore_imf_context_input_panel_geometry_get (imf_context, &x, &y, &w, &h);
    *        //here application can get window size rect 
    *        printf ("x=%d \n", x);
    *        printf ("y=%d \n", y);
    *        printf ("width=%d \n", w);
    *        printf ("height=%d \n", h);
    *    }
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_geometry_get  (Ecore_IMF_Context *ctx, int *x, int *y, int *w, int *h);

   /**
    * Set ISE private key before show ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * Since the second parameter of this API requires a layout index, so before using this API application has to set a specific layout and that layout index 
    * should be passed in the 2nd argument of this API(see sample code).
    * @param ctx context used to map real loaded APIs
    * @param layout_index index of layout page to be set
    * @param key_index index of key to be set
    * @param label text label to be appeared on private key
    * @param value value of key, If NULL it will use original value of key
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    *
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     Evas_Object *en;
    *
    *     en = elm_entry_add(ad->layout_main);
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_private_key_set (imf_context, layout, 0, NULL, "www", -1, "www");
    *     }
    * }
    * @endcode
    */    
   EAPI void ecore_imf_context_input_panel_private_key_set  (Ecore_IMF_Context *ctx, int layout_index, int key_index, const char *img_path, const char* label, int key_value, const char* key_string);

   EAPI Eina_List *ecore_imf_context_input_panel_private_key_list_get  (Ecore_IMF_Context *ctx);

   /**
    * Set ISE Layout before show ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param layout see ECORE_IMF_INPUT_PANEL_LAYOUT
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     en = elm_entry_add(ad->layout_main);
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_layout_set (imf_context, ECORE_IMF_INPUT_PANEL_LAYOUT_EMAIL);
    *     }
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_layout_set  (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Layout layout);

   /**
    * Get ISE Layout of current active ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @return layout see ECORE_IMF_INPUT_PANEL_LAYOUT
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void layout_get(struct appdata *ad)
    * {
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(ad->entry);
    *     
    *     if (imf_context)
    *     {    
    *         layout = ecore_imf_context_input_panel_layout_get (imf_context); 
    *     }
    * }
    * @endcode
    */
   EAPI Ecore_IMF_Input_Panel_Layout ecore_imf_context_input_panel_layout_get  (Ecore_IMF_Context *ctx);

   /**
    * Reset ISE Context including its Style. 
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * This is special function that should be called before calling any ecore_imf_context_set*** APIs to restore all default properties of ISE.
    * @ingroup Ecore_IMF_Context_IMControl_Group
    *
    * NOTE: This API MUST be called before calling ecore_imf_context_input_panel_show().
    */
   EAPI void ecore_imf_context_input_panel_reset       (Ecore_IMF_Context *ctx);        /* Same as reset to default property*/

   /**
    * Set ISE screen orientation.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * To show the keypad in landscape mode application should first call this API with 2nd parameter as 90 or 270.
    * After then only application should call ecore_imf_context_input_panel_show() function.
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     en = elm_entry_add(ad->layout_main);
    *     elm_layout_content_set(ad->layout_main, "entry", en);
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *	  int degree = 90; // the degree value can be either 90/270 for landscape mode and normal portrait mode 0/360.
    *	    
    *     if (imf_context)
    *     {    
    *         // the orient value can be 
    *         // ECORE_IMF_INPUT_PANEL_ORIENT_NONE,
    *         // ECORE_IMF_INPUT_PANEL_ORIENT_90_CW // Clockwise
    *         // ECORE_IMF_INPUT_PANEL_ORIENT_180, 
    *         // ECORE_IMF_INPUT_PANEL_ORIENT_90_CCW // CounterClockwise
    *         ecore_imf_context_input_panel_orient_set(imf_context, ECORE_IMF_INPUT_PANEL_ORIENT_90_CW); 
    *     }
    * }
    */
   EAPI void ecore_imf_context_input_panel_orient_set (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Orient  orientation);

   /**
    * Get Input panel orientation.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * To show the keypad in landscape mode application should first call this API with 2nd parameter as 90 or 270.
    * After then only application should call ecore_imf_context_input_panel_show() function.
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>

    * void get_orient (struct appdata *ad)
    * {
    *     Ecore_IMF_Input_Panel_Orient orient;
    *     ad->entry = elm_entry_add(ad->layout_main);
    *     elm_entry_input_panel_layout_set(ad->entry, ELM_INPUT_PANEL_LAYOUT_URL);
    *     imf_context = elm_entry_imf_context_get (ad->entry);
    *     if (imf_context)
    *     {
    *         orient = ecore_imf_context_input_panel_orient_get (imf_context);
    *     }
    * 
    *     switch (orient)
    *     {
    *     case ECORE_IMF_INPUT_PANEL_ORIENT_NONE:
    *         printf("0 degree\n");
    *         break;
    *     case ECORE_IMF_INPUT_PANEL_ORIENT_90_CW:
    *         printf("90 degree clockwise\n");
    *         break;
    *     case ECORE_IMF_INPUT_PANEL_ORIENT_180:
    *         printf("90 degree clockwise\n");
    *         break;
    *     case ECORE_IMF_INPUT_PANEL_ORIENT_90_CCW:
    *         printf("90 degree counter-clockwise\n");
    *         break;
    *     }
    * }
    */
    
   EAPI Ecore_IMF_Input_Panel_Orient ecore_imf_context_input_panel_orient_get (Ecore_IMF_Context *ctx);   

   /**
    * Get name of current active ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param name ISE name to be got
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     int len = 256;
    *     char *isename = (char*) malloc (len);
    *     memset(isename, '\0', sizeof(isename));
    *
    *     Ecore_IMF_Context *imf_context = NULL;
    *     imf_context = elm_entry_imf_context_get(obj);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_ise_get_active_isename (imf_context, isename);
    *         printf("get isename=[%s]\n", isename);
    *     }
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_ise_get_active_isename     (Ecore_IMF_Context *ctx, char* name);

   /**
    * Set ISE by its name.
    * To use this API application should include Ecore_IMF.h header file.
    * Before calling this API, every application should call ecore_imf_init() to initialize the ecore_imf shared library.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param name ISE name to be set
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     ecore_imf_init();
    *     
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(obj);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_ise_set_active_ise_by_name (imf_context, "Input Pad");
    *     }
    *
    *     //do imf related things
    * 
    *     ecore_imf_shutdown();
    * 
    *     //do imf irrelated things
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_ise_set_active_ise_by_name (Ecore_IMF_Context *ctx, const char* name);

   /**
    * Set ISE by its uuid.
    * To use this API application should include Ecore_IMF.h header file.
    * Before calling this API, every application should call ecore_imf_init() to initialize the ecore_imf shared library.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param uuid ISE uuid to be set
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(obj);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_ise_set_active_ise_by_uuid (imf_context, "ff110940-b8f0-4062-9ff6-a84f4f3575c0");
    *         ecore_imf_context_input_panel_show (imf_context);
    *     }
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_ise_set_active_ise_by_uuid (Ecore_IMF_Context *ctx, const char* uuid);

   /**
    * Get list of ISEs.
    * To use this API application should include Ecore_IMF.h header file.
    * Before calling this API, every application should call ecore_imf_init() to initialize the ecore_imf shared library.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param iselist pointer to the list to be got.
    * @return  int ise counter of iselist
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     ecore_imf_init();
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(obj);
    *     
    *     if (imf_context)
    *     {    
    *         char** iselist;
    *         int count,i;
    *         count = ecore_imf_context_input_panel_get_iselist(imf_context, &iselist);
    *         printf("get_iselist : count[%d] ", count);
    *         for (i=0;i<count;i++)
    *             printf("[%d:%s] ", i, iselist[i]);
    *     }
    *
    *     //do imf related things
    * 
    *     ecore_imf_shutdown();
    * 
    *     //do imf irrelated things
    * }
    * @endcode
    */    
   EAPI int ecore_imf_context_ise_get_iselist (Ecore_IMF_Context *ctx, char*** iselist);

   /**
    * Get state of current active ISE.
    * To use this API application should include Ecore_IMF.h header file.
    * Before calling this API, every application should call ecore_imf_init() to initialize the ecore_imf shared library.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs
    * @param state see ISE_STATE
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void input_panel_state_get(struct appdata *ad)
    * {
    *     Ecore_IMF_Input_Panel_State state;
    *
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(ad->entry);
    *     
    *     if (imf_context)
    *     {    
    *         state = ecore_imf_context_input_panel_state_get (imf_context); 
    *         //here u can see what the current state is
    *         printf("the current state of ISE is %d", state);
    *     }
    * }
    * @endcode
    */
   EAPI Ecore_IMF_Input_Panel_State ecore_imf_context_input_panel_state_get          (Ecore_IMF_Context *ctx);
   
   /**
    * Application can register a callback function which will be called if there is change in ise state,language,mode etc. 
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * @param ctx context used to map real loaded APIs.
    * @param pEventCallback the callback function to be called 
    * @param data application-ISE specific data.
    * @return an integer unique to callabck registered.
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * void _input_panel_event_callback(void *data, Ecore_IMF_Context *ctx, int value)
    * {
    *     if(value == ECORE_IMF_INPUT_PANEL_STATE_SHOW) {
    *         // ISE state has changed to ECORE_IMF_INPUT_PANEL_STATE_SHOW status
    *     } else if(value == ECORE_IMF_INPUT_PANEL_STATE_HIDE) {
    *         // ISE state has changed to ECORE_IMF_INPUT_PANEL_STATE_HIDE status
    *     }
    *     printf("value: %d\n", value);
    * }
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     Evas_Object *en;
    *     en = elm_entry_add(ad->layout_main);
    *     elm_layout_content_set(ad->layout_main, "entry", en);
    * 
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_event_callback_add (imf_context, ECORE_IMF_INPUT_PANEL_STATE_EVENT, _input_panel_event_callback, data);
    *     }
    * }
    * @endcode
    * In order to deregister the callback function registered application should follow the below step.
    * ecore_imf_context_input_panel_event_callback_del (imf_context, ECORE_IMF_INPUT_PANEL_STATE_EVENT, _ise_event_callback);
    */
   EAPI void ecore_imf_context_input_panel_event_callback_add (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Event type, void (*pEventCallbackFunc) (void *data, Ecore_IMF_Context *ctx, int value), const void *data);

   EAPI void ecore_imf_context_input_panel_event_callback_del (Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Event type, void (*pEventCallbackFunc) (void *data, Ecore_IMF_Context *ctx, int value));

   /**
    * Set the key to be disabled.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * Since the second parameter of this API requires a layout index, so before using this API application has to set a specific layout and that layout index 
    * should be passed in the 2nd argument of this API(see sample code).
    * @param ctx context used to map real loaded APIs
    * @param layout_index index of layout page to be set
    * @param key_index index of key to be set
    * @param disabled The state
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     Evas_Object *en;
    *     en = elm_entry_add(ad->layout_main);
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(en);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_key_disabled_set (imf_context, layout, ECORE_IMF_INPUT_PANEL_KEY_SPACE, EINA_TRUE);
    *     }
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_key_disabled_set  (Ecore_IMF_Context *ctx, int layout_index, int key_index, Eina_Bool disabled);

   EAPI Eina_List *ecore_imf_context_input_panel_key_disabled_list_get  (Ecore_IMF_Context *ctx);

   /**
    * Move the soft keyboard to the new position.
    * To use this API application should include Ecore_IMF.h header file.
    * The first parameter i.e Ecore_IMF_Context *ctx is used to map the ecore_imf API into real implemented API which is inside ISF.
    * Since the second parameter of this API requires a layout index, so before using this API application has to set a specific layout and that layout index 
    * should be passed in the 2nd argument of this API(see sample code).
    * @param ctx context used to map real loaded APIs
    * @param x X position to move the virtual keyboard to
    * @param y Y position to move the virtual keyboard to
    * @ingroup Ecore_IMF_Context_IMControl_Group
    * @code
    * #include <Ecore_IMF.h>
    *
    * static void create_entry(struct appdata *ad)
    * {
    *     Ecore_IMF_Context *imf_context = elm_entry_imf_context_get(obj);
    *     
    *     if (imf_context)
    *     {    
    *         ecore_imf_context_input_panel_move (imf_context, 0, 400);
    *     }
    * }
    * @endcode
    */
   EAPI void ecore_imf_context_input_panel_move  (Ecore_IMF_Context *ctx, int x, int y);

   /* The following entry points must be exported by each input method module
    */

   /*
    * int                imf_module_init   (const Ecore_IMF_Context_Info **info);
    * void               imf_module_exit   (void);
    * Ecore_IMF_Context *imf_module_create (void);
    */

#ifdef __cplusplus
}
#endif

#endif

