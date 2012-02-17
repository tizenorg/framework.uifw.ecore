#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <Ecore.h>
#include <ecore_private.h>

#include "Ecore_IMF.h"
#include "ecore_imf_private.h"

/**
 * @defgroup Ecore_IMF_Context_Group Ecore Input Method Context Functions
 *
 * Functions that operate on Ecore Input Method Context objects.
 */

/**
 * Get the list of the available Input Method Context ids.
 *
 * Note that the caller is responsible for freeing the Eina_List
 * when finished with it. There is no need to finish the list strings.
 *
 * @return Return an Eina_List of strings;
 *         on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI Eina_List *
ecore_imf_context_available_ids_get(void)
{
   return ecore_imf_module_context_ids_get();
}

EAPI Eina_List *
ecore_imf_context_available_ids_by_canvas_type_get(const char *canvas_type)
{
   return ecore_imf_module_context_ids_by_canvas_type_get(canvas_type);
}

/*
 * Match @locale against @against.
 *
 * 'en_US' against 'en_US'       => 4
 * 'en_US' against 'en'          => 3
 * 'en', 'en_UK' against 'en_US' => 2
 *  all locales, against '*'     => 1
 */
static int
_ecore_imf_context_match_locale(const char *locale, const char *against, int against_len)
{
   if (strcmp(against, "*") == 0)
     return 1;

   if (strcasecmp(locale, against) == 0)
     return 4;

   if (strncasecmp(locale, against, 2) == 0)
     return (against_len == 2) ? 3 : 2;

   return 0;
}

/**
 * Get the id of the default Input Method Context.
 * The id may to used to create a new instance of an Input Method
 * Context object.
 *
 * @return Return a string containing the id of the default Input
 *         Method Context; on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI const char *
ecore_imf_context_default_id_get(void)
{
   return ecore_imf_context_default_id_by_canvas_type_get(NULL);
}

EAPI const char *
ecore_imf_context_default_id_by_canvas_type_get(const char *canvas_type)
{
   const char *id;
   Eina_List *modules;
   Ecore_IMF_Module *module;
   char *locale;
   char *tmp;
   int best_goodness = 0;

   id = getenv("ECORE_IMF_MODULE");
   if (id)
     {
        if (strcmp(id, "none") == 0) return NULL;
        if (ecore_imf_module_get(id)) return id;
     }

   modules = ecore_imf_module_available_get();
   if (!modules) return NULL;

   locale = setlocale(LC_CTYPE, NULL);
   if (!locale) return NULL;

   locale = strdup(locale);

   tmp = strchr(locale, '.');
   if (tmp) *tmp = '\0';
   tmp = strchr(locale, '@');
   if (tmp) *tmp = '\0';

   id = NULL;

   EINA_LIST_FREE(modules, module)
     {
        if (canvas_type &&
            strcmp(module->info->canvas_type, canvas_type) == 0)
          continue;

        const char *p = module->info->default_locales;
        while (p)
          {
             const char *q = strchr(p, ':');
             int goodness = _ecore_imf_context_match_locale(locale, p, q ? (size_t)(q - p) : strlen (p));

             if (goodness > best_goodness)
               {
                  id = module->info->id;
                  best_goodness = goodness;
               }

             p = q ? q + 1 : NULL;
          }
     }

   free(locale);
   return id;
}

/**
 * Retrieve the info for the Input Method Context with @p id.
 *
 * @param id The Input Method Context id to query for.
 * @return Return a #Ecore_IMF_Context_Info for the Input Method Context with @p id;
 *         on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI const Ecore_IMF_Context_Info *
ecore_imf_context_info_by_id_get(const char *id)
{
   Ecore_IMF_Module *module;

   if (!id) return NULL;
   module = ecore_imf_module_get(id);
   if (!module) return NULL;
   return module->info;
}

/**
 * Create a new Input Method Context defined by the given id.
 *
 * @param id The Input Method Context id.
 * @return A newly allocated Input Method Context;
 *         on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI Ecore_IMF_Context *
ecore_imf_context_add(const char *id)
{
   Ecore_IMF_Context *ctx;

   if (!id) return NULL;
   ctx = ecore_imf_module_context_create(id);
   if (!ctx || !ctx->klass) return NULL;
   if (ctx->klass->add) ctx->klass->add(ctx);
   /* default use_preedit is EINA_TRUE, so let's make sure it's
    * set on the immodule */
   ecore_imf_context_use_preedit_set(ctx, EINA_TRUE);

   /* default prediction is EINA_TRUE, so let's make sure it's
    * set on the immodule */
   ecore_imf_context_prediction_allow_set(ctx, EINA_TRUE);

   /* default autocapital type is SENTENCE type, so let's make sure it's
    * set on the immodule */
   ecore_imf_context_autocapital_type_set(ctx, ECORE_IMF_AUTOCAPITAL_TYPE_SENTENCE);

   /* default input panel enabled status is EINA_TRUE, so let's make sure it's
    * set on the immodule */
   ecore_imf_context_input_panel_enabled_set(ctx, EINA_TRUE);

   /* default input_mode is ECORE_IMF_INPUT_MODE_FULL, so let's make sure it's
    * set on the immodule */
   ecore_imf_context_input_mode_set(ctx, ECORE_IMF_INPUT_MODE_FULL);
   return ctx;
}

/**
 * Retrieve the info for the given Input Method Context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Return a #Ecore_IMF_Context_Info for the given Input Method Context;
 *         on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI const Ecore_IMF_Context_Info *
ecore_imf_context_info_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_info_get");
        return NULL;
     }
   return ctx->module->info;
}

/**
 * Delete the given Input Method Context and free its memory.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_del(Ecore_IMF_Context *ctx)
{
   Ecore_IMF_Func_Node *fn;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_del");
        return;
     }
   if (ctx->klass->del) ctx->klass->del(ctx);

   if (ctx->callbacks)
     {
        EINA_LIST_FREE(ctx->callbacks, fn)
           free(fn);
     }

   ECORE_MAGIC_SET(ctx, ECORE_MAGIC_NONE);
   free(ctx);
}

/**
 * Set the client window for the Input Method Context; this is the
 * Ecore_X_Window when using X11, Ecore_Win32_Window when using Win32, etc.
 * This window is used in order to correctly position status windows, and may
 * also be used for purposes internal to the Input Method Context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param window The client window. This may be NULL to indicate
 *               that the previous client window no longer exists.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_client_window_set(Ecore_IMF_Context *ctx, void *window)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_client_window_set");
        return;
     }
   if (ctx->klass->client_window_set) ctx->klass->client_window_set(ctx, window);
   ctx->window = window;
}

/**
 * Get the client window of the Input Method Context
 *
 * See @ref ecore_imf_context_client_window_set for more details.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Return the client window.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void *
ecore_imf_context_client_window_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_client_window_get");
        return NULL;
     }
   return ctx->window;
}

/**
 * Set the client canvas for the Input Method Context; this is the
 * canvas in which the input appears.
 * The canvas type can be determined by using the context canvas type.
 * Actually only canvas with type "evas" (Evas *) is supported.
 * This canvas may be used in order to correctly position status windows, and may
 * also be used for purposes internal to the Input Method Context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param canvas The client canvas. This may be NULL to indicate
 *               that the previous client canvas no longer exists.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_client_canvas_set(Ecore_IMF_Context *ctx, void *canvas)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_client_canvas_set");
        return;
     }
   if (ctx->klass->client_canvas_set) ctx->klass->client_canvas_set(ctx, canvas);
   ctx->client_canvas = canvas;
}

/**
 * Get the client canvas of the Input Method Context.
 *
 * See @ref ecore_imf_context_client_canvas_set for more details.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Return the client canvas.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void *
ecore_imf_context_client_canvas_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_client_canvas_get");
        return NULL;
     }
   return ctx->client_canvas;
}

/**
 * Ask the Input Method Context to show itself.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_show(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_show");
        return;
     }
   if (ctx->klass->show) ctx->klass->show(ctx);
}

/**
 * Ask the Input Method Context to hide itself.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_hide(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_hide");
        return;
     }
   if (ctx->klass->hide) ctx->klass->hide(ctx);
}

/**
 * Retrieve the current preedit string and cursor position
 * for the Input Method Context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param str Location to store the retrieved string. The
 *            string retrieved must be freed with free().
 * @param cursor_pos Location to store position of cursor (in characters)
 *                   within the preedit string.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_preedit_string_get(Ecore_IMF_Context *ctx, char **str, int *cursor_pos)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_preedit_string_get");
        return;
     }
   if (ctx->klass->preedit_string_get)
     ctx->klass->preedit_string_get(ctx, str, cursor_pos);
   else
     {
        if (str) *str = strdup("");
        if (cursor_pos) *cursor_pos = 0;
     }
}

/**
 * Retrieve the current preedit string, atrributes and
 * cursor position for the Input Method Context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param str Location to store the retrieved string. The
 *            string retrieved must be freed with free().
 * @param attrs an Eina_List of attributes
 * @param cursor_pos Location to store position of cursor (in characters)
 *                   within the preedit string.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_preedit_string_with_attributes_get(Ecore_IMF_Context *ctx, char **str, Eina_List **attrs, int *cursor_pos)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_preedit_string_with_attributes_get");
        return;
     }
   if (ctx->klass->preedit_string_with_attributes_get)
     ctx->klass->preedit_string_with_attributes_get(ctx, str, attrs, cursor_pos);
   else
     {
        if (str) *str = strdup("");
        if (attrs) *attrs = NULL;
        if (cursor_pos) *cursor_pos = 0;
     }
}

/**
 * Notify the Input Method Context that the widget to which its
 * correspond has gained focus.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_focus_in(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_focus_in");
        return;
     }
   if (ctx->klass->focus_in) ctx->klass->focus_in(ctx);
}

/**
 * Notify the Input Method Context that the widget to which its
 * correspond has lost focus.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_focus_out(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_focus_out");
        return;
     }
   if (ctx->klass->focus_out) ctx->klass->focus_out(ctx);
}

/**
 * Notify the Input Method Context that a change such as a
 * change in cursor position has been made. This will typically
 * cause the Input Method Context to clear the preedit state.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_reset(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_reset");
        return;
     }
   if (ctx->klass->reset) ctx->klass->reset(ctx);
}

/**
 * Notify the Input Method Context that a change in the cursor
 * position has been made.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param cursor_pos New cursor position in characters.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_cursor_position_set(Ecore_IMF_Context *ctx, int cursor_pos)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_cursor_position_set");
        return;
     }
   if (ctx->klass->cursor_position_set) ctx->klass->cursor_position_set(ctx, cursor_pos);
}

/**
 * Notify the Input Method Context that a change in the cursor
 * location has been made. The location is relative to the canvas.
 * The cursor location can be used to determine the position of 
 * candidate word window in the immodule.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param x cursor x position.
 * @param x cursor y position.
 * @param w cursor width.
 * @param h cursor height.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_cursor_location_set(Ecore_IMF_Context *ctx, int x, int y, int w, int h)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_cursor_location_set");
        return;
     }
   if (ctx->klass->cursor_location_set) ctx->klass->cursor_location_set(ctx, x, y, w, h);
}

/**
 * Set whether the IM context should use the preedit string
 * to display feedback. If @c use_preedit is EINA_FALSE (default
 * is EINA_TRUE), then the IM context may use some other method to display
 * feedback, such as displaying it in a child of the root window.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param use_preedit Whether the IM context should use the preedit string.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_use_preedit_set(Ecore_IMF_Context *ctx, Eina_Bool use_preedit)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_use_preedit_set");
        return;
     }
   if (ctx->klass->use_preedit_set) ctx->klass->use_preedit_set(ctx, use_preedit);
}

/**
 * Set whether the IM context should allow to use the text prediction.
 * If @c prediction is EINA_FALSE (default is EINA_TRUE), then the IM context will not display the text prediction window.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param prediction Whether the IM context should allow to use the text prediction.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_prediction_allow_set(Ecore_IMF_Context *ctx, Eina_Bool prediction)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_prediction_allow_set");
        return;
     }

   ctx->allow_prediction = prediction;

   if (ctx->klass->prediction_allow_set)
     ctx->klass->prediction_allow_set(ctx, prediction);
}

/**
 * Get whether the IM context should allow to use the text prediction.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return EINA_TRUE if it allows to use the text prediction, otherwise EINA_FALSE.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI Eina_Bool
ecore_imf_context_prediction_allow_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_prediction_allow_get");
        return EINA_FALSE;
     }

   return ctx->allow_prediction;
}

/**
 * Set the autocapitalization type on the immodule. 
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param autocapital_type the autocapitalization type.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_autocapital_type_set(Ecore_IMF_Context *ctx, Ecore_IMF_Autocapital_Type autocapital_type)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_autocapital_type_set");
        return;
     }

   ctx->autocapital_type = autocapital_type;

   if (ctx->klass->autocapital_type_set) ctx->klass->autocapital_type_set(ctx, autocapital_type);
}

/**
 * Get the autocapitalization type.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return The autocapital type being used by @p ctx.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI Ecore_IMF_Autocapital_Type
ecore_imf_context_autocapital_type_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_autocapital_allow_get");
        return ECORE_IMF_AUTOCAPITAL_TYPE_NONE;
     }

   return ctx->autocapital_type;
}

/**
 * Set the callback to be used on get_surrounding request.
 *
 * This callback will be called when the Input Method Context
 * module requests the surrounding context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param func The callback to be called.
 * @param data The data pointer to be passed to @p func
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_retrieve_surrounding_callback_set(Ecore_IMF_Context *ctx, Eina_Bool (*func)(void *data, Ecore_IMF_Context *ctx, char **text, int *cursor_pos), const void *data)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_retrieve_surrounding_callback_set");
        return;
     }

   ctx->retrieve_surrounding_func = func;
   ctx->retrieve_surrounding_data = (void *) data;
}

/**
 * Set the input mode used by the Ecore Input Context.
 *
 * The input mode can be one of the input modes defined in
 * #Ecore_IMF_Input_Mode. The default input mode is
 * ECORE_IMF_INPUT_MODE_FULL.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param input_mode The input mode to be used by @p ctx.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI void
ecore_imf_context_input_mode_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Mode input_mode)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_mode_set");
        return;
     }
   if (ctx->klass->input_mode_set) ctx->klass->input_mode_set(ctx, input_mode);
   ctx->input_mode = input_mode;
}

/**
 * Get the input mode being used by the Ecore Input Context.
 *
 * See @ref ecore_imf_context_input_mode_set for more details.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return The input mode being used by @p ctx.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI Ecore_IMF_Input_Mode
ecore_imf_context_input_mode_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_mode_set");
        return 0;
     }
   return ctx->input_mode;
}

/**
 * Allow an Ecore Input Context to internally handle an event.
 * If this function returns EINA_TRUE, then no further processing
 * should be done for this event.
 *
 * Input methods must be able to accept all types of events (simply
 * returning EINA_FALSE if the event was not handled), but there is no
 * obligation of any events to be submitted to this function.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param type The type of event defined by #Ecore_IMF_Event_Type.
 * @param event The event itself.
 * @return EINA_TRUE if the event was handled; otherwise EINA_FALSE.
 * @ingroup Ecore_IMF_Context_Group
 */
EAPI Eina_Bool
ecore_imf_context_filter_event(Ecore_IMF_Context *ctx, Ecore_IMF_Event_Type type, Ecore_IMF_Event *event)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_filter_event");
        return EINA_FALSE;
     }
   if (ctx->klass->filter_event) return ctx->klass->filter_event(ctx, type, event);
   return EINA_FALSE;
}

/**
 * @defgroup Ecore_IMF_Context_Module_Group Ecore Input Method Context Module Functions
 *
 * Functions that should be used by Ecore Input Method Context modules.
 */

/**
 * Creates a new Input Method Context with klass specified by @p ctxc.
 *
 * This method should be used by modules implementing the Input
 * Method Context interface.
 *
 * @param ctxc An #Ecore_IMF_Context_Class.
 * @return A new #Ecore_IMF_Context; on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI Ecore_IMF_Context *
ecore_imf_context_new(const Ecore_IMF_Context_Class *ctxc)
{
   Ecore_IMF_Context *ctx;

   if (!ctxc) return NULL;
   ctx = calloc(1, sizeof(Ecore_IMF_Context));
   if (!ctx) return NULL;
   ECORE_MAGIC_SET(ctx, ECORE_MAGIC_CONTEXT);
   ctx->klass = ctxc;
   ctx->data = NULL;
   ctx->retrieve_surrounding_func = NULL;
   ctx->retrieve_surrounding_data = NULL;
   return ctx;
}

/**
 * Set the Input Method Context specific data.
 *
 * Note that this method should be used by modules to set
 * the Input Method Context specific data and it's not meant to
 * be used by applications to store application specific data.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param data The Input Method Context specific data.
 * @return A new #Ecore_IMF_Context; on failure it returns NULL.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void
ecore_imf_context_data_set(Ecore_IMF_Context *ctx, void *data)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_data_set");
        return;
     }
   ctx->data = data;
}

/**
 * Get the Input Method Context specific data.
 *
 * See @ref ecore_imf_context_data_set for more details.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return The Input Method Context specific data.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void *ecore_imf_context_data_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_data_get");
        return NULL;
     }
   return ctx->data;
}

/**
 * Retrieve context around insertion point.
 *
 * This function is implemented by calling the
 * Ecore_IMF_Context::retrieve_surrounding_func (
 * set using #ecore_imf_context_retrieve_surrounding_callback_set).
 *
 * There is no obligation for a widget to respond to the
 * ::retrieve_surrounding_func, so input methods must be prepared
 * to function without context.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param text Location to store a UTF-8 encoded string of text
 *             holding context around the insertion point.
 *             If the function returns EINA_TRUE, then you must free
 *             the result stored in this location with free().
 * @param cursor_pos Location to store the position in characters of
 *                   the insertion cursor within @text.
 * @return EINA_TRUE if surrounding text was provided; otherwise EINA_FALSE.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI Eina_Bool
ecore_imf_context_surrounding_get(Ecore_IMF_Context *ctx, char **text, int *cursor_pos)
{
   int result = EINA_FALSE;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_surrounding_get");
        return EINA_FALSE;
     }

   if (ctx->retrieve_surrounding_func)
     {
        result = ctx->retrieve_surrounding_func(ctx->retrieve_surrounding_data, ctx, text, cursor_pos);
        if (!result)
          {
             if (text) *text = NULL;
             if (cursor_pos) *cursor_pos = 0;
          }
     }
   return result;
}

static void
_ecore_imf_event_free_preedit(void *data __UNUSED__, void *event)
{
   free(event);
}

/**
 * Adds ECORE_IMF_EVENT_PREEDIT_START to the event queue.
 *
 * ECORE_IMF_EVENT_PREEDIT_START should be added when a new preedit sequence starts.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void
ecore_imf_context_preedit_start_event_add(Ecore_IMF_Context *ctx)
{
   Ecore_IMF_Event_Commit *ev;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_preedit_start_event_add");
        return;
     }

   ev = malloc(sizeof(Ecore_IMF_Event_Preedit_Start));
   ev->ctx = ctx;
   ecore_event_add(ECORE_IMF_EVENT_PREEDIT_START,
                   ev, _ecore_imf_event_free_preedit, NULL);
}

/**
 * Adds ECORE_IMF_EVENT_PREEDIT_END to the event queue.
 *
 * ECORE_IMF_EVENT_PREEDIT_END should be added when a new preedit sequence has been completed or canceled.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void
ecore_imf_context_preedit_end_event_add(Ecore_IMF_Context *ctx)
{
   Ecore_IMF_Event_Commit *ev;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_preedit_end_event_add");
        return;
     }

   ev = malloc(sizeof(Ecore_IMF_Event_Preedit_End));
   ev->ctx = ctx;
   ecore_event_add(ECORE_IMF_EVENT_PREEDIT_END,
                   ev, _ecore_imf_event_free_preedit, NULL);
}

/**
 * Adds ECORE_IMF_EVENT_PREEDIT_CHANGED to the event queue.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void
ecore_imf_context_preedit_changed_event_add(Ecore_IMF_Context *ctx)
{
   Ecore_IMF_Event_Commit *ev;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_preedit_changed_event_add");
        return;
     }

   ev = malloc(sizeof(Ecore_IMF_Event_Preedit_Changed));
   ev->ctx = ctx;
   ecore_event_add(ECORE_IMF_EVENT_PREEDIT_CHANGED,
                   ev, _ecore_imf_event_free_preedit, NULL);
}

static void
_ecore_imf_event_free_commit(void *data __UNUSED__, void *event)
{
   Ecore_IMF_Event_Commit *ev;

   ev = event;
   if (ev->str) free(ev->str);
   free(ev);
}

/**
 * Adds ECORE_IMF_EVENT_COMMIT to the event queue.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param str The committed string.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void
ecore_imf_context_commit_event_add(Ecore_IMF_Context *ctx, const char *str)
{
   Ecore_IMF_Event_Commit *ev;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_commit_event_add");
        return;
     }

   ev = malloc(sizeof(Ecore_IMF_Event_Commit));
   ev->ctx = ctx;
   ev->str = str ? strdup(str) : NULL;
   ecore_event_add(ECORE_IMF_EVENT_COMMIT,
                   ev, _ecore_imf_event_free_commit, NULL);

}

static void
_ecore_imf_event_free_delete_surrounding(void *data __UNUSED__, void *event)
{
   free(event);
}

/**
 * Adds ECORE_IMF_EVENT_DELETE_SURROUNDING to the event queue.
 *
 * Asks the widget that the input context is attached to to delete characters around the cursor position
 * by adding the ECORE_IMF_EVENT_DELETE_SURROUNDING to the event queue.
 * Note that offset and n_chars are in characters not in bytes.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param offset The start offset of surrounding to be deleted.
 * @param n_chars The number of characters to be deleted.
 * @ingroup Ecore_IMF_Context_Module_Group
 */
EAPI void
ecore_imf_context_delete_surrounding_event_add(Ecore_IMF_Context *ctx, int offset, int n_chars)
{
   Ecore_IMF_Event_Delete_Surrounding *ev;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_delete_surrounding_event_add");
        return;
     }

   ev = malloc(sizeof(Ecore_IMF_Event_Delete_Surrounding));
   ev->ctx = ctx;
   ev->offset = offset;
   ev->n_chars = n_chars;
   ecore_event_add(ECORE_IMF_EVENT_DELETE_SURROUNDING,
                   ev, _ecore_imf_event_free_delete_surrounding, NULL);
}

/**
 * Add (register) a callback function to a given context event.
 *
 * This function adds a function callback to the context @p ctx when the
 * event of type @p type occurs on it. The function pointer is @p
 * func.
 *
 * The event type @p type to trigger the function may be one of
 * #ECORE_IMF_CALLBACK_PREEDIT_START, #ECORE_IMF_CALLBACK_PREEDIT_END,
 * #ECORE_IMF_CALLBACK_PREEDIT_CHANGED, #ECORE_IMF_CALLBACK_COMMIT and
 * #ECORE_IMF_CALLBACK_DELETE_SURROUNDING.
 *
 * @param ctx Ecore_IMF_Context to attach a callback to.
 * @param type The type of event that will trigger the callback
 * @param func The (callback) function to be called when the event is
 *        triggered
 * @param data The data pointer to be passed to @p func
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_event_callback_add(Ecore_IMF_Context *ctx, Ecore_IMF_Callback_Type type, Ecore_IMF_Event_Cb func, const void *data)
{
   Ecore_IMF_Func_Node *fn = NULL;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_event_callback_add");
        return;
     }

   if (!func) return;

   fn = calloc(1, sizeof (Ecore_IMF_Func_Node));
   if (!fn) return;

   fn->func = func;
   fn->data = data;
   fn->type = type;

   ctx->callbacks = eina_list_append(ctx->callbacks, fn);
}

/**
 * Delete (unregister) a callback function registered to a given
 * context event.
 *
 * This function removes a function callback from the context @p ctx when the
 * event of type @p type occurs on it. The function pointer is @p
 * func.
 *
 * @see ecore_imf_context_event_callback_add() for more details
 *
 * @param ctx Ecore_IMF_Context to remove a callback from.
 * @param type The type of event that was trigerring the callback
 * @param func The (callback) function that was to be called when the event was triggered
 * @return the data pointer
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void *
ecore_imf_context_event_callback_del(Ecore_IMF_Context *ctx, Ecore_IMF_Callback_Type type, Ecore_IMF_Event_Cb func)
{
   Eina_List *l = NULL;
   Eina_List *l_next = NULL;
   Ecore_IMF_Func_Node *fn = NULL;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_event_callback_del");
        return NULL;
     }

   if (!func) return NULL;
   if (!ctx->callbacks) return NULL;

   EINA_LIST_FOREACH_SAFE(ctx->callbacks, l, l_next, fn)
     {
        if ((fn) && (fn->func == func) && (fn->type == type))
          {
             void *tmp = (void *)fn->data;
             free(fn);
             ctx->callbacks = eina_list_remove_list(ctx->callbacks, l);
             return tmp;
          }
     }
   return NULL;
}

/**
 * Call a given callback on the context @p ctx.
 *
 * ecore_imf_context_preedit_start_event_add, ecore_imf_context_preedit_end_event_add, 
 * ecore_imf_context_preedit_changed_event_add, ecore_imf_context_commit_event_add and
 * ecore_imf_context_delete_surrounding_event_add APIs are asynchronous 
 * because those API adds each event to the event queue.
 *
 * This API provides the way to call each callback function immediately.
 *
 * @param ctx Ecore_IMF_Context.
 * @param type The type of event that will trigger the callback
 * @param event_info The pointer to event specific struct or information to
 *        pass to the callback functions registered on this event
 * @ingroup Ecore_IMF_Context_Module_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_event_callback_call(Ecore_IMF_Context *ctx, Ecore_IMF_Callback_Type type, void *event_info)
{
   Ecore_IMF_Func_Node *fn = NULL;
   Eina_List *l = NULL;

   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_event_callback_call");
        return;
     }

   EINA_LIST_FOREACH(ctx->callbacks, l, fn)
     {
        if ((fn) && (fn->type == type) && (fn->func))
          fn->func(fn->data, ctx, event_info);
     }
}

/**
 * Ask the Input Method Context to show the control panel of using Input Method.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_control_panel_show(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_control_panel_show");
        return;
     }

   if (ctx->klass->control_panel_show) ctx->klass->control_panel_show(ctx);
}

/**
 * Ask the Input Method Context to hide the control panel of using Input Method.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_control_panel_hide(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_control_panel_hide");
        return;
     }

   if (ctx->klass->control_panel_hide) ctx->klass->control_panel_hide(ctx);
}

/**
 * Ask the Input Method Context to show the input panel (virtual keyboard).
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_show(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_show");
        return;
     }

   if (ctx->klass->show) ctx->klass->show(ctx);
}

/**
 * Ask the Input Method Context to hide the input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_hide(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_hide");
        return;
     }

   if (ctx->klass->hide) ctx->klass->hide(ctx);
}

/**
 * Set the layout of the input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param layout see #ECORE_IMF_INPUT_PANEL_LAYOUT
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_layout_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Layout layout)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_layout_set");
        return;
     }

   if (ctx->klass->input_panel_layout_set)
     ctx->klass->input_panel_layout_set(ctx, layout);

   ctx->input_panel_layout = layout;
}

/**
 * Get the layout of the current active input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return layout see #Ecore_IMF_Input_Panel_Layout
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI Ecore_IMF_Input_Panel_Layout
ecore_imf_context_input_panel_layout_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_layout_get");
        return ECORE_IMF_INPUT_PANEL_LAYOUT_INVALID;
     }

   if (ctx->klass->input_panel_layout_get)
     return ctx->input_panel_layout;
   else
     return ECORE_IMF_INPUT_PANEL_LAYOUT_INVALID;
}

/**
 * Set the language of the input panel.
 * This API can be used when you want to show the English keyboard.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param lang the language to be set to the input panel.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_language_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Lang lang)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_language_set");
        return;
     }

   if (ctx->klass->input_panel_language_set) ctx->klass->input_panel_language_set(ctx, lang);
   ctx->input_panel_lang = lang;
}

/**
 * Get the language of the input panel.
 *
 * See @ref ecore_imf_context_input_panel_language_set for more details.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Ecore_IMF_Input_Panel_Lang
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI Ecore_IMF_Input_Panel_Lang
ecore_imf_context_input_panel_language_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_language_get");
        return ECORE_IMF_INPUT_PANEL_LANG_AUTOMATIC;
     }

   return ctx->input_panel_lang;
}

/**
 * Set whether the Input Method Context should request to show the input panel automatically
 * when the widget has focus.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param enabled If true, the input panel will be shown when the widget is clicked or has focus.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_enabled_set(Ecore_IMF_Context *ctx,
                                           Eina_Bool enabled)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_enabled_set");
        return;
     }

   ctx->input_panel_enabled = enabled;
}

/**
 * Get whether the Input Method Context requests to show the input panel automatically.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Return the attribute to show the input panel automatically
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI Eina_Bool
ecore_imf_context_input_panel_enabled_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_enabled_get");
        return EINA_FALSE;
     }

   return ctx->input_panel_enabled;
}

/**
 * Set the input panel-specific data to deliver to the input panel.
 * this API is used by applications to deliver specific data to the input panel.
 * the data format MUST be negotiated by both application and the input panel.
 * The size and format of data are defined by the input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param data The specific data to be set to the input panel.
 * @param len the length of data, in bytes, to send to the input panel
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_input_panel_imdata_set(Ecore_IMF_Context *ctx, const void *data, int len)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_imdata_set");
        return;
     }

   if (!data) return;

   if (ctx->klass->input_panel_imdata_set)
     ctx->klass->input_panel_imdata_set(ctx, data, len);
}

/**
 * Get the specific data of the current active input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param data The specific data to be got from the input panel
 * @param len The length of data
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_input_panel_imdata_get(Ecore_IMF_Context *ctx, void *data, int *len)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_imdata_get");
        return;
     }

   if (!data) return;

   if (ctx->klass->input_panel_imdata_get)
     ctx->klass->input_panel_imdata_get(ctx, data, len);
}

/**
 * Set the "return" key type. This type is used to set string or icon on the "return" key of the input panel.
 *
 * An input panel displays the string or icon associated with this type
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param return_key_type The type of "return" key on the input panel
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_input_panel_return_key_type_set(Ecore_IMF_Context *ctx, Ecore_IMF_Input_Panel_Return_Key_Type return_key_type)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_return_key_type_set");
        return;
     }

   ctx->input_panel_return_key_type = return_key_type;
   if (ctx->klass->input_panel_return_key_type_set) ctx->klass->input_panel_return_key_type_set(ctx, return_key_type);
}

/**
 * Get the "return" key type.
 *
 * @see ecore_imf_context_input_panel_return_key_type_set() for more details
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return The type of "return" key on the input panel
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI Ecore_IMF_Input_Panel_Return_Key_Type
ecore_imf_context_input_panel_return_key_type_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_return_key_type_get");
        return EINA_FALSE;
     }

   return ctx->input_panel_return_key_type;
}

/**
 * Set the return key on the input panel to be disabled.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param disabled The state
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_input_panel_return_key_disabled_set(Ecore_IMF_Context *ctx, Eina_Bool disabled)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_return_key_disabled_set");
        return;
     }

   ctx->input_panel_return_key_disabled = disabled;
   if (ctx->klass->input_panel_return_key_disabled_set) ctx->klass->input_panel_return_key_disabled_set(ctx, disabled);
}

/**
 * Get whether the return key on the input panel should be disabled or not.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return EINA_TRUE if it should be disabled
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI Eina_Bool
ecore_imf_context_input_panel_return_key_disabled_get(Ecore_IMF_Context *ctx)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_return_key_disabled_get");
        return EINA_FALSE;
     }

   return ctx->input_panel_return_key_disabled;
}

/**
 * Get the position of the current active input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param x top-left x co-ordinate of the input panel
 * @param y top-left y co-ordinate of the input panel
 * @param w width of the input panel
 * @param h height of the input panel
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI void
ecore_imf_context_input_panel_geometry_get(Ecore_IMF_Context *ctx, int *x, int *y, int *w, int *h)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_geometry_get");
        return;
     }

   if (ctx->klass->input_panel_geometry_get)
     ctx->klass->input_panel_geometry_get(ctx, x, y, w, h);
}

/**
 * Set the private key of input panel.
 * Since the second parameter of this API requires a layout index, so before using this API 
 * application has to set a specific layout and that layout index should 
 * be passed in the 2nd argument of this API.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param layout_index index of layout page to be set
 * @param img_path the path of image file to be set
 * @param key_index index of key to be set
 * @param label text label to be appeared on private key
 * @param value value of key, If NULL it will use original value of key
 * @ingroup Ecore_IMF_Context_Group
 */
EINA_DEPRECATED EAPI void
ecore_imf_context_input_panel_private_key_set(Ecore_IMF_Context *ctx, int layout_index, int key_index, const char *img_path, const char *label, int key_value, const char *key_string)
{
   // will be deleted
}

/**
 * Get the list of private key.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Return the list of private key
 * @ingroup Ecore_IMF_Context_Group
 */
EINA_DEPRECATED EAPI Eina_List *
ecore_imf_context_input_panel_private_key_list_get(Ecore_IMF_Context *ctx)
{
   // will be deleted
   return NULL;
}

/**
 * Set the key to be disabled.
 * Since the second parameter of this API requires a layout index, so before using this API application has to set a specific layout and that layout index 
 * should be passed in the 2nd argument of this API.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param layout_index index of layout page to be set
 * @param key_index index of key to be set
 * @param disabled The state
 * @ingroup Ecore_IMF_Context_Group
 */
EINA_DEPRECATED EAPI void
ecore_imf_context_input_panel_key_disabled_set(Ecore_IMF_Context *ctx, int layout_index, int key_index, Eina_Bool disabled)
{
   // will be deleted
}

/**
 * Get the list of disabled keys.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @return Return the list of private key
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EINA_DEPRECATED EAPI Eina_List *
ecore_imf_context_input_panel_key_disabled_list_get(Ecore_IMF_Context *ctx)
{
   // will be deleted
   return NULL;
}

/**
 * Get state of current active input panel.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param The state of input panel.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.2.0
 */
EAPI Ecore_IMF_Input_Panel_State
ecore_imf_context_input_panel_state_get(Ecore_IMF_Context *ctx)
{
   Ecore_IMF_Input_Panel_State state = ECORE_IMF_INPUT_PANEL_STATE_HIDE;
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_state_get");
        return ECORE_IMF_INPUT_PANEL_STATE_HIDE;
     }

   if (ctx->klass->input_panel_state_get)
     state = ctx->klass->input_panel_state_get(ctx);

   return state;
}

/**
 * Register a callback function which will be called if there is change in input panel state,language,mode etc.
 * In order to deregister the callback function 
 * Use @ref ecore_imf_context_input_panel_event_callback_del.
 *
 * @param ctx An #Ecore_IMF_Context
 * @param type event type
 * @param func the callback function
 * @param data application-input panel specific data.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_event_callback_add(Ecore_IMF_Context *ctx,
                                                 Ecore_IMF_Input_Panel_Event type,
                                                 void (*func) (void *data, Ecore_IMF_Context *ctx, int value),
                                                 const void *data)
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_event_callback_add");
        return;
     }

   if (ctx->klass->input_panel_event_callback_add)
     ctx->klass->input_panel_event_callback_add(ctx, type, func, (void *)data);
}

/**
 * Unregister a callback function which will be called if there is change in input panel state, language, mode etc.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param func the callback function
 * @param data application-input panel specific data.
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EAPI void
ecore_imf_context_input_panel_event_callback_del(Ecore_IMF_Context *ctx,
                                                 Ecore_IMF_Input_Panel_Event type,
                                                 void (*func) (void *data, Ecore_IMF_Context *ctx, int value))
{
   if (!ECORE_MAGIC_CHECK(ctx, ECORE_MAGIC_CONTEXT))
     {
        ECORE_MAGIC_FAIL(ctx, ECORE_MAGIC_CONTEXT,
                         "ecore_imf_context_input_panel_event_callback_del");
        return;
     }

   if (ctx->klass->input_panel_event_callback_del)
     ctx->klass->input_panel_event_callback_del(ctx, type, func);
}

/**
 * Move the soft keyboard to the new position.
 *
 * @param ctx An #Ecore_IMF_Context.
 * @param x X position to move the input panel to
 * @param y Y position to move the input panel to
 * @ingroup Ecore_IMF_Context_Group
 * @since 1.1.0
 */
EINA_DEPRECATED EAPI void
ecore_imf_context_input_panel_move(Ecore_IMF_Context *ctx __UNUSED__, int x __UNUSED__, int y __UNUSED__)
{
   // will be deprecated
}
