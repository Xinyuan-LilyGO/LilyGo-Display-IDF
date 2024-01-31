/**
 * @file      display_hmi.c
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-17
 *
 */

#include <sdkconfig.h>
#include "freertos/FreeRTOS.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_dma_utils.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "product_pins.h"

#if CONFIG_LILYGO_T_HMI

#include "driver/spi_master.h"
#include "esp_lcd_touch_xpt2046.h"

#include "lvgl.h"
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (10 * 1000 * 1000)

static const char *TAG = "HMI";

static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_touch_handle_t tp = NULL;

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
    ESP_LOGI(TAG, "============T-HMI==============");

    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = BOARD_TFT_DC,
        .wr_gpio_num = BOARD_TFT_WR,
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .data_gpio_nums =
        {
            BOARD_TFT_DATA0,
            BOARD_TFT_DATA1,
            BOARD_TFT_DATA2,
            BOARD_TFT_DATA3,
            BOARD_TFT_DATA4,
            BOARD_TFT_DATA5,
            BOARD_TFT_DATA6,
            BOARD_TFT_DATA7,
        },
        .bus_width = 8,
        .max_transfer_bytes = AMOLED_WIDTH  * 100 * sizeof(uint16_t),
        .psram_trans_align = 64,
        .sram_trans_align = 4
    };
    esp_lcd_new_i80_bus(&bus_config, &i80_bus);


    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = BOARD_TFT_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .dc_levels =
        {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .on_color_trans_done = display_notify_lvgl_flush_ready,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BOARD_TFT_RST,
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
        .vendor_config = NULL
    };
    esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle);

    esp_lcd_panel_reset(panel_handle);

    esp_lcd_panel_init(panel_handle);

    esp_lcd_panel_mirror(panel_handle, false, true);

    esp_lcd_panel_swap_xy(panel_handle, true);

    esp_lcd_panel_disp_on_off(panel_handle, true);

    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << BOARD_TFT_BL
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(BOARD_TFT_BL, 1);
}



void board_hmi_touch_init()
{
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;

    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = BOARD_TOUCH_SPI_SCLK,
        .mosi_io_num = BOARD_TOUCH_SPI_MOSI,
        .miso_io_num = BOARD_TOUCH_SPI_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 100  * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_spi_config_t tp_io_config = ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(BOARD_TOUCH_SPI_CS);
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &tp_io_config, &tp_io_handle));

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = AMOLED_WIDTH,
        .y_max = AMOLED_HEIGHT,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = BOARD_TOUCH_IRQ,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    ESP_LOGI(TAG, "Initialize touch controller XPT2046");
    ESP_ERROR_CHECK(esp_lcd_touch_new_spi_xpt2046(tp_io_handle, &tp_cfg, &tp));
    esp_lcd_touch_set_swap_xy(tp, true);
    esp_lcd_touch_set_mirror_x(tp, true);
    esp_lcd_touch_set_mirror_y(tp, true);

    ESP_LOGW(TAG, "Touch controller XPT2046 coordinates not calibrated");

}



uint8_t board_hmi_get_point(uint16_t *x, uint16_t *y)
{
    uint8_t touchpad_cnt = 0;
    esp_lcd_touch_read_data(tp);
    esp_lcd_touch_get_coordinates(tp, x, y, NULL, &touchpad_cnt, 1);
    if (touchpad_cnt) {
        *y = AMOLED_HEIGHT - *y;
    }
    return touchpad_cnt;
}
#endif

