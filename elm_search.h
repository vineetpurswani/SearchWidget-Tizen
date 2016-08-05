#define ELM_INTERNAL_API_ARGESFSDFEFC
#define EFL_EO_API_SUPPORT
#include <Elementary.h>
#include <Evas.h>
#include <canvas/evas_object_smart.eo.h>
#include <elm_widget.h>
#include <elm_layout.h>
#include <elm_widget_layout.h>
#include "elm_search.eo.h"

#define LOGFUNC printf("%s\n", __func__);

#define ELM_SAFE_FREE(obj, func) \
	obj != NULL ? (func(obj), obj = NULL) : NULL


static const char SIG_SELECTED[] = "selected";
static const char SIG_DISMISSED[] = "dismissed";
static const char SIG_EXPANDED[] = "expanded";
static const char SIG_ITEM_FOCUSED[] = "item,focused";
static const char SIG_ITEM_UNFOCUSED[] = "item,unfocused";

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
	{SIG_SELECTED, ""},
	{SIG_DISMISSED, ""},
	{SIG_EXPANDED, ""},
	{SIG_ITEM_FOCUSED, ""},
	{SIG_ITEM_UNFOCUSED, ""},
	//{SIG_WIDGET_LANG_CHANGED, ""}, /**< handled by elm_widget */
	//{SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
	{NULL, NULL}
};

// typedef enum
// {
//    ELM_SEARCH_LIST,
//    ELM_SEARCH_GRID
// } Elm_Search_View;

typedef struct
{
	Eo *entry;
	Eo *btn;
	Eo *hover; 
	Eo *table; 
	Eo *list;
	Eina_List *item_list;
	// Eo *grid;
	// Elm_Search_View view_mode;
} Elm_Search_Data;

typedef struct
{
	// const Evas_Object *itc;
	// const void *data;
	char *label;
	Evas_Smart_Cb func;
	const void *func_data;
} Search_List_Item;

#define CRI printf

#define ELM_SEARCH_DATA_GET(o, sd) \
	Elm_Search_Data * sd = eo_data_scope_get(o, ELM_SEARCH_CLASS)

// #define ELM_SEARCH_DATA_GET_OR_RETURN(o, ptr) \
// 	Elm_Search_Data *ptr; \
// 	ptr = eo_data_scope_get(o, ELM_SEARCH_CLASS); \
// 	if (EINA_UNLIKELY(!ptr)) \
// 	{ \
// 		CRI("no search data for object %p (%s)", \
// 			o, evas_object_type_get(o)); \
// 		return NULL; \
// 	}
