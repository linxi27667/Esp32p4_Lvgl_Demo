#include "ui_styles.h"
#include "../fonts/ui_font.h"

lv_obj_t *ui_create_label(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_color_t color)
{
    lv_obj_t *lbl = lv_label_create(parent);
    if (!lbl) return NULL;
    lv_label_set_text(lbl, text ? text : "");
    if (font) lv_obj_set_style_text_font(lbl, font, 0);
    lv_obj_set_style_text_color(lbl, color, 0);
    return lbl;
}

lv_obj_t *ui_create_icon(lv_obj_t *parent, const char *icon, const lv_font_t *font, lv_color_t color)
{
    lv_obj_t *lbl = lv_label_create(parent);
    if (!lbl) return NULL;
    lv_label_set_text(lbl, icon ? icon : "");
    if (font) lv_obj_set_style_text_font(lbl, font, 0);
    lv_obj_set_style_text_color(lbl, color, 0);
    return lbl;
}

lv_obj_t *ui_create_dot(lv_obj_t *parent, lv_color_t color, int size)
{
    lv_obj_t *dot = lv_obj_create(parent);
    lv_obj_remove_style_all(dot);
    lv_obj_set_size(dot, size, size);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, color, 0);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(dot, 0, 0);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
    return dot;
}

lv_obj_t *ui_create_switch(lv_obj_t *parent, lv_color_t active_color)
{
    lv_obj_t *sw = lv_switch_create(parent);
    lv_obj_set_size(sw, 56, 30);

    /* Track: OFF state */
    lv_obj_set_style_bg_color(sw, UI_COLOR_BORDER, 0);
    lv_obj_set_style_bg_opa(sw, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(sw, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(sw, 0, 0);

    /* Track: ON state (CHECKED) */
    lv_obj_set_style_bg_color(sw, active_color, LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(sw, LV_OPA_COVER, LV_STATE_CHECKED);

    /* Indicator (inner filled area) */
    lv_obj_set_style_bg_opa(sw, LV_OPA_TRANSP, LV_PART_INDICATOR);
    lv_obj_set_style_radius(sw, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);

    /* Knob: white circle */
    lv_obj_set_style_bg_color(sw, lv_color_white(), LV_PART_KNOB);
    lv_obj_set_style_bg_opa(sw, LV_OPA_COVER, LV_PART_KNOB);
    lv_obj_set_style_radius(sw, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    lv_obj_set_style_border_width(sw, 0, LV_PART_KNOB);
    lv_obj_set_style_width(sw, 26, LV_PART_KNOB);
    lv_obj_set_style_height(sw, 26, LV_PART_KNOB);
    lv_obj_set_style_shadow_width(sw, 2, LV_PART_KNOB);
    lv_obj_set_style_shadow_opa(sw, LV_OPA_20, LV_PART_KNOB);

    return sw;
}

lv_obj_t *ui_create_card(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_remove_style_all(card);
    lv_obj_set_style_bg_color(card, UI_COLOR_CARD, 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, UI_CARD_RADIUS, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_pad_all(card, 16, 0);
    ui_apply_card_shadow(card);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_layout(card, LV_LAYOUT_FLEX);
    return card;
}

void ui_apply_card_shadow(lv_obj_t *obj)
{
    lv_obj_set_style_shadow_width(obj, 8, 0);
    lv_obj_set_style_shadow_ofs_x(obj, 0, 0);
    lv_obj_set_style_shadow_ofs_y(obj, 2, 0);
    lv_obj_set_style_shadow_opa(obj, LV_OPA_10, 0);
    lv_obj_set_style_shadow_color(obj, lv_color_black(), 0);
}

void ui_styles_init(void)
{
}
