/**
 * @file      display_esp32.c
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-08
 *
 */
#include <sdkconfig.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/gpio.h"
#include "product_pins.h"
#include "esp_log.h"
#include "esp_idf_version.h"
#include "driver/spi_master.h"
#include "lvgl.h"

#if defined(CONFIG_LILYGO_T_DONGLE_S3)
#include "esp_lcd_panel_st7735.h"
#endif


#if defined(CONFIG_LILYGO_T_DISPLAY) ||\
    defined(CONFIG_LILYGO_T_DONGLE_S2) ||\
    defined(CONFIG_LILYGO_T_DONGLE_S3)

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (27 * 1000 * 1000)
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8
#define LCD_HOST                       SPI2_HOST

static const char *TAG = "TFT";
static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;
extern lv_disp_drv_t disp_drv;

void display_push_colors(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, uint16_t *data)
{
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, width, hight, data);
}

bool display_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_disp_flush_ready(&disp_drv);
    return false;
}

void display_init()
{

#if defined(CONFIG_LILYGO_T_DISPLAY)
    ESP_LOGI(TAG, "============T-Display ESP32============");
#elif  defined(CONFIG_LILYGO_T_DONGLE_S2)
    ESP_LOGI(TAG, "============T-Dongle ESP32S2============");
#elif defined(CONFIG_LILYGO_T_DONGLE_S3)
    ESP_LOGI(TAG, "============T-Dongle ESP32S3============");
#endif

    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = BOARD_SPI_SCK,
        .mosi_io_num = BOARD_SPI_MOSI,
        .miso_io_num = BOARD_SPI_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = AMOLED_HEIGHT * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = BOARD_TFT_DC,
        .cs_gpio_num = BOARD_TFT_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .on_color_trans_done = display_notify_lvgl_flush_ready
    };

    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BOARD_TFT_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
#if defined(CONFIG_LILYGO_T_DISPLAY) ||\
    defined(CONFIG_LILYGO_T_DONGLE_S2)
    ESP_LOGI(TAG, "Install ST7789 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
#else
    ESP_LOGI(TAG, "Install ST7735 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7735(io_handle, &panel_config, &panel_handle));
#endif

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

#if defined(CONFIG_LILYGO_T_DISPLAY)
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    esp_lcd_panel_swap_xy(panel_handle, true);
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, true));
    esp_lcd_panel_set_gap(panel_handle, 40, 53);
#elif defined(CONFIG_LILYGO_T_DONGLE_S2)
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    esp_lcd_panel_swap_xy(panel_handle, false);
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    esp_lcd_panel_set_gap(panel_handle, 53, 40);
#elif defined(CONFIG_LILYGO_T_DONGLE_S3)
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    esp_lcd_panel_set_gap(panel_handle, 26, 1);
    // esp_lcd_panel_swap_xy(panel_handle, true);
    // ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, true));
#else


#endif

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << BOARD_TFT_BL
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

#if  defined(CONFIG_LILYGO_T_DONGLE_S3)
    gpio_set_level(BOARD_TFT_BL, 0);
    ESP_LOGI(TAG, "backlight = 0");
#else
    ESP_LOGI(TAG, "backlight = 1");
    gpio_set_level(BOARD_TFT_BL, 1);
#endif
}



#endif
















