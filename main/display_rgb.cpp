/**
 * @file      display_rgb.c
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-21
 *
 */
#include <sdkconfig.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "product_pins.h"
#include "i2c_driver.h"

#if CONFIG_LILYGO_T_RGB

#include "ExtensionIOXL9555.hpp"
#include "TouchDrvGT911.hpp"
#include "TouchDrvFT6X36.hpp"
#include "TouchDrvCSTXXX.hpp"

#include "lvgl.h"
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (8 * 1000 * 1000)

static const char *TAG = "RGB";
static SemaphoreHandle_t sem_vsync_end;
static SemaphoreHandle_t sem_gui_ready;
static esp_lcd_panel_handle_t panel_handle = NULL;

extern lv_disp_drv_t disp_drv;

const lcd_cmd_t *init_cmd = st7701_2_1_inches;
extern i2c_master_bus_handle_t bus_handle;
TouchDrvInterface *touchDrv;
void *buf1 = NULL;
void *buf2 = NULL;

ExtensionIOXL9555 extension;
ExtensionIOXL9555::ExtensionGPIO cs = ExtensionIOXL9555::IO3;
ExtensionIOXL9555::ExtensionGPIO mosi = ExtensionIOXL9555::IO4;
ExtensionIOXL9555::ExtensionGPIO sclk = ExtensionIOXL9555::IO5;
ExtensionIOXL9555::ExtensionGPIO reset = ExtensionIOXL9555::IO6;
ExtensionIOXL9555::ExtensionGPIO power_enable = ExtensionIOXL9555::IO2;
ExtensionIOXL9555::ExtensionGPIO sdmmc_cs = ExtensionIOXL9555::IO7;
ExtensionIOXL9555::ExtensionGPIO tp_reset = ExtensionIOXL9555::IO1;


extern "C" void display_push_colors(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, uint16_t *data)
{
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, width, hight, data);
    lv_disp_flush_ready(&disp_drv);
}

static void writeCommand(const uint8_t cmd)
{
    uint16_t data = cmd;
    extension.transfer9(data);
}

static void writeData(const uint8_t *data, int len)
{
    uint32_t i = 0;
    if (len == 0)
        return; // no need to send anything
    do {
        uint16_t pdat = (*(data + i)) | 1 << 8;
        extension.transfer9(pdat);
        i++;
    } while (len--);
}

extern "C" void display_init()
{
    assert(init_cmd);

    ESP_LOGI(TAG, "============T-RGB==============");

    extension.pinMode(reset, OUTPUT);
    extension.digitalWrite(reset, LOW);
    delay(20);
    extension.digitalWrite(reset, HIGH);
    delay(10);

    ESP_LOGI(TAG, "Extension BEGIN SPI SUCCESS !");
    extension.beginSPI(mosi, -1, sclk, cs);

    int i = 0;
    while (init_cmd[i].len != 0xff) {
        writeCommand(init_cmd[i].addr);
        writeData(init_cmd[i].param, init_cmd[i].len & 0x1F);
        if (init_cmd[i].len & 0x80) {
            delay(100);
        }
        i++;
    }

    esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .timings =
        {
            .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
            .h_res = AMOLED_WIDTH,
            .v_res = AMOLED_HEIGHT,
            // The following parameters should refer to LCD spec
            .hsync_pulse_width = 1,
            .hsync_back_porch = 30,
            .hsync_front_porch = 50,
            .vsync_pulse_width = 1,
            .vsync_back_porch = 30,
            .vsync_front_porch = 20,

            .flags =
            {
                .hsync_idle_low = 0,
                .vsync_idle_low = 0,
                .de_idle_high = 0,
                .pclk_active_neg = 1,
                .pclk_idle_high = 0
            },
        },
        .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
        .bits_per_pixel = 0,
        .num_fbs = 1,
        .bounce_buffer_size_px = 10 * AMOLED_WIDTH,
        .sram_trans_align = 64,
        .psram_trans_align = 64,
        .hsync_gpio_num = BOARD_TFT_HSYNC,
        .vsync_gpio_num = BOARD_TFT_VSYNC,
        .de_gpio_num = BOARD_TFT_DE,
        .pclk_gpio_num = BOARD_TFT_PCLK,
        .disp_gpio_num = GPIO_NUM_NC,
        .data_gpio_nums =
        {
            // BOARD_TFT_DATA0,
            BOARD_TFT_DATA13,
            BOARD_TFT_DATA14,
            BOARD_TFT_DATA15,
            BOARD_TFT_DATA16,
            BOARD_TFT_DATA17,

            BOARD_TFT_DATA6,
            BOARD_TFT_DATA7,
            BOARD_TFT_DATA8,
            BOARD_TFT_DATA9,
            BOARD_TFT_DATA10,
            BOARD_TFT_DATA11,
            // BOARD_TFT_DATA12,

            BOARD_TFT_DATA1,
            BOARD_TFT_DATA2,
            BOARD_TFT_DATA3,
            BOARD_TFT_DATA4,
            BOARD_TFT_DATA5,
        },
        .flags =
        {
            .disp_active_low = 0,
            .refresh_on_demand = 0,
            .fb_in_psram = 1,
            .double_fb = 0,
            .no_fb  = 0,
            .bb_invalidate_cache = 0
        },
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));



    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_config_t bk_gpio_config = {
        .pin_bit_mask = 1ULL << BOARD_TFT_BL,
                             .mode = GPIO_MODE_OUTPUT,
                             .pull_up_en = GPIO_PULLUP_DISABLE,
                             .pull_down_en = GPIO_PULLDOWN_DISABLE,
                             .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(BOARD_TFT_BL, 1);
}

void TouchDrvDigitalWrite(uint32_t gpio, uint8_t level)
{
    if (gpio & 0x80) {
        extension.digitalWrite(gpio & 0x7F, level);
    } else {
        digitalWrite(gpio, level);
    }
}

int TouchDrvDigitalRead(uint32_t gpio)
{
    if (gpio & 0x80) {
        return extension.digitalRead(gpio & 0x7F);
    } else {
        return digitalRead(gpio);
    }
}

void TouchDrvPinMode(uint32_t gpio, uint8_t mode)
{
    if (gpio & 0x80) {
        extension.pinMode(gpio & 0x7F, mode);
    } else {
        pinMode(gpio, mode);
    }
}

extern "C"  bool board_rgb_touch_init()
{

    const uint8_t touch_reset_pin = tp_reset | 0x80;
    const uint8_t touch_irq_pin = BOARD_TOUCH_IRQ;

    ESP_LOGI(TAG, "=================setupTouchDrv====================");

    bool result = false;

    if (!extension.begin(bus_handle,  XL9555_SLAVE_ADDRESS0)) {
        ESP_LOGE(TAG, "ERROR : XL9555 NO ON LINE!!!");
        return false;
    }

    extension.pinMode(power_enable, OUTPUT);
    extension.digitalWrite(power_enable, HIGH);

    extension.pinMode(sdmmc_cs, OUTPUT);
    extension.digitalWrite(sdmmc_cs, HIGH);

    touchDrv = new TouchDrvCSTXXX();
    touchDrv->setGpioCallback(TouchDrvPinMode, TouchDrvDigitalWrite, TouchDrvDigitalRead);
    touchDrv->setPins(touch_reset_pin, touch_irq_pin);
    result = touchDrv->begin(bus_handle, CST816_SLAVE_ADDRESS);
    if (result) {
        init_cmd = st7701_2_1_inches;
        const char *model = touchDrv->getModelName();
        TouchDrvCSTXXX *drv = static_cast<TouchDrvCSTXXX *>(touchDrv);
        drv->disableAutoSleep();
        ESP_LOGI(TAG, "Successfully initialized %s, using %s Driver!", model, model);
        return true;
    }

    delete touchDrv;

    touchDrv = new TouchDrvFT6X36();
    touchDrv->setGpioCallback(TouchDrvPinMode, TouchDrvDigitalWrite, TouchDrvDigitalRead);
    touchDrv->setPins(touch_reset_pin, touch_irq_pin);
    result = touchDrv->begin(bus_handle, FT3267_SLAVE_ADDRESS);
    if (result) {
        init_cmd = st7701_2_1_inches;
        ESP_LOGI(TAG, "Successfully initialized FT63X6, using FT63X6 Driver!");
        return true;
    }

    delete touchDrv;

    touchDrv = new TouchDrvGT911();
    touchDrv->setGpioCallback(TouchDrvPinMode, TouchDrvDigitalWrite, TouchDrvDigitalRead);
    touchDrv->setPins(touch_reset_pin, touch_irq_pin);
    result = touchDrv->begin(bus_handle, GT911_SLAVE_ADDRESS_L);
    if (result) {
        init_cmd = st7701_2_8_inches;
        ESP_LOGI(TAG, "Successfully initialized GT911, using GT911 Driver!");
        return true;
    }
    delete touchDrv;

    touchDrv = NULL;

    ESP_LOGE(TAG, "Failed initialized TouchDrv!!!!!");

    return false;
}



extern "C" uint8_t board_rgb_get_point(uint16_t *x, uint16_t *y)
{
    uint8_t touchpad_cnt = 0;
    static int16_t lastX = 0, lastY = 0;
    if (touchDrv) {
        touchpad_cnt =  (touchDrv->getPoint(&lastX, &lastY, 1));
        if (touchpad_cnt) {
            ESP_LOGI(TAG, "X: %d Y:%d", lastX, lastY);
            *x = lastX;
            *y = lastY;
        }
    }
    return touchpad_cnt;
}
#endif

