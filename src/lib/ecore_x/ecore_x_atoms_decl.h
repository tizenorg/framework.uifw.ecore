/* generic atoms */
EAPI Ecore_X_Atom ECORE_X_ATOM_ATOM = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_CARDINAL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_COMPOUND_TEXT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_FILE_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_STRING = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_TEXT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_UTF8_STRING = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WINDOW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_PIXMAP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_VISUALID = 0;

/* dnd atoms */
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_XDND = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_XDND = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_AWARE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ENTER = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_TYPE_LIST = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_POSITION = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_COPY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_MOVE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_PRIVATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_ASK = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_LIST = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_LINK = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_DESCRIPTION = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_PROXY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_STATUS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_LEAVE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_DROP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_XDND_FINISHED = 0;

/* dnd atoms that need to be exposed to the application interface */
EAPI Ecore_X_Atom ECORE_X_DND_ACTION_COPY = 0;
EAPI Ecore_X_Atom ECORE_X_DND_ACTION_MOVE = 0;
EAPI Ecore_X_Atom ECORE_X_DND_ACTION_LINK = 0;
EAPI Ecore_X_Atom ECORE_X_DND_ACTION_ASK = 0;
EAPI Ecore_X_Atom ECORE_X_DND_ACTION_PRIVATE = 0;

/* old E atom */
EAPI Ecore_X_Atom ECORE_X_ATOM_E_FRAME_SIZE = 0;

/* old Gnome atom */
EAPI Ecore_X_Atom ECORE_X_ATOM_WIN_LAYER = 0;

/* ICCCM atoms */

/* ICCCM: client properties */
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_ICON_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_NORMAL_HINTS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_SIZE_HINTS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_HINTS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_CLASS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_TRANSIENT_FOR = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_PROTOCOLS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_COLORMAP_WINDOWS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_COMMAND = 0; /* obsolete */
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_CLIENT_MACHINE = 0; /* obsolete */

/* ICCCM: window manager properties */
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_ICON_SIZE = 0;

/* ICCCM: WM_STATE  property */
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_CHANGE_STATE = 0;

/* ICCCM: WM_PROTOCOLS properties */
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_TAKE_FOCUS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_SAVE_YOURSELF = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_DELETE_WINDOW = 0;

/* ICCCM: WM_COLORMAP properties */
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_COLORMAP_NOTIFY = 0;

/* ICCCM: session management properties */
EAPI Ecore_X_Atom ECORE_X_ATOM_SM_CLIENT_ID = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_CLIENT_LEADER = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_WM_WINDOW_ROLE = 0;

/* Motif WM atom */
EAPI Ecore_X_Atom ECORE_X_ATOM_MOTIF_WM_HINTS = 0;

/* NetWM 1.3 atoms (http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html) */

/*
 * NetWM: Root Window Properties and related messages (complete)
 */

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_SUPPORTED = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_CLIENT_LIST = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_CLIENT_LIST_STACKING = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_NUMBER_OF_DESKTOPS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_GEOMETRY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_VIEWPORT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_CURRENT_DESKTOP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_NAMES = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_ACTIVE_WINDOW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WORKAREA = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_SUPPORTING_WM_CHECK = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_VIRTUAL_ROOTS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_LAYOUT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_SHOWING_DESKTOP = 0;

/*
 * NetWM: Other Root Window Messages (complete)
 */

/* pager */
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_CLOSE_WINDOW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_MOVERESIZE_WINDOW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_MOVERESIZE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_RESTACK_WINDOW = 0;

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_REQUEST_FRAME_EXTENTS = 0;

/*
 * NetWM: Application Window Properties (complete)
 */

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_VISIBLE_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_VISIBLE_ICON_NAME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_DESKTOP = 0;

/* window type */
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DESKTOP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DOCK = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLBAR = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_MENU = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_UTILITY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_SPLASH = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DIALOG = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NORMAL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DROPDOWN_MENU = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_POPUP_MENU = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLTIP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NOTIFICATION = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_COMBO = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DND = 0;

/* state */
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MODAL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_STICKY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_VERT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_HORZ = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SHADED = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SKIP_TASKBAR = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SKIP_PAGER = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_HIDDEN = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_FULLSCREEN = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_ABOVE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_BELOW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_DEMANDS_ATTENTION = 0;

/* allowed actions */
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ALLOWED_ACTIONS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MOVE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_RESIZE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MINIMIZE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_SHADE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_STICK = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_HORZ = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_VERT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_FULLSCREEN = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_CHANGE_DESKTOP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_CLOSE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_ABOVE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_BELOW = 0;

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STRUT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_STRUT_PARTIAL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON_GEOMETRY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_PID = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_HANDLED_ICONS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_USER_TIME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_ID = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_FRAME_EXTENTS = 0;

/*
 * NetWM: Window Manager Protocols (complete)
 */

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_PING = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_SYNC_REQUEST = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_SYNC_REQUEST_COUNTER = 0;

/*
 * NetWM: Not in the spec
 */

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_OPACITY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_SHADOW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_SHADE = 0;

/*
 * Startup Notification (http://standards.freedesktop.org/startup-notification-spec/startup-notification-0.1.txt)
 */

EAPI Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_INFO_BEGIN = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_INFO = 0;

/* selection atoms */
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_TARGETS = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_PRIMARY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_SECONDARY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_CLIPBOARD = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_PRIMARY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_SECONDARY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_CLIPBOARD = 0;

/* currently E specific virtual keyboard extension, aim to submit to netwm spec
 * later */

EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ON = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_OFF = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ALPHA = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_NUMERIC = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PIN = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PHONE_NUMBER = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HEX = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_TERMINAL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PASSWORD = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_IP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HOST = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_FILE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_URL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_KEYPAD = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_J2ME = 0;

/* currently E specific illume extension */
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ZONE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ZONE_LIST = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CONFORMANT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_SINGLE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_DUAL_TOP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_DUAL_LEFT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_BACK = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_FORWARD = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_HOME = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLOSE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_HOME_NEW = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_HOME_DEL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_LOCKED = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_START = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_END = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_GEOMETRY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SOFTKEY_GEOMETRY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_KEYBOARD_GEOMETRY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE_TOGGLE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ON = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_OFF = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MAJOR = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MINOR = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ZONE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_POSITION_UPDATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_ON = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OFF = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OPACITY_MODE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OPAQUE= 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSLUCENT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSPARENT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_AVAILABLE_ANGLE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_ANGLE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_ROOT_ANGLE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_GEOMETRY = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_ON = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_OFF = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_NORMAL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_FLOATING = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_NEXT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_PREV = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_ACTIVATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_NEXT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_PREV = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_UP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_DOWN = 0;

EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_COUNTER = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_DRAW_DONE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_SUPPORTED = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_BEGIN = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_END = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_CANCEL = 0;

EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_FLUSH = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_DUMP = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_COMP_PIXMAP = 0;

/* currently Emotion and E17 specific extension */
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIDEO_PARENT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_VIDEO_POSITION = 0;

/* for sliding window */
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_STATE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_GEOMETRY = 0;

/* for SDB(Samsung Debug Bridge) */
EAPI Ecore_X_Atom ECORE_X_ATOM_SDB_SERVER_CONNECT = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_SDB_SERVER_DISCONNECT = 0;

/* currently elementary and E specific extension */
EAPI Ecore_X_Atom ECORE_X_ATOM_E_PROFILE = 0;
EAPI Ecore_X_Atom ECORE_X_ATOM_E_PROFILE_LIST = 0;

typedef struct _Atom_Item Atom_Item;

struct _Atom_Item
{
   const char   *name;
   Ecore_X_Atom *atom;
};

const Atom_Item atom_items[] =
{
   { "ATOM", &ECORE_X_ATOM_ATOM },
   { "CARDINAL", &ECORE_X_ATOM_CARDINAL },
   { "COMPOUND_TEXT", &ECORE_X_ATOM_COMPOUND_TEXT },
   { "FILE_NAME", &ECORE_X_ATOM_FILE_NAME },
   { "STRING", &ECORE_X_ATOM_STRING },
   { "TEXT", &ECORE_X_ATOM_TEXT },
   { "UTF8_STRING", &ECORE_X_ATOM_UTF8_STRING },
   { "WINDOW", &ECORE_X_ATOM_WINDOW },
   { "PIXMAP", &ECORE_X_ATOM_PIXMAP },
   { "VISUALID", &ECORE_X_ATOM_VISUALID },

   { "JXSelectionWindowProperty", &ECORE_X_ATOM_SELECTION_PROP_XDND },
   { "XdndSelection", &ECORE_X_ATOM_SELECTION_XDND },
   { "XdndAware", &ECORE_X_ATOM_XDND_AWARE },
   { "XdndEnter", &ECORE_X_ATOM_XDND_ENTER },
   { "XdndTypeList", &ECORE_X_ATOM_XDND_TYPE_LIST },
   { "XdndPosition", &ECORE_X_ATOM_XDND_POSITION },
   { "XdndActionCopy", &ECORE_X_ATOM_XDND_ACTION_COPY },
   { "XdndActionMove", &ECORE_X_ATOM_XDND_ACTION_MOVE },
   { "XdndActionPrivate", &ECORE_X_ATOM_XDND_ACTION_PRIVATE },
   { "XdndActionAsk", &ECORE_X_ATOM_XDND_ACTION_ASK },
   { "XdndActionList", &ECORE_X_ATOM_XDND_ACTION_LIST },
   { "XdndActionLink", &ECORE_X_ATOM_XDND_ACTION_LINK },
   { "XdndActionDescription", &ECORE_X_ATOM_XDND_ACTION_DESCRIPTION },
   { "XdndProxy", &ECORE_X_ATOM_XDND_PROXY },
   { "XdndStatus", &ECORE_X_ATOM_XDND_STATUS },
   { "XdndLeave", &ECORE_X_ATOM_XDND_LEAVE },
   { "XdndDrop", &ECORE_X_ATOM_XDND_DROP },
   { "XdndFinished", &ECORE_X_ATOM_XDND_FINISHED },

   { "XdndActionCopy", &ECORE_X_DND_ACTION_COPY },
   { "XdndActionMove", &ECORE_X_DND_ACTION_MOVE },
   { "XdndActionLink", &ECORE_X_DND_ACTION_LINK },
   { "XdndActionAsk", &ECORE_X_DND_ACTION_ASK },
   { "XdndActionPrivate", &ECORE_X_DND_ACTION_PRIVATE },

   { "_E_FRAME_SIZE", &ECORE_X_ATOM_E_FRAME_SIZE },

   { "_WIN_LAYER", &ECORE_X_ATOM_WIN_LAYER },

   { "WM_NAME", &ECORE_X_ATOM_WM_NAME },
   { "WM_ICON_NAME", &ECORE_X_ATOM_WM_ICON_NAME },
   { "WM_NORMAL_HINTS", &ECORE_X_ATOM_WM_NORMAL_HINTS },
   { "WM_SIZE_HINTS", &ECORE_X_ATOM_WM_SIZE_HINTS },
   { "WM_HINTS", &ECORE_X_ATOM_WM_HINTS },
   { "WM_CLASS", &ECORE_X_ATOM_WM_CLASS },
   { "WM_TRANSIENT_FOR", &ECORE_X_ATOM_WM_TRANSIENT_FOR },
   { "WM_PROTOCOLS", &ECORE_X_ATOM_WM_PROTOCOLS },
   { "WM_COLORMAP_WINDOWS", &ECORE_X_ATOM_WM_COLORMAP_WINDOWS },
   { "WM_COMMAND", &ECORE_X_ATOM_WM_COMMAND },
   { "WM_CLIENT_MACHINE", &ECORE_X_ATOM_WM_CLIENT_MACHINE },

   { "WM_STATE", &ECORE_X_ATOM_WM_STATE },
   { "WM_ICON_SIZE", &ECORE_X_ATOM_WM_ICON_SIZE },

   { "WM_CHANGE_STATE", &ECORE_X_ATOM_WM_CHANGE_STATE },

   { "WM_TAKE_FOCUS", &ECORE_X_ATOM_WM_TAKE_FOCUS },
   { "WM_SAVE_YOURSELF", &ECORE_X_ATOM_WM_SAVE_YOURSELF },
   { "WM_DELETE_WINDOW", &ECORE_X_ATOM_WM_DELETE_WINDOW },

   { "WM_COLORMAP_NOTIFY", &ECORE_X_ATOM_WM_COLORMAP_NOTIFY },

   { "SM_CLIENT_ID", &ECORE_X_ATOM_SM_CLIENT_ID },
   { "WM_CLIENT_LEADER", &ECORE_X_ATOM_WM_CLIENT_LEADER },
   { "WM_WINDOW_ROLE", &ECORE_X_ATOM_WM_WINDOW_ROLE },

   { "_MOTIF_WM_HINTS", &ECORE_X_ATOM_MOTIF_WM_HINTS },

   { "_NET_SUPPORTED", &ECORE_X_ATOM_NET_SUPPORTED },
   { "_NET_CLIENT_LIST", &ECORE_X_ATOM_NET_CLIENT_LIST },
   { "_NET_CLIENT_LIST_STACKING", &ECORE_X_ATOM_NET_CLIENT_LIST_STACKING },
   { "_NET_NUMBER_OF_DESKTOPS", &ECORE_X_ATOM_NET_NUMBER_OF_DESKTOPS },
   { "_NET_DESKTOP_GEOMETRY", &ECORE_X_ATOM_NET_DESKTOP_GEOMETRY },
   { "_NET_DESKTOP_VIEWPORT", &ECORE_X_ATOM_NET_DESKTOP_VIEWPORT },
   { "_NET_CURRENT_DESKTOP", &ECORE_X_ATOM_NET_CURRENT_DESKTOP },
   { "_NET_DESKTOP_NAMES", &ECORE_X_ATOM_NET_DESKTOP_NAMES },
   { "_NET_ACTIVE_WINDOW", &ECORE_X_ATOM_NET_ACTIVE_WINDOW },
   { "_NET_WORKAREA", &ECORE_X_ATOM_NET_WORKAREA },
   { "_NET_SUPPORTING_WM_CHECK", &ECORE_X_ATOM_NET_SUPPORTING_WM_CHECK },
   { "_NET_VIRTUAL_ROOTS", &ECORE_X_ATOM_NET_VIRTUAL_ROOTS },
   { "_NET_DESKTOP_LAYOUT", &ECORE_X_ATOM_NET_DESKTOP_LAYOUT },
   { "_NET_SHOWING_DESKTOP", &ECORE_X_ATOM_NET_SHOWING_DESKTOP },

   { "_NET_CLOSE_WINDOW", &ECORE_X_ATOM_NET_CLOSE_WINDOW },
   { "_NET_MOVERESIZE_WINDOW", &ECORE_X_ATOM_NET_MOVERESIZE_WINDOW },
   { "_NET_WM_MOVERESIZE", &ECORE_X_ATOM_NET_WM_MOVERESIZE },
   { "_NET_RESTACK_WINDOW", &ECORE_X_ATOM_NET_RESTACK_WINDOW },

   { "_NET_REQUEST_FRAME_EXTENTS", &ECORE_X_ATOM_NET_REQUEST_FRAME_EXTENTS },

   { "_NET_WM_NAME", &ECORE_X_ATOM_NET_WM_NAME },
   { "_NET_WM_VISIBLE_NAME", &ECORE_X_ATOM_NET_WM_VISIBLE_NAME },
   { "_NET_WM_ICON_NAME", &ECORE_X_ATOM_NET_WM_ICON_NAME },
   { "_NET_WM_VISIBLE_ICON_NAME", &ECORE_X_ATOM_NET_WM_VISIBLE_ICON_NAME },
   { "_NET_WM_DESKTOP", &ECORE_X_ATOM_NET_WM_DESKTOP },

   { "_NET_WM_WINDOW_TYPE", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE },
   { "_NET_WM_WINDOW_TYPE_DESKTOP", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DESKTOP },
   { "_NET_WM_WINDOW_TYPE_DOCK", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DOCK },
   { "_NET_WM_WINDOW_TYPE_TOOLBAR", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLBAR },
   { "_NET_WM_WINDOW_TYPE_MENU", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_MENU },
   { "_NET_WM_WINDOW_TYPE_UTILITY", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_UTILITY },
   { "_NET_WM_WINDOW_TYPE_SPLASH", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_SPLASH },
   { "_NET_WM_WINDOW_TYPE_DIALOG", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DIALOG },
   { "_NET_WM_WINDOW_TYPE_NORMAL", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NORMAL },
   { "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU",
     &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DROPDOWN_MENU },
   { "_NET_WM_WINDOW_TYPE_POPUP_MENU",
     &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_POPUP_MENU },
   { "_NET_WM_WINDOW_TYPE_TOOLTIP", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLTIP },
   { "_NET_WM_WINDOW_TYPE_NOTIFICATION",
     &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NOTIFICATION },
   { "_NET_WM_WINDOW_TYPE_COMBO", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_COMBO },
   { "_NET_WM_WINDOW_TYPE_DND", &ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DND },

   { "_NET_WM_STATE", &ECORE_X_ATOM_NET_WM_STATE },
   { "_NET_WM_STATE_MODAL", &ECORE_X_ATOM_NET_WM_STATE_MODAL },
   { "_NET_WM_STATE_STICKY", &ECORE_X_ATOM_NET_WM_STATE_STICKY },
   { "_NET_WM_STATE_MAXIMIZED_VERT",
     &ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_VERT },
   { "_NET_WM_STATE_MAXIMIZED_HORZ",
     &ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_HORZ },
   { "_NET_WM_STATE_SHADED", &ECORE_X_ATOM_NET_WM_STATE_SHADED },
   { "_NET_WM_STATE_SKIP_TASKBAR", &ECORE_X_ATOM_NET_WM_STATE_SKIP_TASKBAR },
   { "_NET_WM_STATE_SKIP_PAGER", &ECORE_X_ATOM_NET_WM_STATE_SKIP_PAGER },
   { "_NET_WM_STATE_HIDDEN", &ECORE_X_ATOM_NET_WM_STATE_HIDDEN },
   { "_NET_WM_STATE_FULLSCREEN", &ECORE_X_ATOM_NET_WM_STATE_FULLSCREEN },
   { "_NET_WM_STATE_ABOVE", &ECORE_X_ATOM_NET_WM_STATE_ABOVE },
   { "_NET_WM_STATE_BELOW", &ECORE_X_ATOM_NET_WM_STATE_BELOW },
   { "_NET_WM_STATE_DEMANDS_ATTENTION",
     &ECORE_X_ATOM_NET_WM_STATE_DEMANDS_ATTENTION },

   { "_NET_WM_ALLOWED_ACTIONS", &ECORE_X_ATOM_NET_WM_ALLOWED_ACTIONS },
   { "_NET_WM_ACTION_MOVE", &ECORE_X_ATOM_NET_WM_ACTION_MOVE },
   { "_NET_WM_ACTION_RESIZE", &ECORE_X_ATOM_NET_WM_ACTION_RESIZE },
   { "_NET_WM_ACTION_MINIMIZE", &ECORE_X_ATOM_NET_WM_ACTION_MINIMIZE },
   { "_NET_WM_ACTION_SHADE", &ECORE_X_ATOM_NET_WM_ACTION_SHADE },
   { "_NET_WM_ACTION_STICK", &ECORE_X_ATOM_NET_WM_ACTION_STICK },
   { "_NET_WM_ACTION_MAXIMIZE_HORZ",
     &ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_HORZ },
   { "_NET_WM_ACTION_MAXIMIZE_VERT",
     &ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_VERT },
   { "_NET_WM_ACTION_FULLSCREEN", &ECORE_X_ATOM_NET_WM_ACTION_FULLSCREEN },
   { "_NET_WM_ACTION_CHANGE_DESKTOP",
     &ECORE_X_ATOM_NET_WM_ACTION_CHANGE_DESKTOP },
   { "_NET_WM_ACTION_CLOSE", &ECORE_X_ATOM_NET_WM_ACTION_CLOSE },
   { "_NET_WM_ACTION_ABOVE", &ECORE_X_ATOM_NET_WM_ACTION_ABOVE },
   { "_NET_WM_ACTION_BELOW", &ECORE_X_ATOM_NET_WM_ACTION_BELOW },

   { "_NET_WM_STRUT", &ECORE_X_ATOM_NET_WM_STRUT },
   { "_NET_WM_STRUT_PARTIAL", &ECORE_X_ATOM_NET_WM_STRUT_PARTIAL },
   { "_NET_WM_ICON_GEOMETRY", &ECORE_X_ATOM_NET_WM_ICON_GEOMETRY },
   { "_NET_WM_ICON", &ECORE_X_ATOM_NET_WM_ICON },
   { "_NET_WM_PID", &ECORE_X_ATOM_NET_WM_PID },
   { "_NET_WM_HANDLED_ICONS", &ECORE_X_ATOM_NET_WM_HANDLED_ICONS },
   { "_NET_WM_USER_TIME", &ECORE_X_ATOM_NET_WM_USER_TIME },
   { "_NET_STARTUP_ID", &ECORE_X_ATOM_NET_STARTUP_ID },
   { "_NET_FRAME_EXTENTS", &ECORE_X_ATOM_NET_FRAME_EXTENTS },

   { "_NET_WM_PING", &ECORE_X_ATOM_NET_WM_PING },
   { "_NET_WM_SYNC_REQUEST", &ECORE_X_ATOM_NET_WM_SYNC_REQUEST },
   { "_NET_WM_SYNC_REQUEST_COUNTER",
     &ECORE_X_ATOM_NET_WM_SYNC_REQUEST_COUNTER },

   { "_NET_WM_WINDOW_OPACITY", &ECORE_X_ATOM_NET_WM_WINDOW_OPACITY },
   { "_NET_WM_WINDOW_SHADOW", &ECORE_X_ATOM_NET_WM_WINDOW_SHADOW },
   { "_NET_WM_WINDOW_SHADE", &ECORE_X_ATOM_NET_WM_WINDOW_SHADE },

   { "TARGETS", &ECORE_X_ATOM_SELECTION_TARGETS },
   { "CLIPBOARD", &ECORE_X_ATOM_SELECTION_CLIPBOARD },
   { "PRIMARY", &ECORE_X_ATOM_SELECTION_PRIMARY },
   { "SECONDARY", &ECORE_X_ATOM_SELECTION_SECONDARY },
   { "_ECORE_SELECTION_PRIMARY", &ECORE_X_ATOM_SELECTION_PROP_PRIMARY },
   { "_ECORE_SELECTION_SECONDARY", &ECORE_X_ATOM_SELECTION_PROP_SECONDARY },
   { "_ECORE_SELECTION_CLIPBOARD", &ECORE_X_ATOM_SELECTION_PROP_CLIPBOARD },

   { "_E_VIRTUAL_KEYBOARD", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD },
   { "_E_VIRTUAL_KEYBOARD_STATE", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_STATE },
   { "_E_VIRTUAL_KEYBOARD_ON", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ON },
   { "_E_VIRTUAL_KEYBOARD_OFF", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_OFF },
   { "_E_VIRTUAL_KEYBOARD_ALPHA", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ALPHA },
   { "_E_VIRTUAL_KEYBOARD_NUMERIC", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_NUMERIC },
   { "_E_VIRTUAL_KEYBOARD_PIN", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PIN },
   { "_E_VIRTUAL_KEYBOARD_PHONE_NUMBER",
     &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PHONE_NUMBER },
   { "_E_VIRTUAL_KEYBOARD_HEX", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HEX },
   { "_E_VIRTUAL_KEYBOARD_TERMINAL",
     &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_TERMINAL },
   { "_E_VIRTUAL_KEYBOARD_PASSWORD",
     &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PASSWORD },
   { "_E_VIRTUAL_KEYBOARD_IP", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_IP },
   { "_E_VIRTUAL_KEYBOARD_HOST", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HOST },
   { "_E_VIRTUAL_KEYBOARD_FILE", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_FILE },
   { "_E_VIRTUAL_KEYBOARD_URL", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_URL },
   { "_E_VIRTUAL_KEYBOARD_KEYPAD", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_KEYPAD },
   { "_E_VIRTUAL_KEYBOARD_J2ME", &ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_J2ME },

   { "_E_ILLUME_ZONE", &ECORE_X_ATOM_E_ILLUME_ZONE },
   { "_E_ILLUME_ZONE_LIST", &ECORE_X_ATOM_E_ILLUME_ZONE_LIST },
   { "_E_ILLUME_CONFORMANT", &ECORE_X_ATOM_E_ILLUME_CONFORMANT },
   { "_E_ILLUME_MODE", &ECORE_X_ATOM_E_ILLUME_MODE },
   { "_E_ILLUME_MODE_SINGLE", &ECORE_X_ATOM_E_ILLUME_MODE_SINGLE },
   { "_E_ILLUME_MODE_DUAL_TOP", &ECORE_X_ATOM_E_ILLUME_MODE_DUAL_TOP },
   { "_E_ILLUME_MODE_DUAL_LEFT", &ECORE_X_ATOM_E_ILLUME_MODE_DUAL_LEFT },
   { "_E_ILLUME_FOCUS_BACK", &ECORE_X_ATOM_E_ILLUME_FOCUS_BACK },
   { "_E_ILLUME_FOCUS_FORWARD", &ECORE_X_ATOM_E_ILLUME_FOCUS_FORWARD },
   { "_E_ILLUME_FOCUS_HOME", &ECORE_X_ATOM_E_ILLUME_FOCUS_HOME },
   { "_E_ILLUME_CLOSE", &ECORE_X_ATOM_E_ILLUME_CLOSE },
   { "_E_ILLUME_HOME_NEW", &ECORE_X_ATOM_E_ILLUME_HOME_NEW },
   { "_E_ILLUME_HOME_DEL", &ECORE_X_ATOM_E_ILLUME_HOME_DEL },
   { "_E_ILLUME_DRAG", &ECORE_X_ATOM_E_ILLUME_DRAG },
   { "_E_ILLUME_DRAG_LOCKED", &ECORE_X_ATOM_E_ILLUME_DRAG_LOCKED },
   { "_E_ILLUME_DRAG_START", &ECORE_X_ATOM_E_ILLUME_DRAG_START },
   { "_E_ILLUME_DRAG_END", &ECORE_X_ATOM_E_ILLUME_DRAG_END },
   { "_E_ILLUME_INDICATOR_GEOMETRY",
     &ECORE_X_ATOM_E_ILLUME_INDICATOR_GEOMETRY },
   { "_E_ILLUME_SOFTKEY_GEOMETRY", &ECORE_X_ATOM_E_ILLUME_SOFTKEY_GEOMETRY },
   { "_E_ILLUME_KEYBOARD_GEOMETRY", &ECORE_X_ATOM_E_ILLUME_KEYBOARD_GEOMETRY },
   { "_E_ILLUME_QUICKPANEL", &ECORE_X_ATOM_E_ILLUME_QUICKPANEL },
   { "_E_ILLUME_QUICKPANEL_STATE", &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE },
   { "_E_ILLUME_QUICKPANEL_STATE_TOGGLE",
     &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE_TOGGLE },
   { "_E_ILLUME_QUICKPANEL_ON", &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ON },
   { "_E_ILLUME_QUICKPANEL_OFF", &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_OFF },
   { "_E_ILLUME_QUICKPANEL_PRIORITY_MAJOR",
     &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MAJOR },
   { "_E_ILLUME_QUICKPANEL_PRIORITY_MINOR",
     &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MINOR },
   { "_E_ILLUME_QUICKPANEL_ZONE", &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ZONE },
   { "_E_ILLUME_QUICKPANEL_POSITION_UPDATE",
     &ECORE_X_ATOM_E_ILLUME_QUICKPANEL_POSITION_UPDATE },
   { "_E_ILLUME_INDICATOR_STATE", &ECORE_X_ATOM_E_ILLUME_INDICATOR_STATE },
   { "_E_ILLUME_INDICATOR_ON", &ECORE_X_ATOM_E_ILLUME_INDICATOR_ON },
   { "_E_ILLUME_INDICATOR_OFF", &ECORE_X_ATOM_E_ILLUME_INDICATOR_OFF },
   { "_E_ILLUME_INDICATOR_OPACITY_MODE", &ECORE_X_ATOM_E_ILLUME_INDICATOR_OPACITY_MODE },
   { "_E_ILLUME_INDICATOR_OPAQUE", &ECORE_X_ATOM_E_ILLUME_INDICATOR_OPAQUE },
   { "_E_ILLUME_INDICATOR_TRANSLUCENT", &ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSLUCENT },
   { "_E_ILLUME_INDICATOR_TRANSPARENT", &ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSPARENT },
   { "_E_ILLUME_ROTATE_WINDOW_AVAILABLE_ANGLES", &ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_AVAILABLE_ANGLE },
   { "_E_ILLUME_ROTATE_WINDOW_ANGLE", &ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_ANGLE },
   { "_E_ILLUME_ROTATE_ROOT_ANGLE", &ECORE_X_ATOM_E_ILLUME_ROTATE_ROOT_ANGLE },
   { "_E_ILLUME_CLIPBOARD_STATE", &ECORE_X_ATOM_E_ILLUME_CLIPBOARD_STATE },
   { "_E_ILLUME_CLIPBOARD_ON", &ECORE_X_ATOM_E_ILLUME_CLIPBOARD_ON },
   { "_E_ILLUME_CLIPBOARD_OFF", &ECORE_X_ATOM_E_ILLUME_CLIPBOARD_OFF },
   { "_E_ILLUME_CLIPBOARD_GEOMETRY", &ECORE_X_ATOM_E_ILLUME_CLIPBOARD_GEOMETRY },
   { "_E_ILLUME_WINDOW_STATE", &ECORE_X_ATOM_E_ILLUME_WINDOW_STATE },
   { "_E_ILLUME_WINDOW_STATE_NORMAL", &ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_NORMAL },
   { "_E_ILLUME_WINDOW_STATE_FLOATING", &ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_FLOATING },
   { "_E_ILLUME_ACCESS_CONTROL", &ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL },
   { "_E_ILLUME_ACCESS_ACTION_NEXT", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_NEXT },
   { "_E_ILLUME_ACCESS_ACTION_PREV", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_PREV },
   { "_E_ILLUME_ACCESS_ACTION_ACTIVATE", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_ACTIVATE },
   { "_E_ILLUME_ACCESS_ACTION_READ", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ },
   { "_E_ILLUME_ACCESS_ACTION_READ_NEXT", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_NEXT },
   { "_E_ILLUME_ACCESS_ACTION_READ_PREV", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_PREV },
   { "_E_ILLUME_ACCESS_ACTION_UP", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_UP },
   { "_E_ILLUME_ACCESS_ACTION_DOWN", &ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_DOWN },
   { "_E_COMP_SYNC_COUNTER", &ECORE_X_ATOM_E_COMP_SYNC_COUNTER },
   { "_E_COMP_SYNC_DRAW_DONE", &ECORE_X_ATOM_E_COMP_SYNC_DRAW_DONE },
   { "_E_COMP_SYNC_SUPPORTED", &ECORE_X_ATOM_E_COMP_SYNC_SUPPORTED },
   { "_E_COMP_SYNC_BEGIN", &ECORE_X_ATOM_E_COMP_SYNC_BEGIN },
   { "_E_COMP_SYNC_END", &ECORE_X_ATOM_E_COMP_SYNC_END },
   { "_E_COMP_SYNC_CANCEL", &ECORE_X_ATOM_E_COMP_SYNC_CANCEL },

   { "_E_COMP_FLUSH", &ECORE_X_ATOM_E_COMP_FLUSH },
   { "_E_COMP_DUMP", &ECORE_X_ATOM_E_COMP_DUMP },
   { "_E_COMP_PIXMAP", &ECORE_X_ATOM_E_COMP_PIXMAP },
   { "_E_VIDEO_PARENT", &ECORE_X_ATOM_E_VIDEO_PARENT },
   { "_E_VIDEO_POSITION", &ECORE_X_ATOM_E_VIDEO_POSITION },

   { "_E_PROFILE", &ECORE_X_ATOM_E_PROFILE },
   { "_E_PROFILE_LIST", &ECORE_X_ATOM_E_PROFILE_LIST }

   /* SLP additions after the comma */ ,

   /* for sliding window */
   { "_E_ILLUME_SLIDING_WIN_STATE", &ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_STATE },
   { "_E_ILLUME_SLIDING_WIN_GEOMETRY", &ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_GEOMETRY },

   /* for SDB(Samsung Debug Bridge) */
   { "_SDB_SERVER_CONNECT", &ECORE_X_ATOM_SDB_SERVER_CONNECT },
   { "_SDB_SERVER_DISCONNECT", &ECORE_X_ATOM_SDB_SERVER_DISCONNECT }
};

