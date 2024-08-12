/**
 * @file      product_pins.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-07
 *
 */

#pragma once

#include <sdkconfig.h>
#include "initSequence.h"

#define BOARD_NONE_PIN      (-1)

// LILYGO 1.47 Inch AMOLED(SH8501) S3R8
// https://www.lilygo.cc/products/t-display-amoled
#if CONFIG_LILYGO_T_AMOLED_LITE_147

#define BOARD_DISP_CS       (4)
#define BOARD_DISP_SCK      (5)
#define BOARD_DISP_DATA0    (7)
#define BOARD_DISP_DATA1    (10)
#define BOARD_DISP_DATA2    (11)
#define BOARD_DISP_DATA3    (12)
#define BOARD_DISP_RESET    (40)
#define BOARD_DISP_TE       (6)
#define BOARD_I2C_SDA       (1)
#define BOARD_I2C_SCL       (2)
#define BOARD_PMU_IRQ       (3)
#define BOARD_SENSOR_IRQ    (8)
#define BOARD_TOUCH_IRQ     (13)
#define BOARD_TOUCH_RST     (14)
#define BOARD_BOOT_PIN      (0)
#define BOARD_BUTTON1_PIN   (0)
#define BOARD_BUTTON2_PIN   (21)
#define BOARD_PIXELS_PIN    (18)
#define BOARD_PIXELS_NUM    (1)
#define DEFAULT_SCK_SPEED   (30000000)

#define AMOLED_WIDTH        194
#define AMOLED_HEIGHT       368
#define AMOLED_EN_PIN       (-1)


#define AMOLED_INIT_CMD     sh8501_cmd
#define AMOLED_INIT_CMD_LEN SH8501_INIT_SEQUENCE_LENGHT

#define CONFIG_PMU_AXP2101  (1)
#define BOARD_HAS_TOUCH      1
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)
#define DISPLAY_FULLRESH     true
// LILYGO 1.91 Inch AMOLED(RM67162) S3R8
// https://www.lilygo.cc/products/t-display-s3-amoled?variant=42837728526517
#elif CONFIG_LILYGO_T_DISPLAY_S3_AMOLED

#define BOARD_DISP_CS       (6)
#define BOARD_DISP_SCK      (47)
#define BOARD_DISP_DATA0    (18)
#define BOARD_DISP_DATA1    (7)
#define BOARD_DISP_DATA2    (48)
#define BOARD_DISP_DATA3    (5)
#define BOARD_DISP_RESET    (17)
#define BOARD_DISP_TE       (9)

#define BOARD_BOOT_PIN      (0)
#define BOARD_BUTTON1_PIN   (0)
#define BOARD_BUTTON2_PIN   (21)
#define DEFAULT_SCK_SPEED   (75000000)

// Define 3,2 use i2c
#define BOARD_I2C_SDA       (3)
#define BOARD_I2C_SCL       (2)

#define AMOLED_WIDTH        240
#define AMOLED_HEIGHT       536
#define AMOLED_EN_PIN       (-1)

#define AMOLED_INIT_CMD     rm67162_cmd
#define AMOLED_INIT_CMD_LEN RM67162_INIT_SEQUENCE_LENGHT
#define BOARD_HAS_TOUCH      0
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)
#define DISPLAY_FULLRESH     true

// LILYGO 1.91 Inch AMOLED Touch (RM67162) S3R8
// https://www.lilygo.cc/products/t-display-s3-amoled?variant=43228221636789
#elif CONFIG_LILYGO_T_DISPLAY_S3_AMOLED_TOUCH

#define BOARD_DISP_CS       (6)
#define BOARD_DISP_SCK      (47)
#define BOARD_DISP_DATA0    (18)
#define BOARD_DISP_DATA1    (7)
#define BOARD_DISP_DATA2    (48)
#define BOARD_DISP_DATA3    (5)
#define BOARD_DISP_RESET    (17)
#define BOARD_DISP_TE       (9)

#define BOARD_I2C_SDA       (3)
#define BOARD_I2C_SCL       (2)

#define BOARD_TOUCH_IRQ     (21)
#define BOARD_TOUCH_RST     (-1)

#define BOARD_BOOT_PIN      (0)
#define BOARD_BUTTON1_PIN   (0)
#define BOARD_BUTTON2_PIN   (21)
#define DEFAULT_SCK_SPEED   (75000000)

#define AMOLED_WIDTH        240
#define AMOLED_HEIGHT       536
#define AMOLED_EN_PIN       (38)

#define AMOLED_INIT_CMD     rm67162_cmd
#define AMOLED_INIT_CMD_LEN RM67162_INIT_SEQUENCE_LENGHT
#define BOARD_HAS_TOUCH      1

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)
#define DISPLAY_FULLRESH     true
// LILYGO 2.41 Inch AMOLED(RM690B0) S3R8
// https://www.lilygo.cc/products/t4-s3
#elif CONFIG_LILYGO_T4_S3_241

#define BOARD_DISP_CS       (11)
#define BOARD_DISP_SCK      (15)
#define BOARD_DISP_DATA0    (14)
#define BOARD_DISP_DATA1    (10)
#define BOARD_DISP_DATA2    (16)
#define BOARD_DISP_DATA3    (12)
#define BOARD_DISP_RESET    (13)
#define BOARD_DISP_TE       (-1)

#define BOARD_I2C_SDA       (6)
#define BOARD_I2C_SCL       (7)

#define BOARD_TOUCH_IRQ     (8)
#define BOARD_TOUCH_RST     (17)

#define BOARD_SD_MISO       (4)
#define BOARD_SD_MOSI       (2)
#define BOARD_SD_SCLK       (3)
#define BOARD_SD_CS         (1)

#define BOARD_BOOT_PIN      (0)
#define BOARD_BUTTON1_PIN   (0)
#define DEFAULT_SCK_SPEED   (36000000)

#define AMOLED_WIDTH        600
#define AMOLED_HEIGHT       450

#define AMOLED_EN_PIN       (9)

#define AMOLED_INIT_CMD     rm690b0_cmd
#define AMOLED_INIT_CMD_LEN RM690B0_INIT_SEQUENCE_LENGHT

#define CONFIG_PMU_SY6970   (1)

#define BOARD_HAS_TOUCH      1

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)

#define DISPLAY_FULLRESH     true

#elif CONFIG_LILYGO_T_Track_102

#define AMOLED_WIDTH        600
#define AMOLED_HEIGHT       450

#define AMOLED_EN_PIN       (-1)

#define AMOLED_INIT_CMD     jd9613_cmd
#define AMOLED_INIT_CMD_LEN JD9613_INIT_SEQUENCE_LENGHT

#define BOARD_HAS_TOUCH      0
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)


#define DISPLAY_FULLRESH     true


#elif CONFIG_LILYGO_T_DISPLAY

#define BOARD_POWERON        (gpio_num_t)(14)

#define BOARD_SPI_MISO       (21)
#define BOARD_SPI_MOSI       (19)
#define BOARD_SPI_SCK        (18)
#define BOARD_TFT_CS         (5)
#define BOARD_TFT_RST        (23)
#define BOARD_TFT_DC         (16)
#define BOARD_TFT_BL         (4)

#define AMOLED_WIDTH         (135)
#define AMOLED_HEIGHT        (240)

#define BOARD_HAS_TOUCH      0

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * 100)

#define DISPLAY_FULLRESH     false

#elif CONFIG_LILYGO_T_DISPLAY_S3

#define BOARD_POWERON        (gpio_num_t)(15)

#define BOARD_TFT_BL         (38)
#define BOARD_TFT_DATA0      (39)
#define BOARD_TFT_DATA1      (40)
#define BOARD_TFT_DATA2      (41)
#define BOARD_TFT_DATA3      (42)
#define BOARD_TFT_DATA4      (45)
#define BOARD_TFT_DATA5      (46)
#define BOARD_TFT_DATA6      (47)
#define BOARD_TFT_DATA7      (48)
#define BOARD_TFT_RST        (5)
#define BOARD_TFT_CS         (6)
#define BOARD_TFT_DC         (7)
#define BOARD_TFT_WR         (8)
#define BOARD_TFT_RD         (9)
#define BOARD_I2C_SCL        (17)
#define BOARD_I2C_SDA        (18)
#define BOARD_TOUCH_IRQ      (16)
#define BOARD_TOUCH_RST      (21)
#define AMOLED_WIDTH         (170)
#define AMOLED_HEIGHT        (320)

#define BOARD_HAS_TOUCH      1

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * 100)

#define DISPLAY_FULLRESH     false

#elif CONFIG_LILYGO_T_DISPLAY_S3_PRO

// LTR553 , TOUCH , SY6970 , Camera share I2C Bus
#define BOARD_I2C_SDA        (5)
#define BOARD_I2C_SCL        (6)
// SD , TFT share SPI Bus
#define BOARD_SPI_MISO       (8)
#define BOARD_SPI_MOSI       (17)
#define BOARD_SPI_SCK        (18)
#define BOARD_TFT_CS         (39)
#define BOARD_TFT_RST        (47)
#define BOARD_TFT_DC         (9)
#define BOARD_TFT_BL         (48)
#define BOARD_SD_CS          (14)
#define BOARD_SENSOR_IRQ     (21)
#define BOARD_TOUCH_RST      (13)
#define BOARD_TOUCH_IRQ      (7)
#define AMOLED_HEIGHT        (480)
#define AMOLED_WIDTH         (222)

#define BOARD_HAS_TOUCH      1
#define DISPLAY_FULLRESH     false

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * 100)

#elif CONFIG_LILYGO_T_QT_S3

// #define BOARD_POWERON        (gpio_num_t)(10)

#define BOARD_SPI_MISO       (-1)
#define BOARD_SPI_MOSI       (2)
#define BOARD_SPI_SCK        (3)
#define BOARD_TFT_CS         (5)
#define BOARD_TFT_RST        (1)
#define BOARD_TFT_DC         (6)
#define BOARD_TFT_BL         (10)

#define AMOLED_HEIGHT        (128)
#define AMOLED_WIDTH         (128)

#define BOARD_HAS_TOUCH      0
#define DISPLAY_FULLRESH     false
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)


#elif CONFIG_LILYGO_T_DONGLE_S2

// #define BOARD_POWERON        (gpio_num_t)(10)

#define BOARD_SPI_MISO       (-1)
#define BOARD_SPI_MOSI       (35)
#define BOARD_SPI_SCK        (36)
#define BOARD_TFT_CS         (34)
#define BOARD_TFT_RST        (38)
#define BOARD_TFT_DC         (37)
#define BOARD_TFT_BL         (33)

#define AMOLED_HEIGHT        (135)
#define AMOLED_WIDTH         (240)

#define BOARD_HAS_TOUCH      0
#define DISPLAY_FULLRESH     false
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)

#elif CONFIG_LILYGO_T_DONGLE_S3

// #define BOARD_POWERON        (gpio_num_t)(10)

#define BOARD_SPI_MISO       (-1)
#define BOARD_SPI_MOSI       (3)
#define BOARD_SPI_SCK        (5)
#define BOARD_TFT_CS         (4)
#define BOARD_TFT_RST        (1)
#define BOARD_TFT_DC         (2)
#define BOARD_TFT_BL         (38)

#define AMOLED_HEIGHT        (80)
#define AMOLED_WIDTH         (160)

#define BOARD_HAS_TOUCH      0
#define DISPLAY_FULLRESH     false
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)

#elif CONFIG_LILYGO_T_DISPLAY_LONG


#define BOARD_DISP_CS       (12)
#define BOARD_DISP_SCK      (17)
#define BOARD_DISP_DATA0    (13)
#define BOARD_DISP_DATA1    (18)
#define BOARD_DISP_DATA2    (21)
#define BOARD_DISP_DATA3    (14)
#define BOARD_DISP_RESET    (16)
#define BOARD_DISP_TE       (-1)
#define BOARD_DISP_BL       (1)

#define BOARD_I2C_SDA       (15)
#define BOARD_I2C_SCL       (10)

#define BOARD_TOUCH_IRQ     (11)
#define BOARD_TOUCH_RST     (16)

#define AMOLED_HEIGHT       (180)
#define AMOLED_WIDTH        (640)

#define BOARD_HAS_TOUCH      1

#define DISPLAY_FULLRESH     true
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)

#define DEFAULT_SCK_SPEED   (30000000)

#elif CONFIG_LILYGO_T_HMI


#define BOARD_POWERON         (gpio_num_t)(14)
#define BOARD_POWERON1        (gpio_num_t)(10)

#define BOARD_TFT_BL         (38)
#define BOARD_TFT_DATA0      (48)
#define BOARD_TFT_DATA1      (47)
#define BOARD_TFT_DATA2      (39)
#define BOARD_TFT_DATA3      (40)
#define BOARD_TFT_DATA4      (41)
#define BOARD_TFT_DATA5      (42)
#define BOARD_TFT_DATA6      (45)
#define BOARD_TFT_DATA7      (46)
#define BOARD_TFT_RST        (-1)
#define BOARD_TFT_CS         (6)
#define BOARD_TFT_DC         (7)
#define BOARD_TFT_WR         (8)


#define BOARD_TOUCH_SPI_MISO (4)
#define BOARD_TOUCH_SPI_MOSI (3)
#define BOARD_TOUCH_SPI_SCLK (1)
#define BOARD_TOUCH_SPI_CS   (2)
#define BOARD_TOUCH_IRQ      (gpio_num_t)(9)

#define AMOLED_WIDTH         (240)
#define AMOLED_HEIGHT        (320)

#define BOARD_HAS_TOUCH      1

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * 100)

#define DISPLAY_FULLRESH     false

#elif CONFIG_LILYGO_T_QT_C6

#define BOARD_SPI_MISO       (-1)
#define BOARD_SPI_MOSI       (15)
#define BOARD_SPI_SCK        (18)
#define BOARD_TFT_CS         (14)
#define BOARD_TFT_RST        (20)
#define BOARD_TFT_DC         (19)
#define BOARD_TFT_BL         (2)

#define BOARD_I2C_SDA       (21)
#define BOARD_I2C_SCL       (22)

#define BOARD_TOUCH_IRQ     (7)
#define BOARD_TOUCH_RST     (23)

#define AMOLED_HEIGHT        (128)
#define AMOLED_WIDTH         (128)

#define BOARD_HAS_TOUCH      1
#define DISPLAY_FULLRESH     false
#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)



#elif CONFIG_LILYGO_T_RGB

// #define BOARD_POWERON        (15)

#define BOARD_TFT_BL         (gpio_num_t)(46)

#define BOARD_TFT_HSYNC          47
#define BOARD_TFT_VSYNC          41
#define BOARD_TFT_DE             45
#define BOARD_TFT_PCLK           42


#define BOARD_TFT_DATA0      (44)
#define BOARD_TFT_DATA1      (21)
#define BOARD_TFT_DATA2      (18)
#define BOARD_TFT_DATA3      (17)
#define BOARD_TFT_DATA4      (16)
#define BOARD_TFT_DATA5      (15)
#define BOARD_TFT_DATA6      (14)
#define BOARD_TFT_DATA7      (13)
#define BOARD_TFT_DATA8      (12)
#define BOARD_TFT_DATA9      (11)
#define BOARD_TFT_DATA10     (10)
#define BOARD_TFT_DATA11     (9)
#define BOARD_TFT_DATA12     (43)
#define BOARD_TFT_DATA13     (7)
#define BOARD_TFT_DATA14     (6)
#define BOARD_TFT_DATA15     (5)
#define BOARD_TFT_DATA16     (3)
#define BOARD_TFT_DATA17     (2)

#define BOARD_TFT_RST        (6)
#define BOARD_TFT_CS         (3)
#define BOARD_TFT_MOSI       (4)
#define BOARD_TFT_SCLK       (5)

#define BOARD_I2C_SDA        (8)
#define BOARD_I2C_SCL        (48)

#define BOARD_TOUCH_IRQ      (1)
#define BOARD_TOUCH_RST      (1)
#define AMOLED_WIDTH         (480)
#define AMOLED_HEIGHT        (480)

#define SDMMC_EN            (7)
#define BOARD_HAS_TOUCH      1

#define DISPLAY_BUFFER_SIZE  (AMOLED_WIDTH * AMOLED_HEIGHT)

#define DISPLAY_FULLRESH     false

#elif CONFIG_LILYGO_T_WATCH_S3

#define BOARD_SPI_MISO       (-1)
#define BOARD_SPI_MOSI       (13)
#define BOARD_SPI_SCK        (18)
#define BOARD_TFT_CS         (12)
#define BOARD_TFT_RST        (-1)
#define BOARD_TFT_DC         (38)
#define BOARD_TFT_BL         (45)

#define BOARD_I2C_SDA        (10)
#define BOARD_I2C_SCL        (11)

#define BOARD_TOUCH_IRQ      (16)
#define BOARD_TOUCH_RST      (-1)

#define AMOLED_HEIGHT        (240)
#define AMOLED_WIDTH         (240)

#define BOARD_HAS_TOUCH       0         //NOT YET
#define DISPLAY_FULLRESH      false
#define DISPLAY_BUFFER_SIZE   (AMOLED_WIDTH * AMOLED_HEIGHT)
#define CONFIG_PMU_AXP2101  (1)

#else
#error "No select product"
#endif




















