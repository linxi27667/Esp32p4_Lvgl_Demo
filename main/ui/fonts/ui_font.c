#include "ui_font.h"
#include <stdio.h>

#if LV_USE_TINY_TTF

/* TTF data must persist for lifetime of created fonts */
static void *s_cn_ttf_data = NULL;
static size_t s_cn_ttf_size = 0;
static void *s_fa_ttf_data = NULL;

static lv_font_t *s_cn_12 = NULL;
static lv_font_t *s_cn_14 = NULL;
static lv_font_t *s_cn_16 = NULL;
static lv_font_t *s_cn_20 = NULL;
static lv_font_t *s_cn_24 = NULL;
static lv_font_t *s_fa_icon = NULL;

static void *load_ttf_file(const char *path, size_t *out_size)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        LV_LOG_WARN("ui_font: can't open %s", path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (len <= 0) { fclose(f); return NULL; }
    void *data = lv_malloc(len);
    if (!data) { fclose(f); return NULL; }
    size_t rd = fread(data, 1, len, f);
    fclose(f);
    if (rd != (size_t)len) { lv_free(data); return NULL; }
    *out_size = len;
    return data;
}

void ui_font_init(void)
{
    s_cn_ttf_data = load_ttf_file("/spiffs/NotoSansSC.ttf", &s_cn_ttf_size);
    if (s_cn_ttf_data) {
        s_cn_12 = lv_tiny_ttf_create_data(s_cn_ttf_data, s_cn_ttf_size, 12);
        s_cn_14 = lv_tiny_ttf_create_data(s_cn_ttf_data, s_cn_ttf_size, 14);
        s_cn_16 = lv_tiny_ttf_create_data(s_cn_ttf_data, s_cn_ttf_size, 16);
        s_cn_20 = lv_tiny_ttf_create_data(s_cn_ttf_data, s_cn_ttf_size, 20);
        s_cn_24 = lv_tiny_ttf_create_data(s_cn_ttf_data, s_cn_ttf_size, 24);
        LV_LOG_USER("Chinese TTF loaded OK (5 sizes)");
    } else {
        LV_LOG_WARN("Chinese TTF not found: /spiffs/NotoSansSC.ttf");
    }

    size_t fa_size;
    s_fa_ttf_data = load_ttf_file("/spiffs/fa-solid-900.ttf", &fa_size);
    if (s_fa_ttf_data) {
        s_fa_icon = lv_tiny_ttf_create_data(s_fa_ttf_data, fa_size, 20);
        LV_LOG_USER("FontAwesome TTF loaded OK");
    }
}

static uint8_t nearest_cn_size(uint8_t size)
{
    if (size <= 13) return 12;
    if (size <= 15) return 14;
    if (size <= 18) return 16;
    if (size <= 22) return 20;
    if (size <= 28) return 24;
    return 26;
}

const lv_font_t *ui_font_cn(uint8_t size)
{
    if (s_cn_14) {
        switch (nearest_cn_size(size)) {
            case 12: return s_cn_12;
            case 14: return s_cn_14;
            case 16: return s_cn_16;
            case 20: return s_cn_20;
            case 24: return s_cn_24;
            default: return s_cn_24;
        }
    }
    /* Fallback to built-in Montserrat */
    switch (nearest_cn_size(size)) {
        case 12: return UI_FONT_12;
        case 14: return UI_FONT_14;
        case 20: return UI_FONT_20;
        default: return UI_FONT_24;
    }
}

const lv_font_t *ui_font_icon(uint8_t size)
{
    (void)size;
    return s_fa_icon ? s_fa_icon : UI_FONT_24;
}

#else

void ui_font_init(void) {}

static uint8_t nearest_cn_size(uint8_t size)
{
    if (size <= 13) return 12;
    if (size <= 17) return 14;
    if (size <= 22) return 20;
    if (size <= 30) return 24;
    return 26;
}

const lv_font_t *ui_font_cn(uint8_t size)
{
    switch (nearest_cn_size(size)) {
        case 12: return UI_FONT_12;
        case 14: return UI_FONT_14;
        case 20: return UI_FONT_20;
        case 24: return UI_FONT_24;
        default: return UI_FONT_36;
    }
}

const lv_font_t *ui_font_icon(uint8_t size)
{
    (void)size;
    return UI_FONT_24;
}

#endif
