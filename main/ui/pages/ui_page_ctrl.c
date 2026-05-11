#include "ui_page_ctrl.h"
#include "../core/ui_styles.h"
#include "../services/ui_i18n.h"
#include "../services/ui_icons.h"
#include "../model/garden_model.h"
#include "../fonts/ui_font.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    lv_obj_t *sw_corridor, *sw_strip, *sw_fan, *sw_auto;
} ctrl_ctx_t;

/* --- Scene handlers --- */
static void on_scene_morning(lv_event_t *e)
{
    (void)e;
    printf("[CTRL] Scene: Morning\n");
    garden_model_set_light("corridor_light", true);
    garden_model_set_light("strip_light", false);
    garden_model_set_fan(false);
}

static void on_scene_night(lv_event_t *e)
{
    (void)e;
    printf("[CTRL] Scene: Night\n");
    garden_model_set_light("corridor_light", false);
    garden_model_set_light("strip_light", true);
    garden_model_set_fan(false);
}

static void on_scene_away(lv_event_t *e)
{
    (void)e;
    printf("[CTRL] Scene: Away\n");
    garden_model_set_light("corridor_light", false);
    garden_model_set_light("strip_light", false);
    garden_model_set_fan(false);
}

/* --- Device switch handlers --- */
static void on_sw_corridor(lv_event_t *e)
{
    bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    printf("[CTRL] Corridor Light -> %s\n", on ? "ON" : "OFF");
    garden_model_set_light("corridor_light", on);
}

static void on_sw_strip(lv_event_t *e)
{
    bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    printf("[CTRL] Strip Light -> %s\n", on ? "ON" : "OFF");
    garden_model_set_light("strip_light", on);
}

static void on_sw_fan(lv_event_t *e)
{
    bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    printf("[CTRL] Fan -> %s\n", on ? "ON" : "OFF");
    garden_model_set_fan(on);
}

static void on_sw_auto(lv_event_t *e)
{
    bool on = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    printf("[CTRL] Auto Mode -> %s\n", on ? "ON" : "OFF");
    garden_model_set_auto(on);
}

static void page_ctrl_delete(lv_event_t *e)
{
    ctrl_ctx_t *ctx = (ctrl_ctx_t *)lv_event_get_user_data(e);
    if (!ctx) return;
    lv_free(ctx);
}

/* --- Scene button helper --- */
static lv_obj_t *create_scene_btn(lv_obj_t *parent, const char *icon, const char *label,
    lv_color_t color, lv_color_t icon_bg, lv_event_cb_t cb)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_flex_grow(btn, 1);
    lv_obj_set_height(btn, 80);
    lv_obj_set_style_bg_color(btn, UI_COLOR_CARD, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_70, LV_STATE_PRESSED);
    lv_obj_set_style_border_width(btn, 0, 0);
    lv_obj_set_style_radius(btn, UI_CARD_RADIUS, 0);
    lv_obj_set_style_pad_all(btn, 14, 0);
    lv_obj_set_style_shadow_width(btn, 4, 0);
    lv_obj_set_style_shadow_ofs_y(btn, 2, 0);
    lv_obj_set_style_shadow_opa(btn, LV_OPA_10, 0);
    lv_obj_set_style_shadow_color(btn, lv_color_black(), 0);
    lv_obj_set_layout(btn, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(btn, 8, 0);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);

    /* Icon in colored circle */
    lv_obj_t *icon_bg_obj = lv_obj_create(btn);
    lv_obj_remove_style_all(icon_bg_obj);
    lv_obj_set_size(icon_bg_obj, 36, 36);
    lv_obj_set_style_bg_color(icon_bg_obj, icon_bg, 0);
    lv_obj_set_style_bg_opa(icon_bg_obj, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(icon_bg_obj, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_layout(icon_bg_obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(icon_bg_obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(icon_bg_obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon_lbl = lv_label_create(icon_bg_obj);
    lv_label_set_text(icon_lbl, icon);
    lv_obj_set_style_text_font(icon_lbl, ui_font_icon(18), 0);
    lv_obj_set_style_text_color(icon_lbl, color, 0);

    ui_create_label(btn, label, ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT);

    return btn;
}

/* --- Device card helper --- */
static lv_obj_t *create_device_card(lv_obj_t *parent, const char *icon, const char *label,
    lv_color_t icon_color, lv_color_t icon_bg, lv_color_t sw_color, lv_event_cb_t sw_cb, lv_obj_t **out_sw)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_remove_style_all(card);
    lv_obj_set_width(card, lv_pct(48));
    lv_obj_set_height(card, 76);
    lv_obj_set_style_bg_color(card, UI_COLOR_CARD, 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, UI_CARD_RADIUS, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_pad_hor(card, 14, 0);
    lv_obj_set_style_pad_ver(card, 14, 0);
    lv_obj_set_layout(card, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    ui_apply_card_shadow(card);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *left = lv_obj_create(card);
    lv_obj_remove_style_all(left);
    lv_obj_set_style_bg_opa(left, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(left, 0, 0);
    lv_obj_set_style_pad_all(left, 0, 0);
    lv_obj_set_layout(left, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(left, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(left, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(left, 10, 0);
    lv_obj_clear_flag(left, LV_OBJ_FLAG_SCROLLABLE);

    /* Icon in colored rounded square */
    lv_obj_t *icon_bg_obj = lv_obj_create(left);
    lv_obj_remove_style_all(icon_bg_obj);
    lv_obj_set_size(icon_bg_obj, 36, 36);
    lv_obj_set_style_bg_color(icon_bg_obj, icon_bg, 0);
    lv_obj_set_style_bg_opa(icon_bg_obj, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(icon_bg_obj, 10, 0);
    lv_obj_set_layout(icon_bg_obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(icon_bg_obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(icon_bg_obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon_lbl = lv_label_create(icon_bg_obj);
    lv_label_set_text(icon_lbl, icon);
    lv_obj_set_style_text_font(icon_lbl, ui_font_icon(18), 0);
    lv_obj_set_style_text_color(icon_lbl, icon_color, 0);

    ui_create_label(left, label, ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT);

    *out_sw = ui_create_switch(card, sw_color);
    lv_obj_add_event_cb(*out_sw, sw_cb, LV_EVENT_VALUE_CHANGED, NULL);

    return card;
}

lv_obj_t *ui_page_ctrl_create(lv_obj_t *parent)
{
    ctrl_ctx_t *ctx = lv_malloc(sizeof(ctrl_ctx_t));
    memset(ctx, 0, sizeof(*ctx));

    lv_obj_t *page = lv_obj_create(parent);
    lv_obj_remove_style_all(page);
    lv_obj_set_width(page, lv_pct(100));
    lv_obj_set_height(page, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(page, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(page, 0, 0);
    lv_obj_set_style_pad_hor(page, 20, 0);
    lv_obj_set_style_pad_ver(page, 16, 0);
    lv_obj_set_layout(page, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(page, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(page, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(page, 16, 0);
    lv_obj_add_event_cb(page, page_ctrl_delete, LV_EVENT_DELETE, ctx);

    /* Title row with icon */
    lv_obj_t *title_row = lv_obj_create(page);
    lv_obj_remove_style_all(title_row);
    lv_obj_set_size(title_row, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(title_row, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(title_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(title_row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(title_row, 10, 0);
    lv_obj_clear_flag(title_row, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title_icon_bg = lv_obj_create(title_row);
    lv_obj_remove_style_all(title_icon_bg);
    lv_obj_set_size(title_icon_bg, 28, 28);
    lv_obj_set_style_bg_color(title_icon_bg, lv_color_hex(0xF3E5F5), 0);
    lv_obj_set_style_bg_opa(title_icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(title_icon_bg, 8, 0);
    lv_obj_set_layout(title_icon_bg, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(title_icon_bg, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(title_icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title_icon = lv_label_create(title_icon_bg);
    lv_label_set_text(title_icon, ICON_CONTROL);
    lv_obj_set_style_text_font(title_icon, ui_font_icon(16), 0);
    lv_obj_set_style_text_color(title_icon, UI_COLOR_PURPLE, 0);

    ui_create_label(title_row, ui_i18n_get(UI_KEY_CTRL_TITLE),
        ui_i18n_font(UI_FONT_24), UI_COLOR_TEXT);

    /* Scene mode buttons */
    lv_obj_t *scene_row = lv_obj_create(page);
    lv_obj_remove_style_all(scene_row);
    lv_obj_set_width(scene_row, lv_pct(100));
    lv_obj_set_height(scene_row, LV_SIZE_CONTENT);
    lv_obj_set_layout(scene_row, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(scene_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(scene_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(scene_row, 10, 0);
    lv_obj_clear_flag(scene_row, LV_OBJ_FLAG_SCROLLABLE);

    create_scene_btn(scene_row, ICON_SUN, ui_i18n_get(UI_KEY_CTRL_MORNING),
        UI_COLOR_ORANGE, lv_color_hex(0xFFF3E0), on_scene_morning);
    create_scene_btn(scene_row, ICON_MOON, ui_i18n_get(UI_KEY_CTRL_NIGHT),
        UI_COLOR_PURPLE, lv_color_hex(0xF3E5F5), on_scene_night);
    create_scene_btn(scene_row, ICON_HOME, ui_i18n_get(UI_KEY_CTRL_AWAY),
        UI_COLOR_TEXT_SEC, lv_color_hex(0xF5F5F5), on_scene_away);

    /* Section: Device Control */
    ui_create_label(page, ui_i18n_get(UI_KEY_GARDEN_DEVICES),
        ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT_SEC);

    lv_obj_t *dev_grid = lv_obj_create(page);
    lv_obj_remove_style_all(dev_grid);
    lv_obj_set_width(dev_grid, lv_pct(100));
    lv_obj_set_height(dev_grid, LV_SIZE_CONTENT);
    lv_obj_set_layout(dev_grid, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(dev_grid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(dev_grid, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(dev_grid, 12, 0);
    lv_obj_set_style_pad_column(dev_grid, 0, 0);
    lv_obj_clear_flag(dev_grid, LV_OBJ_FLAG_SCROLLABLE);

    create_device_card(dev_grid, ICON_LIGHTBULB, ui_i18n_get(UI_KEY_GARDEN_CORRIDOR),
        UI_COLOR_ORANGE, lv_color_hex(0xFFF3E0), UI_COLOR_ORANGE, on_sw_corridor, &ctx->sw_corridor);
    create_device_card(dev_grid, ICON_LIGHTBULB, ui_i18n_get(UI_KEY_GARDEN_STRIP),
        UI_COLOR_PURPLE, lv_color_hex(0xF3E5F5), UI_COLOR_PURPLE, on_sw_strip, &ctx->sw_strip);
    create_device_card(dev_grid, ICON_AIR, ui_i18n_get(UI_KEY_GARDEN_FAN),
        UI_COLOR_BLUE, lv_color_hex(0xE3F2FD), UI_COLOR_BLUE, on_sw_fan, &ctx->sw_fan);
    create_device_card(dev_grid, ICON_SETTINGS, ui_i18n_get(UI_KEY_GARDEN_AUTO),
        UI_COLOR_GREEN, lv_color_hex(0xE8F5E9), UI_COLOR_GREEN, on_sw_auto, &ctx->sw_auto);

    printf("[CTRL] Control page created\n");
    return page;
}
