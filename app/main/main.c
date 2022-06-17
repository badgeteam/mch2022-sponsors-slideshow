
#include "main.h"
#include "hardware.h"
#include "pax_gfx.h"
#include "pax_codecs.h"
#include "ili9341.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "nvs.h"
#include "soc/rtc.h"
#include "soc/rtc_cntl_reg.h"
#include <stdlib.h>
#include <ws2812.h>

pax_buf_t buf;
xQueueHandle buttonQueue;

static const char *TAG = "spon";

extern const uint8_t logo1_adyen_png_start[]        asm("_binary_1_logo_adyen_png_start");
extern const uint8_t logo1_computest_png_start[]    asm("_binary_1_logo_computest_png_start");
extern const uint8_t logo1_deloitte_png_start[]     asm("_binary_1_logo_deloitte_png_start");
extern const uint8_t logo2_mullvad_vpn_png_start[]  asm("_binary_2_logo_mullvad_vpn_png_start");
extern const uint8_t logo2_secura_png_start[]       asm("_binary_2_logo_secura_png_start");
extern const uint8_t logo3_stegen_png_start[]       asm("_binary_3_logo_stegen_png_start");
extern const uint8_t logo3_access42_png_start[]     asm("_binary_3_logo_access42_png_start");
extern const uint8_t logo3_at_computing_png_start[] asm("_binary_3_logo_at_computing_png_start");
extern const uint8_t logo3_zerocopter_png_start[]   asm("_binary_3_logo_zerocopter_png_start");

extern const uint8_t logo1_adyen_png_end[]        asm("_binary_1_logo_adyen_png_end");
extern const uint8_t logo1_computest_png_end[]    asm("_binary_1_logo_computest_png_end");
extern const uint8_t logo1_deloitte_png_end[]     asm("_binary_1_logo_deloitte_png_end");
extern const uint8_t logo2_mullvad_vpn_png_end[]  asm("_binary_2_logo_mullvad_vpn_png_end");
extern const uint8_t logo2_secura_png_end[]       asm("_binary_2_logo_secura_png_end");
extern const uint8_t logo3_stegen_png_end[]       asm("_binary_3_logo_stegen_png_end");
extern const uint8_t logo3_access42_png_end[]     asm("_binary_3_logo_access42_png_end");
extern const uint8_t logo3_at_computing_png_end[] asm("_binary_3_logo_at_computing_png_end");
extern const uint8_t logo3_zerocopter_png_end[]   asm("_binary_3_logo_zerocopter_png_end");

const void *start_regions[] = {
    logo1_adyen_png_start,
    logo1_computest_png_start,
    logo1_deloitte_png_start,
    logo2_mullvad_vpn_png_start,
    logo2_secura_png_start,
    logo3_stegen_png_start,
    logo3_access42_png_start,
    logo3_at_computing_png_start,
    logo3_zerocopter_png_start,
};

const void *end_regions[] = {
    logo1_adyen_png_end,
    logo1_computest_png_end,
    logo1_deloitte_png_end,
    logo2_mullvad_vpn_png_end,
    logo2_secura_png_end,
    logo3_stegen_png_end,
    logo3_access42_png_end,
    logo3_at_computing_png_end,
    logo3_zerocopter_png_end,
};

const size_t num_logos = sizeof(start_regions) / sizeof(const void *);

#define IMAGE_TIME pdMS_TO_TICKS(500)

void disp_flush() {
    ili9341_write(get_ili9341(), buf.buf);
}

void display_logo(const void *start, const void *end, char *text) {
    size_t len = (size_t) end - (size_t) start;
    pax_decode_png_buf(&buf, (void *) start, len, buf.type, CODEC_FLAG_EXISTING);
    if (text) {
        pax_col_t col = pax_get_pixel(&buf, 0, 0) ^ 0x00ffffff;
        col |= 0xff000000;
        pax_draw_text(&buf, col, pax_get_font("saira regular"), 18, 10, 10, text);
    }
    disp_flush();
}

void app_main() {
    // Init HW.
    bsp_init();
    bsp_rp2040_init();
    buttonQueue = get_rp2040()->queue;
    
    // Init GFX.
    pax_buf_init(&buf, NULL, 320, 240, PAX_BUF_16_565RGB);
    
    // Try to LEDINAR.
    uint8_t ledinar[15];
    memset(ledinar, 255, sizeof(ledinar));
    esp_err_t res = ws2812_init(GPIO_LED_DATA);
    
    // Init NVS.
    nvs_flash_init();
    nvs_handle_t handle;
    nvs_open("system", NVS_READWRITE, &handle);
    uint8_t has_watched = 0;
    nvs_get_u8(handle, "sponsors", &has_watched);
    ESP_LOGI(TAG, "User has%s watched demo before.", has_watched ? "" : " not");
    
    // Show ALL of them.
    for (int i = 0; i < num_logos; i++) {
        display_logo(start_regions[i], end_regions[i], NULL);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
    // Set an NVS variable.
    nvs_set_u8(handle, "sponsors", 1);
    nvs_close(handle);
    
    // Done!
    pax_background(&buf, -1);
    disp_flush();
    
    // Go to launcher.
    REG_WRITE(RTC_CNTL_STORE0_REG, 0);
    esp_restart();
}
