#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "bsp_board_extra.h"
#include "esp_lv_adapter.h"
#include "lvgl_adapter_init.h"
#include "lvgl.h"
#include "ui/core/ui_app.h"

static const char *TAG = "garden_ui";
static lv_timer_t *s_poll_timer;

static void mount_spiffs(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "storage",
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS mount failed: %d", err);
        return;
    }
    size_t total = 0, used = 0;
    esp_spiffs_info("storage", &total, &used);
    ESP_LOGI(TAG, "SPIFFS mounted: %d/%d KB used", used / 1024, total / 1024);
}

static void ui_poll_cb(lv_timer_t *timer)
{
    (void)timer;
    ui_app_poll();
}

void app_main(void)
{
    mount_spiffs();

    bsp_display_cfg_t cfg = {
        .hw_cfg = {
            .hdmi_resolution = BSP_HDMI_RES_NONE,
            .dsi_bus = {
                .lane_bit_rate_mbps = BSP_LCD_MIPI_DSI_LANE_BITRATE_MBPS,
            },
        },
    };

    lv_display_t *disp = lvgl_adapter_init(&cfg);
    if (!disp) {
        ESP_LOGE(TAG, "Failed to init LVGL adapter");
        return;
    }
    bsp_display_backlight_on();

    ESP_ERROR_CHECK(esp_lv_adapter_lock(-1));

    ui_app_init();

    s_poll_timer = lv_timer_create(ui_poll_cb, 5, NULL);

    esp_lv_adapter_unlock();

    ESP_LOGI(TAG, "Smart Garden UI started");
}
