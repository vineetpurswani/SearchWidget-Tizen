#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "elm_search.h"

#define MY_CLASS_NAME_LEGACY "elm_search"

static char *
strlwr(char * foo)
{
	int i;
	for(i = 0; foo[i]; i++){
	  foo[i] = tolower(foo[i]);
	}
	return foo;
}

EOLIAN static Eo_Base *
_elm_search_eo_base_constructor(Eo *obj, Elm_Search_Data *pd)
{
	LOGFUNC;
	obj = eo_constructor(eo_super(obj, ELM_SEARCH_CLASS));
	evas_obj_type_set(obj, MY_CLASS_NAME_LEGACY);
	evas_obj_smart_callbacks_descriptions_set(obj, _smart_callbacks);
	return obj;
}

EOLIAN static void
_elm_search_eo_base_destructor(Eo *obj, Elm_Search_Data *pd)
{
	LOGFUNC;
	eo_destructor(eo_super(obj, ELM_SEARCH_CLASS));
}

EOLIAN static void
_hover_end(Eo *obj)
{
	LOGFUNC;
	ELM_SEARCH_DATA_GET(obj, sd);
	ELM_SAFE_FREE(sd->hover, evas_object_del);
}

EOLIAN static void
_on_hover_clicked(void *d, Evas_Object *obj, void *ei) 
{
	LOGFUNC;
	_hover_end(d);
}

static Evas_Object *
_widget_min_set(Evas_Object *obj, Evas_Object *parent, Evas_Coord w, Evas_Coord h)
{
	LOGFUNC;
	Evas_Object *table, *rect;
	table = elm_table_add(parent);
	rect = evas_object_rectangle_add(evas_object_evas_get(table));
	evas_object_size_hint_min_set(rect, w, h);
	evas_object_color_set(rect, 0, 0, 0, 0);
	evas_object_size_hint_align_set(rect, EVAS_HINT_FILL,
								   EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);
	elm_table_pack(table, rect, 0, 0, 1, 1);
	evas_object_size_hint_align_set(obj, EVAS_HINT_FILL,
								   EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(obj, EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);

	elm_table_pack(table, obj, 0, 0, 1, 1);
	evas_object_show(rect);
	evas_object_show(obj);

	return table;
}

static Evas_Object *
_create_list(Evas_Object *p)
{
	LOGFUNC;
	Eina_List *l = NULL;
	void *list_data = NULL;

	ELM_SEARCH_DATA_GET(p, pd);

	Evas_Object *list;

	list = elm_list_add(p);
	evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	if (elm_obj_entry_is_empty(pd->entry) == EINA_TRUE) 
		return list;

	char *entry_text = strdup(elm_entry_entry_get(pd->entry));
	EINA_LIST_FOREACH(pd->item_list, l, list_data)
	{
		Search_List_Item *item = list_data;
		if (strstr(strlwr(item->label), strlwr(entry_text))){
			elm_list_item_append(list, item->label/* label */, NULL/* icon */, 
				NULL /* end */, item->func /* func */, item->func_data /* func data */);
		}
	}

	return list;
}

// static Evas_Object *
// _create_gl(Evas_Object *p)
// {
// 	Eina_List *l = NULL;
// 	void *list_data = NULL;

// 	ELM_SEARCH_DATA_GET(p, pd);
// 	Evas_Object *gl;

// 	if (pd->view_mode == ELM_SEARCH_LIST)
// 		gl = elm_list_add(p);
// 	else
// 	{
// 		gl = elm_gengrid_add(p);
// 		elm_gengrid_item_size_set(gl, ELM_SCALE_SIZE(40),
// 		ELM_SCALE_SIZE(40));
// 	}
// 	evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
// 	evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

// 	if (elm_obj_entry_is_empty(pd->entry) == EINA_TRUE) 
// 		return gl;

// 	char *entry_text = strdup(elm_entry_entry_get(pd->entry));
// 	EINA_LIST_FOREACH(pd->item_list, l, list_data)
// 	{
// 		Search_List_Item *item = list_data;
// 		if (strstr(strlwr(item->label), strlwr(entry_text))) {
// 			if (pd->view_mode == ELM_SEARCH_LIST)
// 			{
// 				elm_list_item_append(gl, item->label/* label */, NULL/* icon */, 
// 					NULL /* end */, item->func, item->func_data);
// 			}
// 			else
// 			{
// 				elm_gengrid_item_append(gl, item->itc, item->data, 
// 					item->func, item->func_data);
// 			}
// 		}
// 	}

// 	return gl;
// }

static Evas_Object *
_create_hvlist(Evas_Object *p)
{
	LOGFUNC;
	Evas_Object *hv, *list;
	char style[256];

	ELM_SEARCH_DATA_GET(p, pd);

	hv = elm_hover_add(p);
	elm_widget_sub_object_add(p, hv);
	snprintf(style, sizeof(style), "search/%s",
			elm_widget_style_get(p));
	elm_object_style_set(hv, style);

	pd->list = list = _create_list(p);
	// pd->list = list = _create_gl(p);

	pd->table = _widget_min_set(list, hv, 100, 100);
	elm_object_part_content_set(hv, "bottom", pd->table);
	evas_object_show(pd->table);

	return hv;
}

static void
_activate_hover(Evas_Object *obj)
{
	LOGFUNC;
	ELM_SEARCH_DATA_GET(obj, pd);

	_hover_end(obj);
	pd->hover = _create_hvlist(obj);
	elm_hover_target_set(pd->hover, pd->entry);
	evas_object_smart_callback_add(pd->hover, "clicked", _on_hover_clicked, obj); 
	evas_object_show(pd->hover);
	elm_object_focus_set(pd->entry, EINA_TRUE);
	evas_object_smart_callback_call(obj, SIG_EXPANDED, NULL);
}

static void
_on_clicked(void *d, Evas_Object *o, void *ei)
{
	LOGFUNC;
	_activate_hover(d);
}


EOLIAN static void
_elm_search_evas_object_smart_add(Eo *obj, Elm_Search_Data *pd)
{
	LOGFUNC;
	evas_obj_smart_add(eo_super(obj, ELM_SEARCH_CLASS));
	elm_widget_sub_object_parent_add(obj);

	if(!elm_layout_theme_set(obj, "search", "base", elm_widget_style_get(obj)))
		printf("Failed to set layout!\n");

	ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, );

	if (edje_object_part_exists(wd->resize_obj, "elm.swallow.button"))
	{
		Eo *btn = elm_button_add(obj);
		
		Eo *icon = elm_icon_add(obj);
		elm_icon_standard_set(icon, "search");
		elm_object_part_content_set(btn, "icon", icon);

		pd->btn = btn;
		evas_object_smart_callback_add(pd->btn, "clicked", _on_clicked, obj);
		edje_object_part_swallow(wd->resize_obj, "elm.swallow.button", pd->btn);

		printf("Loading button.. \n");
	}
	if (edje_object_part_exists(wd->resize_obj, "elm.swallow.entry"))
	{
		Eo *entry = elm_entry_add(obj);
		elm_entry_anchor_hover_parent_set(entry, pd->hover);
		elm_entry_single_line_set(entry, EINA_TRUE);
		evas_object_smart_callback_add(entry, "activated", _on_clicked, obj);
		
		pd->entry = entry;
		edje_object_part_swallow(wd->resize_obj, "elm.swallow.entry", pd->entry);
		printf("Loading entry.. \n");
	}
	else
	{
		printf("FAILED TO SET theme... abort abort!!\n");
	}

}

EOLIAN static void
_elm_search_evas_object_smart_del(Eo *obj, Elm_Search_Data *pd)
{
	LOGFUNC;
	ELM_SAFE_FREE(pd->entry, evas_object_del);
	ELM_SAFE_FREE(pd->btn, evas_object_del);
	// ELM_SAFE_FREE(pd->hover, evas_object_del);

	evas_obj_smart_del(eo_super(obj, ELM_SEARCH_CLASS));
	exit(0);
}

EOLIAN static void
_elm_search_class_constructor(Eo_Class *klass)
{
	LOGFUNC;
	 evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}


EOLIAN static void
_elm_search_item_append(Eo *obj, Elm_Search_Data *pd, const char *label, Evas_Smart_Cb func, const void *func_data)
{
	LOGFUNC;
	
	Eina_List *l = NULL;
	void *list_data = NULL;
	EINA_LIST_FOREACH(pd->item_list, l, list_data)
	{
		Search_List_Item *item = list_data;
		if (strcmp(item->label, label) == 0){
			CRI("Label '%s' already exists", label);
			return;
		}
	}

	Search_List_Item *item = malloc(sizeof(Search_List_Item));
	item->label = strdup(label);
	item->func = func;
	item->func_data = func_data;
	pd->item_list = eina_list_append(pd->item_list, item);

	elm_object_focus_set(pd->entry, EINA_TRUE);	
}

#include "elm_search.eo.c"
