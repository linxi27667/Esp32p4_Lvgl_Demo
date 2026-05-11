#include "ui_page_data.h"
#include "../core/ui_styles.h"
#include "../services/ui_i18n.h"
#include "../services/ui_icons.h"
#include "../model/garden_model.h"
#include "../fonts/ui_font.h"
#include <stdio.h>
#include <string.h>

#define CHART_POINTS 24

typedef struct {
    lv_timer_t *timer;
    lv_obj_t *lbl_soil, *lbl_light, *lbl_temp, *lbl_humidity;
    lv_obj_t *card_soil, *card_light, *card_temp, *card_humidity;
    lv_obj_t *chart;
    lv_chart_series_t *chart_ser;
    int32_t chart_buf[CHART_POINTS];
    int chart_idx;
    bool chart_wrapped;
    bool deleted;  /* Flag to prevent timer from accessing freed ctx */
} data_ctx_t;

static void update_data(lv_timer_t *timer)
{
    data_ctx_t *ctx = (data_ctx_t *)lv_timer_get_user_data(timer);
    if (!ctx || ctx->deleted) return;
    const garden_model_t *st = garden_model_get();
    char b[16];

    lv_snprintf(b, sizeof(b), "%d%%", st->soil);
    if (ctx->lbl_soil) lv_label_set_text(ctx->lbl_soil, b);

    lv_snprintf(b, sizeof(b), "%d%%", st->light);
    if (ctx->lbl_light) lv_label_set_text(ctx->lbl_light, b);

    lv_snprintf(b, sizeof(b), "%d\xC2\xB0""C", st->temperature);
    if (ctx->lbl_temp) lv_label_set_text(ctx->lbl_temp, b);

    lv_snprintf(b, sizeof(b), "%d%%", st->humidity);
    if (ctx->lbl_humidity) lv_label_set_text(ctx->lbl_humidity, b);

    if (ctx->chart && ctx->chart_ser) {
        int32_t val = (int32_t)st->humidity;
        ctx->chart_buf[ctx->chart_idx % CHART_POINTS] = val;
        ctx->chart_idx++;
        if (ctx->chart_idx >= CHART_POINTS) ctx->chart_wrapped = true;
        /* Prevent int overflow: reset index but preserve wrapped state */
        if (ctx->chart_idx > 1000000) {
            ctx->chart_idx = ctx->chart_idx % CHART_POINTS + CHART_POINTS;
        }
        lv_chart_set_next_value(ctx->chart, ctx->chart_ser, val);
    }
}

static void chart_draw_post(lv_event_t *e)
{
    lv_obj_t *chart = lv_event_get_target(e);
    data_ctx_t *ctx = (data_ctx_t *)lv_event_get_user_data(e);
    if (!ctx || ctx->deleted) return;

    int n = ctx->chart_wrapped ? CHART_POINTS : ctx->chart_idx;
    if (n < 2) return;

    lv_layer_t *layer = lv_event_get_layer(e);
    lv_area_t area;
    lv_obj_get_content_coords(chart, &area);

    int32_t h = lv_area_get_height(&area);
    int32_t w = lv_area_get_width(&area);
    int32_t x_step = w / (CHART_POINTS - 1);
    int32_t y_base = area.y2;
    int32_t y_range = 100;
    int idx0 = ctx->chart_wrapped ? (ctx->chart_idx % CHART_POINTS) : 0;

    lv_draw_triangle_dsc_t tri_dsc;
    lv_draw_triangle_dsc_init(&tri_dsc);
    tri_dsc.color = lv_color_hex(0x007AFF);
    tri_dsc.opa = LV_OPA_10;

    for (int i = 0; i < n - 1; i++) {
        int di1 = (idx0 + i) % CHART_POINTS;
        int di2 = (idx0 + i + 1) % CHART_POINTS;

        int32_t y1 = y_base - (ctx->chart_buf[di1] * h / y_range);
        int32_t y2 = y_base - (ctx->chart_buf[di2] * h / y_range);
        int32_t x1 = area.x1 + i * x_step;
        int32_t x2 = x1 + x_step;

        if (y1 < area.y1) y1 = area.y1;
        if (y2 < area.y1) y2 = area.y1;

        tri_dsc.p[0].x = x1; tri_dsc.p[0].y = y1;
        tri_dsc.p[1].x = x2; tri_dsc.p[1].y = y2;
        tri_dsc.p[2].x = x1; tri_dsc.p[2].y = y_base;
        lv_draw_triangle(layer, &tri_dsc);

        tri_dsc.p[0].x = x2; tri_dsc.p[0].y = y2;
        tri_dsc.p[1].x = x1; tri_dsc.p[1].y = y_base;
        tri_dsc.p[2].x = x2; tri_dsc.p[2].y = y_base;
        lv_draw_triangle(layer, &tri_dsc);
    }
}

static void page_data_delete(lv_event_t *e)
{
    data_ctx_t *ctx = (data_ctx_t *)lv_event_get_user_data(e);
    if (!ctx) return;
    ctx->deleted = true;  /* Mark first to stop timer from using stale pointers */
    if (ctx->timer) { lv_timer_delete(ctx->timer); ctx->timer = NULL; }
    lv_free(ctx);
}

static lv_obj_t *create_sensor_card(lv_obj_t *parent, const char *icon, const char *label,
    lv_color_t icon_color, lv_color_t accent_bg, const lv_font_t *big_font,
    lv_obj_t **out_value, lv_obj_t **out_card)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_remove_style_all(card);
    lv_obj_set_width(card, lv_pct(48));
    lv_obj_set_height(card, 140);
    lv_obj_set_style_bg_color(card, UI_COLOR_CARD, 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, UI_CARD_RADIUS, 0);
    lv_obj_set_style_border_width(card, 0, 0);
    lv_obj_set_style_pad_all(card, 16, 0);
    lv_obj_set_layout(card, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(card, 8, 0);
    ui_apply_card_shadow(card);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    /* Top row: icon + label */
    lv_obj_t *top = lv_obj_create(card);
    lv_obj_remove_style_all(top);
    lv_obj_set_size(top, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(top, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(top, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(top, 8, 0);
    lv_obj_clear_flag(top, LV_OBJ_FLAG_SCROLLABLE);

    /* Icon in a small colored circle */
    lv_obj_t *icon_bg = lv_obj_create(top);
    lv_obj_remove_style_all(icon_bg);
    lv_obj_set_size(icon_bg, 32, 32);
    lv_obj_set_style_bg_color(icon_bg, accent_bg, 0);
    lv_obj_set_style_bg_opa(icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(icon_bg, 10, 0);
    lv_obj_set_layout(icon_bg, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(icon_bg, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *icon_lbl = lv_label_create(icon_bg);
    lv_label_set_text(icon_lbl, icon);
    lv_obj_set_style_text_font(icon_lbl, ui_font_icon(16), 0);
    lv_obj_set_style_text_color(icon_lbl, icon_color, 0);

    ui_create_label(top, label, ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT_SEC);

    *out_value = ui_create_label(card, "--", big_font, UI_COLOR_TEXT);
    *out_card = card;

    return card;
}

lv_obj_t *ui_page_data_create(lv_obj_t *parent)
{
    data_ctx_t *ctx = lv_malloc(sizeof(data_ctx_t));
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
    lv_obj_add_event_cb(page, page_data_delete, LV_EVENT_DELETE, ctx);

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
    lv_obj_set_style_bg_color(title_icon_bg, lv_color_hex(0xE3F2FD), 0);
    lv_obj_set_style_bg_opa(title_icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(title_icon_bg, 8, 0);
    lv_obj_set_layout(title_icon_bg, LV_LAYOUT_FLEX);
    lv_obj_set_flex_align(title_icon_bg, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(title_icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title_icon = lv_label_create(title_icon_bg);
    lv_label_set_text(title_icon, ICON_CHART);
    lv_obj_set_style_text_font(title_icon, ui_font_icon(16), 0);
    lv_obj_set_style_text_color(title_icon, UI_COLOR_ACCENT, 0);

    ui_create_label(title_row, ui_i18n_get(UI_KEY_DATA_REALTIME),
        ui_i18n_font(UI_FONT_24), UI_COLOR_TEXT);

    /* 2x2 Sensor card grid */
    lv_obj_t *grid = lv_obj_create(page);
    lv_obj_remove_style_all(grid);
    lv_obj_set_width(grid, lv_pct(100));
    lv_obj_set_height(grid, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_layout(grid, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(grid, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(grid, 12, 0);
    lv_obj_clear_flag(grid, LV_OBJ_FLAG_SCROLLABLE);

    const lv_font_t *hero_font = ui_font_cn(36);

    create_sensor_card(grid, ICON_WATER, ui_i18n_get(UI_KEY_GARDEN_SOIL),
        UI_COLOR_BLUE, lv_color_hex(0xE3F2FD), hero_font, &ctx->lbl_soil, &ctx->card_soil);
    create_sensor_card(grid, ICON_SUN, ui_i18n_get(UI_KEY_GARDEN_LIGHT),
        UI_COLOR_ORANGE, lv_color_hex(0xFFF3E0), hero_font, &ctx->lbl_light, &ctx->card_light);
    create_sensor_card(grid, ICON_TEMP, ui_i18n_get(UI_KEY_DATA_TEMP),
        UI_COLOR_RED, lv_color_hex(0xFFEBEE), hero_font, &ctx->lbl_temp, &ctx->card_temp);
    create_sensor_card(grid, ICON_WATER, ui_i18n_get(UI_KEY_DATA_HUMIDITY),
        UI_COLOR_BLUE, lv_color_hex(0xE3F2FD), hero_font, &ctx->lbl_humidity, &ctx->card_humidity);

    /* Chart section label */
    ui_create_label(page, ui_i18n_get(UI_KEY_DATA_HUMIDITY),
        ui_i18n_font(UI_FONT_14), UI_COLOR_TEXT_SEC);

    /* Trend chart */
    lv_obj_t *chart = lv_chart_create(page);
    lv_obj_set_width(chart, lv_pct(100));
    lv_obj_set_height(chart, 180);
    lv_obj_set_style_bg_color(chart, UI_COLOR_CARD, 0);
    lv_obj_set_style_bg_opa(chart, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(chart, UI_CARD_RADIUS, 0);
    lv_obj_set_style_pad_all(chart, 16, 0);
    lv_obj_set_style_border_width(chart, 0, 0);
    ui_apply_card_shadow(chart);

    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(chart, CHART_POINTS);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_div_line_count(chart, 4, 0);

    lv_obj_set_style_pad_column(chart, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_row(chart, 0, LV_PART_ITEMS);
    lv_obj_set_style_line_color(chart, lv_color_hex(0xE5E5EA), LV_PART_MAIN);
    lv_obj_set_style_line_width(chart, 1, LV_PART_MAIN);
    lv_obj_set_style_line_opa(chart, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_line_color(chart, UI_COLOR_ACCENT, LV_PART_ITEMS);
    lv_obj_set_style_line_width(chart, 3, LV_PART_ITEMS);
    lv_obj_set_style_line_rounded(chart, true, LV_PART_ITEMS);
    lv_obj_set_style_bg_color(chart, UI_COLOR_ACCENT, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(chart, LV_OPA_50, LV_PART_ITEMS);

    ctx->chart_ser = lv_chart_add_series(chart, UI_COLOR_ACCENT, LV_CHART_AXIS_PRIMARY_Y);
    ctx->chart = chart;
    lv_obj_add_event_cb(chart, chart_draw_post, LV_EVENT_DRAW_POST, ctx);

    ctx->timer = lv_timer_create(update_data, 1000, ctx);
    update_data(ctx->timer);

    printf("[DATA] Data page created\n");
    return page;
}
