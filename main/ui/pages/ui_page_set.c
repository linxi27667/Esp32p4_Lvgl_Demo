#include "ui_page_set.h"
#include "../core/ui_styles.h"
#include "../core/ui_events.h"
#include "../services/ui_i18n.h"
#include "../services/ui_icons.h"
#include "../model/garden_model.h"
#include "../fonts/ui_font.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    lv_obj_t *lbl_lang_value;
    lv_obj_t *lbl_wifi_status;
    lv_obj_t *lbl_model;
    lv_obj_t *lbl_reset;
} set_ctx_t;

static void on_lang_toggle(lv_event_t *e)
{
    (void)e;
    ui_lang_t cur = ui_i18n_get_lang();
    printf("[SET] Language toggle: %s -> %s\n",
        cur == UI_LANG_ZH ? "ZH" : "EN",
        cur == UI_LANG_ZH ? "EN" : "ZH");
    ui_i18n_set_lang(cur == UI_LANG_ZH ? UI_LANG_EN : UI_LANG_ZH);
}

static void refresh_text(set_ctx_t *ctx)
{
    if (!ctx) return;
    if (ctx->lbl_lang_value) lv_label_set_text(ctx->lbl_lang_value, ui_i18n_get(UI_KEY_SET_LANG_CURRENT));
    if (ctx->lbl_wifi_status) lv_label_set_text(ctx->lbl_wifi_status, ui_i18n_get(UI_KEY_SET_CONNECTED));
    if (ctx->lbl_model) lv_label_set_text(ctx->lbl_model, ui_i18n_get(UI_KEY_MODEL));
    if (ctx->lbl_reset) lv_label_set_text(ctx->lbl_reset, ui_i18n_get(UI_KEY_SET_RESET));
}

static void on_lang_changed_event(void *user_data)
{
    refresh_text((set_ctx_t *)user_data);
}

static void on_reset(lv_event_t *e)
{
    (void)e;
    printf("[SET] Reset all data\n");
    garden_model_init();
}

static void page_set_delete(lv_event_t *e)
{
    set_ctx_t *ctx = (set_ctx_t *)lv_event_get_user_data(e);
    if (ctx) {
        ui_event_unsubscribe(UI_EVENT_LANG_CHANGED, on_lang_changed_event);
        lv_free(ctx);
    }
}

/* --- iOS-style row card --- */
static lv_obj_t *create_row_card(lv_obj_t *parent, bool clickable)
{
    lv_obj_t *card;
    if (clickable) {
        card = lv_btn_create(parent);
        lv_obj_set_style_bg_opa(card, LV_OPA_80, LV_STATE_PRESSED);
    } else {
        card = lv_obj_create(parent);
        lv_obj_clear_flag(card, LV_OBJ_FLAG_CLICKABLE);
    }
    lv_obj_remove_style_all(card);
    lv_obj_set_width(card, lv_pct(100));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(card, UI_COLOR_CARD, 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, UI_CARD_RADIUS, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_pad_ver(card, 14, 0);
    lv_obj_set_style_pad_hor(card, 16, 0);
    lv_obj_set_layout(card, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    ui_apply_card_shadow(card);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    return card;
}

/* Helper: create icon in a colored rounded square */
static lv_obj_t *create_row_icon(lv_obj_t *parent, const char *icon, lv_color_t color, lv_color_t bg)
{
    lv_obj_t *left = lv_obj_create(parent);
    lv_obj_remove_style_all(left);
    lv_obj_set_style_bg_opa(left, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(left, 0, 0);
    lv_obj_set_style_pad_all(left, 0, 0);
    lv_obj_set_layout(left, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(left, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(left, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(left, 12, 0);
    lv_obj_clear_flag(left, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon_bg = lv_obj_create(left);
    lv_obj_remove_style_all(icon_bg);
    lv_obj_set_size(icon_bg, 32, 32);
    lv_obj_set_style_bg_color(icon_bg, bg, 0);
    lv_obj_set_style_bg_opa(icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(icon_bg, 8, 0);
    lv_obj_set_layout(icon_bg, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(icon_bg, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon_lbl = lv_label_create(icon_bg);
    lv_label_set_text(icon_lbl, icon);
    lv_obj_set_style_text_font(icon_lbl, ui_font_icon(16), 0);
    lv_obj_set_style_text_color(icon_lbl, color, 0);

    return left;
}

lv_obj_t *ui_page_set_create(lv_obj_t *parent)
{
    set_ctx_t *ctx = lv_malloc(sizeof(set_ctx_t));
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
    lv_obj_set_style_pad_row(page, 8, 0);
    lv_obj_add_event_cb(page, page_set_delete, LV_EVENT_DELETE, ctx);

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
    lv_obj_set_style_bg_color(title_icon_bg, lv_color_hex(0xECEFF1), 0);
    lv_obj_set_style_bg_opa(title_icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(title_icon_bg, 8, 0);
    lv_obj_set_layout(title_icon_bg, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(title_icon_bg, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(title_icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title_icon = lv_label_create(title_icon_bg);
    lv_label_set_text(title_icon, ICON_SETTINGS);
    lv_obj_set_style_text_font(title_icon, ui_font_icon(16), 0);
    lv_obj_set_style_text_color(title_icon, UI_COLOR_TEXT_SEC, 0);

    ui_create_label(title_row, ui_i18n_get(UI_KEY_SET_TITLE),
        ui_i18n_font(UI_FONT_24), UI_COLOR_TEXT);

    /* Language Row */
    lv_obj_t *lang_card = create_row_card(page, true);
    lv_obj_add_event_cb(lang_card, on_lang_toggle, LV_EVENT_CLICKED, NULL);

    lv_obj_t *lang_left = create_row_icon(lang_card, ICON_LANGUAGE,
        UI_COLOR_ACCENT, lv_color_hex(0xE3F2FD));
    ui_create_label(lang_left, ui_i18n_get(UI_KEY_SET_LANGUAGE), ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT);
    ctx->lbl_lang_value = ui_create_label(lang_card, ui_i18n_get(UI_KEY_SET_LANG_CURRENT),
        ui_i18n_font(UI_FONT_14), UI_COLOR_ACCENT);

    /* WiFi Row */
    lv_obj_t *wifi_card = create_row_card(page, false);

    lv_obj_t *wifi_left = create_row_icon(wifi_card, ICON_WIFI,
        UI_COLOR_GREEN, lv_color_hex(0xE8F5E9));
    ui_create_label(wifi_left, ui_i18n_get(UI_KEY_SET_WIFI), ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT);
    ctx->lbl_wifi_status = ui_create_label(wifi_card, ui_i18n_get(UI_KEY_SET_CONNECTED),
        ui_i18n_font(UI_FONT_14), UI_COLOR_GREEN);

    /* About Row */
    lv_obj_t *about_card = create_row_card(page, false);

    lv_obj_t *about_left = create_row_icon(about_card, ICON_INFO,
        UI_COLOR_TEXT_SEC, lv_color_hex(0xF5F5F5));
    ui_create_label(about_left, ui_i18n_get(UI_KEY_BRAND), ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT);
    ctx->lbl_model = ui_create_label(about_card, ui_i18n_get(UI_KEY_MODEL),
        ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT_SEC);

    /* Version Row */
    lv_obj_t *ver_card = create_row_card(page, false);

    lv_obj_t *ver_left = create_row_icon(ver_card, "\xEF\x81\xAE",
        UI_COLOR_PURPLE, lv_color_hex(0xF3E5F5));
    ui_create_label(ver_left, ui_i18n_get(UI_KEY_SET_VERSION), ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT);
    ui_create_label(ver_card, "v1.0.0", ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT_SEC);

    /* Reset Row */
    lv_obj_t *reset_card = create_row_card(page, true);
    lv_obj_add_event_cb(reset_card, on_reset, LV_EVENT_CLICKED, NULL);

    lv_obj_t *reset_left = create_row_icon(reset_card, ICON_TRASH,
        UI_COLOR_RED, lv_color_hex(0xFFEBEE));
    ctx->lbl_reset = ui_create_label(reset_left, ui_i18n_get(UI_KEY_SET_RESET),
        ui_i18n_font(UI_FONT_14), UI_COLOR_RED);

    ui_event_subscribe(UI_EVENT_LANG_CHANGED, on_lang_changed_event, ctx);

    printf("[SET] Settings page created\n");
    return page;
}
