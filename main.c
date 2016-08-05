#define EFL_BETA_API_SUPPORT
#include <Elementary.h>
#include <Eo.h>
#include "elm_search.eo.h"

static Elm_Genlist_Item_Class *itc1;

static char *
gl_text_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part EINA_UNUSED)
{
   return strdup("Item");
}

static Evas_Object *
gl_content_get(void *data EINA_UNUSED,
			   Evas_Object *obj, const char *part)
{
   Evas_Object *ic = elm_icon_add(obj);
   evas_object_size_hint_align_set(ic, EVAS_HINT_FILL,
								   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(ic, EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);
   elm_icon_standard_set(ic, "home");
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
   return ic;
}

static void
gl_sel(void *data, Evas_Object *obj,
	   void *event_info)
{
	Elm_Object_Item *it = event_info;
	// unsigned int index;

	// index = elm_genlist_item_index_get(it);

	// printf("item pointer [%p], index [%d]\n", it, index);
	//set the button/entry text.
	const char *text;
	text = eina_stringshare_add(elm_object_item_text_get(it));
	printf("Item selected has label: %s\n", text);
	elm_object_text_set(data, text);
	// eo_do(data, elm_obj_combobox_hover_end());
	eina_stringshare_del(text);

	// printf("Search box text: %s\n",
	// 	elm_object_text_get(data));
}

int main(int argc, char **argv)
{
	 elm_init(argc, argv);

	 Evas_Object *win;
	 elm_theme_overlay_add(NULL, "./search.edj");
	 //elm_config_item_select_on_focus_disabled_set(1);

	 win = elm_win_util_standard_add("Eo Smart Object", "Eo Smart Object");
	elm_win_focus_highlight_enabled_set(win, 1);
	elm_win_focus_highlight_animate_set(win, 1);
	elm_win_autodel_set(win, 1);

	Evas_Object *text = elm_entry_add(win);
	evas_object_resize(text, 100, 30);
	evas_object_move(text, 100, 250);
	evas_object_show(text);

	 Evas_Object *obj = eo_add(ELM_SEARCH_CLASS, win);
	 evas_object_resize(obj, 100, 30);
	 evas_object_move(obj, 100, 100);
	 evas_object_show(obj);

	int i = 0;
	for (; i < 10; ++i) {
		char label[20];
		sprintf(label, "hello%d", i);
		elm_search_item_append(obj, label, gl_sel, text);
	}

	 evas_object_resize(win, 400, 400);
	 evas_object_show(win);

	 elm_object_focus_set(obj, 1);

	 elm_run();
	 elm_shutdown();

	 return 0;
}
