/**
 * @file      display_long.c
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-17
 *
 */
#include <sdkconfig.h>
#include <driver/spi_master.h>
#include <sys/cdefs.h>
#include "driver/gpio.h"
#include "product_pins.h"
#include "esp_log.h"
#include <stdlib.h>
#include <string.h>
#include "esp_lcd_panel_vendor.h"

#if CONFIG_LILYGO_T_DISPLAY_LONG

#define SEND_BUF_SIZE           (14400)
#define DEFAULT_SPI_HANDLER     (SPI3_HOST)

static const char *TAG = "LONG";
static spi_device_handle_t spi = NULL;
static void amoled_write_cmd(uint32_t cmd, uint8_t *pdat, uint32_t lenght);


#define delay(ms)   vTaskDelay(ms / portTICK_PERIOD_MS)
#ifndef LOW
#define LOW 0
#endif
#ifndef HIGH
#define HIGH 1
#endif
#ifndef OUTPUT
#define OUTPUT GPIO_MODE_OUTPUT
#endif
#ifndef INPUT
#define INPUT   GPIO_MODE_INPUT
#endif



static const lcd_cmd_t vendor_config[] = {
    {0x28, {0x00}, 0x40},
    {0x10, {0x00}, 0x20},
    {0x11, {0x00}, 0x80},
    {0x29, {0x00}, 0x00},
};

static void pinMode(uint32_t gpio, uint8_t mode)
{
    gpio_config_t config = {0};
    config.pin_bit_mask = 1ULL << gpio;
    config.mode = mode == INPUT  ? GPIO_MODE_INPUT : mode == OUTPUT ? GPIO_MODE_OUTPUT : GPIO_MODE_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&config));
}

static void digitalWrite(uint32_t gpio, uint8_t level)
{
    gpio_set_level((gpio_num_t )gpio, level);
}

static void inline setCS()
{
    digitalWrite(BOARD_DISP_CS, LOW);
}

static void inline clrCS()
{
    digitalWrite(BOARD_DISP_CS, HIGH);
}

bool display_init()
{
    ESP_LOGI(TAG, "============T-Display-Long============");

    ESP_LOGI(TAG, "RST    > %d", BOARD_DISP_RESET);
    ESP_LOGI(TAG, "CS     > %d", BOARD_DISP_CS);
    ESP_LOGI(TAG, "SCK    > %d", BOARD_DISP_SCK);
    ESP_LOGI(TAG, "D0     > %d", BOARD_DISP_DATA0);
    ESP_LOGI(TAG, "D1     > %d", BOARD_DISP_DATA1);
    ESP_LOGI(TAG, "D2     > %d", BOARD_DISP_DATA2);
    ESP_LOGI(TAG, "D3     > %d", BOARD_DISP_DATA3);
    ESP_LOGI(TAG, "TE     > %d", BOARD_DISP_TE);
    ESP_LOGI(TAG, "Freq   > %d", DEFAULT_SCK_SPEED);
    ESP_LOGI(TAG, "BL     > %d", BOARD_DISP_BL);
    ESP_LOGI(TAG, "==================");

    pinMode(BOARD_DISP_RESET, OUTPUT);
    pinMode(BOARD_DISP_CS, OUTPUT);

    pinMode(BOARD_DISP_BL, OUTPUT);
    digitalWrite(BOARD_DISP_BL, LOW);

    //reset display
    digitalWrite(BOARD_DISP_RESET, HIGH);
    delay(130);
    digitalWrite(BOARD_DISP_RESET, LOW);
    delay(130);
    digitalWrite(BOARD_DISP_RESET, HIGH);
    delay(300);

    spi_bus_config_t buscfg = {
        .data0_io_num = BOARD_DISP_DATA0,
        .data1_io_num = BOARD_DISP_DATA1,
        .sclk_io_num = BOARD_DISP_SCK,
        .data2_io_num = BOARD_DISP_DATA2,
        .data3_io_num = BOARD_DISP_DATA3,
        .data4_io_num = BOARD_NONE_PIN,
        .data5_io_num = BOARD_NONE_PIN,
        .data6_io_num = BOARD_NONE_PIN,
        .data7_io_num = BOARD_NONE_PIN,
        .max_transfer_sz = (SEND_BUF_SIZE * 16) + 8,
        .flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_GPIO_PINS,
    };

    spi_device_interface_config_t devcfg = {
        .command_bits = 8,
        .address_bits = 24,
        .mode = 0,
        .clock_speed_hz = DEFAULT_SCK_SPEED,
        .spics_io_num = -1,
        .flags = SPI_DEVICE_HALFDUPLEX,
        .queue_size = 17,
    };
    esp_err_t ret = spi_bus_initialize(DEFAULT_SPI_HANDLER, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_initialize fail!");
        return false;
    }
    ret = spi_bus_add_device(DEFAULT_SPI_HANDLER, &devcfg, &spi);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "spi_bus_add_device fail!");
        return false;
    }

    int i = 1;
    while (i--) {
        const lcd_cmd_t *lcd_init = vendor_config;
        for (int i = 0; i < sizeof(vendor_config) / sizeof(lcd_cmd_t); i++) {
            amoled_write_cmd(lcd_init[i].addr,
                             lcd_init[i].param,
                             lcd_init[i].len & 0x3f);
            if (lcd_init[i].len & 0x80)
                delay(200);
            if (lcd_init[i].len & 0x40)
                delay(20);
        }
    }

    digitalWrite(BOARD_DISP_BL, HIGH);
    return true;
}

static void amoled_write_cmd(uint32_t cmd, uint8_t *pdat, uint32_t lenght)
{
    setCS();
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.flags = (SPI_TRANS_MULTILINE_CMD | SPI_TRANS_MULTILINE_ADDR);
    t.cmd = 0x02;
    t.addr = cmd << 8;
    if (lenght != 0) {
        t.tx_buffer = pdat;
        t.length = 8 * lenght;
    } else {
        t.tx_buffer = NULL;
        t.length = 0;
    }
    spi_device_polling_transmit(spi, &t);
    clrCS();
}


static void amoled_set_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    lcd_cmd_t t[2] = {
        {
            0x2A, {
                (uint8_t)((xs >> 8) & 0xFF),
                (uint8_t)(xs & 0xFF),
                (uint8_t)((xe >> 8) & 0xFF),
                (uint8_t)(xe & 0xFF)
            }, 0x04
        },
        {
            0x2B, {
                (uint8_t)((ys >> 8) & 0xFF),
                (uint8_t)(ys & 0xFF),
                (uint8_t)((ye >> 8) & 0xFF),
                (uint8_t)(ye & 0xFF)
            }, 0x04
        },
    };

    for (uint32_t i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
        amoled_write_cmd(t[i].addr, t[i].param, t[i].len);
    }
}


// Push (aka write pixel) colours to the TFT (use amoled_set_window() first)
static void amoled_push_buffer(uint16_t *data, uint32_t len)
{
    bool first_send = true;
    uint16_t *p = data;
    assert(p);
    assert(spi);
    do {
        setCS();
        size_t chunk_size = len;
        spi_transaction_ext_t t = {0};

        memset(&t, 0, sizeof(t));
        t.base.flags = SPI_TRANS_MODE_QIO;
        t.base.cmd = 0x32 ;

        if (first_send) {
            t.base.addr = 0x002C00;
        } else {
            t.base.addr = 0x003C00;
        }
        first_send = 0;

        if (chunk_size > SEND_BUF_SIZE) {
            chunk_size = SEND_BUF_SIZE;
        }

        t.base.tx_buffer = p;
        t.base.length = chunk_size * 16;
        if (!first_send) {
            clrCS();
        }

        setCS();

        spi_device_polling_transmit(spi, (spi_transaction_t *)&t);
        len -= chunk_size;
        p += chunk_size;
    } while (len > 0);
    clrCS();
}

void display_push_colors(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, uint16_t *data)
{
    amoled_set_window(x, y, x + width - 1, y + hight - 1);
    amoled_push_buffer(data, width * hight);
}
#endif

