#include "ui_i18n.h"
#include "../core/ui_events.h"
#include "../fonts/ui_font.h"
#include <string.h>

static ui_lang_t s_lang = UI_LANG_ZH;

static const char *s_zh[] = {
    [0]  = "\xE6\x95\xB0\xE6\x8D\xAE",
    [1]  = "\xE6\x8E\xA7\xE5\x88\xB6",
    [2]  = "\xE8\xAE\xBE\xE7\xBD\xAE",
    [3]  = "\xE5\x9C\xA8\xE7\xBA\xBF",
    [4]  = "\xE6\x99\xBA\xE8\x83\xBD\xE8\x8F\x9C\xE5\x9B\xAD",
    [5]  = "ESP32-P4",
    [6]  = "\xE5\xAE\x9E\xE6\x97\xB6\xE6\x95\xB0\xE6\x8D\xAE",
    [7]  = "\xE6\xB8\xA9\xE5\xBA\xA6",
    [8]  = "\xE6\xB9\xBF\xE5\xBA\xA6",
    [9]  = "\xE5\x9C\x9F\xE5\xA3\xA4",
    [10] = "\xE5\x85\x89\xE7\x85\xA7",
    [11] = "\xE9\x9B\xA8\xE9\x87\x8F",
    [12] = "\xE4\xB8\x8B\xE9\x9B\xA8",
    [13] = "\xE5\xB9\xB2\xE7\x87\xA5",
    [14] = "\xE8\xB5\xB0\xE5\xBB\x8A\xE7\x81\xAF",
    [15] = "\xE7\x81\xAF\xE5\xB8\xA6",
    [16] = "\xE8\x8F\x9C\xE7\x81\xAF",
    [17] = "\xE9\xA3\x8E\xE6\x89\x87",
    [18] = "\xE7\x85\xA7\xE6\x98\x8E",
    [19] = "\xE8\xAE\xBE\xE5\xA4\x87",
    [20] = "\xE4\xBC\xA0\xE6\x84\x9F\xE5\x99\xA8",
    [21] = "\xE8\x87\xAA\xE5\x8A\xA8\xE6\xA8\xA1\xE5\xBC\x8F",
    [22] = "\xE5\xB7\xB2\xE5\x80\x89\xE5\x87\xBA",
    [23] = "\xE5\xB7\xB2\xE6\x94\xB6\xE8\xb5\xB7",
    [24] = "\xE5\x80\x89\xE8\xA1\xA3\xE6\x9E\xB6",
    [25] = "\xE5\x80\x89\xE5\x87\xBA",
    [26] = "\xE6\x94\xB6\xE8\xb5\xB7",
    [27] = "\xE8\x87\xAA\xE5\x8A\xA8\xE6\xA8\xA1\xE5\xBC\x8F",
    [28] = "\xE6\x89\x8B\xE5\x8A\xA8",
    [29] = "\xE5\xBC\x80",
    [30] = "\xE5\x85\xB3",
    [31] = "\xE8\xAE\xBE\xE7\xBD\xAE",
    [32] = "\xE8\xAF\xAD\xE8\xA8\x80",
    [33] = "\xE7\xAE\x80\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87",
    [34] = "WiFi",
    [35] = "\xE5\xB7\xB2\xE8\xBF\x9E\xE6\x8E\xA5",
    [36] = "\xE5\x85\xB3\xE4\xBA\x8E",
    [37] = "\xE7\x89\x88\xE6\x9C\xAC",
    [38] = "\xE6\x81\xA2\xE5\xA4\x8D\xE5\x87\xBA\xE5\x8E\x82",
    [39] = "\xE7\xA1\xAE\xE8\xAE\xA4\xE9\x87\x8D\xE7\xBD\xAE\xE6\x89\x80\xE6\x9C\x89\xE6\x95\xB0\xE6\x8D\xAE?",
    [40] = "\xE6\x97\xA9\xE5\xAE\x89",
    [41] = "\xE6\x99\x9A\xE5\xAE\x89",
    [42] = "\xE7\xA6\xBB\xE5\xAE\xb6",
    [43] = "\xE6\x8E\xA7\xE5\x88\xB6\xE9\x9D\xA2\xE6\x9D\xBF",
};

static const char *s_en[] = {
    [0]  = "Data",
    [1]  = "Control",
    [2]  = "Settings",
    [3]  = "Online",
    [4]  = "Smart Garden",
    [5]  = "ESP32-P4",
    [6]  = "Real-time Data",
    [7]  = "Temperature",
    [8]  = "Humidity",
    [9]  = "Soil",
    [10] = "Light",
    [11] = "Rainfall",
    [12] = "Raining",
    [13] = "Dry",
    [14] = "Corridor Light",
    [15] = "Strip Light",
    [16] = "Veg Light",
    [17] = "Fan",
    [18] = "Lighting",
    [19] = "Devices",
    [20] = "Sensors",
    [21] = "Auto Mode",
    [22] = "Hung Out",
    [23] = "Collected",
    [24] = "Clothes Rack",
    [25] = "Hang Out",
    [26] = "Collect",
    [27] = "Auto Mode",
    [28] = "Manual",
    [29] = "On",
    [30] = "Off",
    [31] = "Settings",
    [32] = "Language",
    [33] = "English",
    [34] = "WiFi",
    [35] = "Connected",
    [36] = "About",
    [37] = "Version",
    [38] = "Reset",
    [39] = "Reset all data?",
    [40] = "Morning",
    [41] = "Night",
    [42] = "Away",
    [43] = "Control Panel",
};

void ui_i18n_init(void)
{
    s_lang = UI_LANG_ZH;
}

const char *ui_i18n_get(ui_i18n_key_t key)
{
    if (key < 0 || key >= UI_KEY_COUNT) return "";
    return s_lang == UI_LANG_ZH ? s_zh[key] : s_en[key];
}

ui_lang_t ui_i18n_get_lang(void)
{
    return s_lang;
}

void ui_i18n_set_lang(ui_lang_t lang)
{
    s_lang = lang;
    ui_event_publish(UI_EVENT_LANG_CHANGED);
}

const lv_font_t *ui_i18n_font(const lv_font_t *fallback)
{
    uint8_t req_size = 14;
    if (fallback == &lv_font_montserrat_12) {
        req_size = 12;
    } else if (fallback == &lv_font_montserrat_14) {
        req_size = 14;
    } else if (fallback == &lv_font_montserrat_20) {
        req_size = 20;
    } else if (fallback == &lv_font_montserrat_24) {
        req_size = 24;
    } else if (fallback == &lv_font_montserrat_26) {
        req_size = 26;
    }
    if (s_lang == UI_LANG_ZH) {
        return ui_font_cn(req_size);
    }
    return fallback;
}
