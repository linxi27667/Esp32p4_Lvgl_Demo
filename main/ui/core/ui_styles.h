#ifndef UI_STYLES_H
#define UI_STYLES_H

#include "lvgl.h"

#define UI_COLOR_BG        lv_color_hex(0xF2F2F7)
#define UI_COLOR_CARD      lv_color_hex(0xFFFFFF)
#define UI_COLOR_ACCENT    lv_color_hex(0x007AFF)
#define UI_COLOR_TEXT      lv_color_hex(0x1C1C1E)
#define UI_COLOR_TEXT_SEC  lv_color_hex(0x8E8E93)
#define UI_COLOR_GREEN     lv_color_hex(0x34C759)
#define UI_COLOR_ORANGE    lv_color_hex(0xFF9500)
#define UI_COLOR_RED       lv_color_hex(0xFF3B30)
#define UI_COLOR_PURPLE    lv_color_hex(0xAF52DE)
#define UI_COLOR_BLUE      lv_color_hex(0x007AFF)
#define UI_COLOR_BORDER    lv_color_hex(0xE5E5EA)

#define UI_CARD_RADIUS  16
#define UI_BTN_RADIUS   12
#define UI_HEADER_HEIGHT 56
#define UI_TAB_HEIGHT    60

lv_obj_t *ui_create_label(lv_obj_t *parent, const char *text, const lv_font_t *font, lv_color_t color);
lv_obj_t *ui_create_icon(lv_obj_t *parent, const char *icon, const lv_font_t *font, lv_color_t color);
lv_obj_t *ui_create_dot(lv_obj_t *parent, lv_color_t color, int size);
lv_obj_t *ui_create_switch(lv_obj_t *parent, lv_color_t active_color);
lv_obj_t *ui_create_card(lv_obj_t *parent);
void ui_apply_card_shadow(lv_obj_t *card);

void ui_styles_init(void);

#endif
