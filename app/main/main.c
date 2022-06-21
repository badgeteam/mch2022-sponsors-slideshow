
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
#include <string.h>
#include <ws2812.h>

pax_buf_t buf;
xQueueHandle buttonQueue;

static const char *TAG = "spon";

extern const uint8_t animation_frame_1_png_start[]  asm("_binary_animation_frame_1_png_start");
extern const uint8_t animation_frame_2_png_start[]  asm("_binary_animation_frame_2_png_start");
extern const uint8_t animation_frame_3_png_start[]  asm("_binary_animation_frame_3_png_start");
extern const uint8_t animation_frame_4_png_start[]  asm("_binary_animation_frame_4_png_start");
extern const uint8_t animation_frame_5_png_start[]  asm("_binary_animation_frame_5_png_start");
extern const uint8_t animation_frame_6_png_start[]  asm("_binary_animation_frame_6_png_start");
extern const uint8_t animation_frame_7_png_start[]  asm("_binary_animation_frame_7_png_start");
extern const uint8_t animation_frame_8_png_start[]  asm("_binary_animation_frame_8_png_start");
extern const uint8_t animation_frame_9_png_start[]  asm("_binary_animation_frame_9_png_start");
extern const uint8_t animation_frame_10_png_start[] asm("_binary_animation_frame_10_png_start");
extern const uint8_t animation_frame_11_png_start[] asm("_binary_animation_frame_11_png_start");
extern const uint8_t animation_frame_12_png_start[] asm("_binary_animation_frame_12_png_start");
extern const uint8_t animation_frame_13_png_start[] asm("_binary_animation_frame_13_png_start");
extern const uint8_t animation_frame_14_png_start[] asm("_binary_animation_frame_14_png_start");
extern const uint8_t animation_frame_15_png_start[] asm("_binary_animation_frame_15_png_start");
extern const uint8_t animation_frame_16_png_start[] asm("_binary_animation_frame_16_png_start");
extern const uint8_t animation_frame_17_png_start[] asm("_binary_animation_frame_17_png_start");
extern const uint8_t animation_frame_18_png_start[] asm("_binary_animation_frame_18_png_start");
extern const uint8_t animation_frame_19_png_start[] asm("_binary_animation_frame_19_png_start");
extern const uint8_t animation_frame_20_png_start[] asm("_binary_animation_frame_20_png_start");
extern const uint8_t animation_frame_21_png_start[] asm("_binary_animation_frame_21_png_start");
extern const uint8_t animation_frame_22_png_start[] asm("_binary_animation_frame_22_png_start");
extern const uint8_t animation_frame_23_png_start[] asm("_binary_animation_frame_23_png_start");
extern const uint8_t animation_frame_24_png_start[] asm("_binary_animation_frame_24_png_start");
extern const uint8_t animation_frame_25_png_start[] asm("_binary_animation_frame_25_png_start");
extern const uint8_t animation_frame_26_png_start[] asm("_binary_animation_frame_26_png_start");
extern const uint8_t animation_frame_27_png_start[] asm("_binary_animation_frame_27_png_start");
extern const uint8_t animation_frame_28_png_start[] asm("_binary_animation_frame_28_png_start");

extern const uint8_t animation_frame_1_png_end[]  asm("_binary_animation_frame_1_png_end");
extern const uint8_t animation_frame_2_png_end[]  asm("_binary_animation_frame_2_png_end");
extern const uint8_t animation_frame_3_png_end[]  asm("_binary_animation_frame_3_png_end");
extern const uint8_t animation_frame_4_png_end[]  asm("_binary_animation_frame_4_png_end");
extern const uint8_t animation_frame_5_png_end[]  asm("_binary_animation_frame_5_png_end");
extern const uint8_t animation_frame_6_png_end[]  asm("_binary_animation_frame_6_png_end");
extern const uint8_t animation_frame_7_png_end[]  asm("_binary_animation_frame_7_png_end");
extern const uint8_t animation_frame_8_png_end[]  asm("_binary_animation_frame_8_png_end");
extern const uint8_t animation_frame_9_png_end[]  asm("_binary_animation_frame_9_png_end");
extern const uint8_t animation_frame_10_png_end[] asm("_binary_animation_frame_10_png_end");
extern const uint8_t animation_frame_11_png_end[] asm("_binary_animation_frame_11_png_end");
extern const uint8_t animation_frame_12_png_end[] asm("_binary_animation_frame_12_png_end");
extern const uint8_t animation_frame_13_png_end[] asm("_binary_animation_frame_13_png_end");
extern const uint8_t animation_frame_14_png_end[] asm("_binary_animation_frame_14_png_end");
extern const uint8_t animation_frame_15_png_end[] asm("_binary_animation_frame_15_png_end");
extern const uint8_t animation_frame_16_png_end[] asm("_binary_animation_frame_16_png_end");
extern const uint8_t animation_frame_17_png_end[] asm("_binary_animation_frame_17_png_end");
extern const uint8_t animation_frame_18_png_end[] asm("_binary_animation_frame_18_png_end");
extern const uint8_t animation_frame_19_png_end[] asm("_binary_animation_frame_19_png_end");
extern const uint8_t animation_frame_20_png_end[] asm("_binary_animation_frame_20_png_end");
extern const uint8_t animation_frame_21_png_end[] asm("_binary_animation_frame_21_png_end");
extern const uint8_t animation_frame_22_png_end[] asm("_binary_animation_frame_22_png_end");
extern const uint8_t animation_frame_23_png_end[] asm("_binary_animation_frame_23_png_end");
extern const uint8_t animation_frame_24_png_end[] asm("_binary_animation_frame_24_png_end");
extern const uint8_t animation_frame_25_png_end[] asm("_binary_animation_frame_25_png_end");
extern const uint8_t animation_frame_26_png_end[] asm("_binary_animation_frame_26_png_end");
extern const uint8_t animation_frame_27_png_end[] asm("_binary_animation_frame_27_png_end");
extern const uint8_t animation_frame_28_png_end[] asm("_binary_animation_frame_28_png_end");

const void *anim_start_regions[] = {
    animation_frame_1_png_start,
    animation_frame_2_png_start,
    animation_frame_3_png_start,
    animation_frame_4_png_start,
    animation_frame_5_png_start,
    animation_frame_6_png_start,
    animation_frame_7_png_start,
    animation_frame_8_png_start,
    animation_frame_9_png_start,
    animation_frame_10_png_start,
    animation_frame_11_png_start,
    animation_frame_12_png_start,
    animation_frame_13_png_start,
    animation_frame_14_png_start,
    animation_frame_15_png_start,
    animation_frame_16_png_start,
    animation_frame_17_png_start,
    animation_frame_18_png_start,
    animation_frame_19_png_start,
    animation_frame_20_png_start,
    animation_frame_21_png_start,
    animation_frame_22_png_start,
    animation_frame_23_png_start,
    animation_frame_24_png_start,
    animation_frame_25_png_start,
    animation_frame_26_png_start,
    animation_frame_27_png_start,
    animation_frame_28_png_start,
};

const void *anim_end_regions[] = {
    animation_frame_1_png_end,
    animation_frame_2_png_end,
    animation_frame_3_png_end,
    animation_frame_4_png_end,
    animation_frame_5_png_end,
    animation_frame_6_png_end,
    animation_frame_7_png_end,
    animation_frame_8_png_end,
    animation_frame_9_png_end,
    animation_frame_10_png_end,
    animation_frame_11_png_end,
    animation_frame_12_png_end,
    animation_frame_13_png_end,
    animation_frame_14_png_end,
    animation_frame_15_png_end,
    animation_frame_16_png_end,
    animation_frame_17_png_end,
    animation_frame_18_png_end,
    animation_frame_19_png_end,
    animation_frame_20_png_end,
    animation_frame_21_png_end,
    animation_frame_22_png_end,
    animation_frame_23_png_end,
    animation_frame_24_png_end,
    animation_frame_25_png_end,
    animation_frame_26_png_end,
    animation_frame_27_png_end,
    animation_frame_28_png_end,
};

extern const uint8_t logo1_adyen_png_start[]          asm("_binary_1_logo_adyen_png_start");
extern const uint8_t logo1_computest_png_start[]      asm("_binary_1_logo_computest_png_start");
extern const uint8_t logo1_deloitte_png_start[]       asm("_binary_1_logo_deloitte_png_start");
extern const uint8_t logo1_schubergphilis_png_start[] asm("_binary_1_logo_schubergphilis_png_start");
extern const uint8_t logo2_mullvad_vpn_png_start[]    asm("_binary_2_logo_mullvad_vpn_png_start");
extern const uint8_t logo2_s_unit_png_start[]         asm("_binary_2_logo_s_unit_png_start");
extern const uint8_t logo2_secura_png_start[]         asm("_binary_2_logo_secura_png_start");
extern const uint8_t logo2_teamblue_png_start[]       asm("_binary_2_logo_teamblue_png_start");
extern const uint8_t logo3_stegen_png_start[]         asm("_binary_3_logo_stegen_png_start");
extern const uint8_t logo3_access42_png_start[]       asm("_binary_3_logo_access42_png_start");
extern const uint8_t logo3_at_computing_png_start[]   asm("_binary_3_logo_at_computing_png_start");
extern const uint8_t logo3_zerocopter_png_start[]     asm("_binary_3_logo_zerocopter_png_start");

extern const uint8_t logo1_adyen_png_end[]          asm("_binary_1_logo_adyen_png_end");
extern const uint8_t logo1_computest_png_end[]      asm("_binary_1_logo_computest_png_end");
extern const uint8_t logo1_deloitte_png_end[]       asm("_binary_1_logo_deloitte_png_end");
extern const uint8_t logo1_schubergphilis_png_end[] asm("_binary_1_logo_schubergphilis_png_end");
extern const uint8_t logo2_mullvad_vpn_png_end[]    asm("_binary_2_logo_mullvad_vpn_png_end");
extern const uint8_t logo2_s_unit_png_end[]         asm("_binary_2_logo_s_unit_png_end");
extern const uint8_t logo2_secura_png_end[]         asm("_binary_2_logo_secura_png_end");
extern const uint8_t logo2_teamblue_png_end[]       asm("_binary_2_logo_teamblue_png_end");
extern const uint8_t logo3_stegen_png_end[]         asm("_binary_3_logo_stegen_png_end");
extern const uint8_t logo3_access42_png_end[]       asm("_binary_3_logo_access42_png_end");
extern const uint8_t logo3_at_computing_png_end[]   asm("_binary_3_logo_at_computing_png_end");
extern const uint8_t logo3_zerocopter_png_end[]     asm("_binary_3_logo_zerocopter_png_end");

const void *event_start_regions[] = {
    logo1_adyen_png_start,
    logo1_computest_png_start,
    logo1_deloitte_png_start,
    logo1_schubergphilis_png_start,
    logo2_mullvad_vpn_png_start,
    logo2_s_unit_png_start,
    logo2_secura_png_start,
    logo2_teamblue_png_start,
    logo3_stegen_png_start,
    logo3_access42_png_start,
    logo3_at_computing_png_start,
    logo3_zerocopter_png_start,
};

const void *event_end_regions[] = {
    logo1_adyen_png_end,
    logo1_computest_png_end,
    logo1_deloitte_png_end,
    logo1_schubergphilis_png_end,
    logo2_mullvad_vpn_png_end,
    logo2_s_unit_png_end,
    logo2_secura_png_end,
    logo2_teamblue_png_end,
    logo3_stegen_png_end,
    logo3_access42_png_end,
    logo3_at_computing_png_end,
    logo3_zerocopter_png_end,
};

const size_t num_anim  = sizeof(anim_start_regions) / sizeof(const void *);
const size_t num_event = sizeof(event_start_regions) / sizeof(const void *);

#define IMAGE_TIME pdMS_TO_TICKS(500)

// Updates the display with what's been drawn.
void disp_flush() {
    ili9341_write(get_ili9341(), buf.buf);
}

// Draws a single PNG.
void display_logo(const void *start, const void *end) {
    size_t len = (size_t) end - (size_t) start;
    pax_decode_png_buf(&buf, (void *) start, len, buf.type, CODEC_FLAG_EXISTING);
}

// Draws a progress bar on the bottom of the screen.
void display_progress(int part, int total) {
    // Calculate spacing between dots.
    float delta = buf.width / (total + 1);
    // Draw dots.
    float x     = delta;
    for (int i = 0; i < total; i++) {
        // Make the dot darker when it's selected.
        pax_col_t color = part == i ? 0xff000000 : 0xffa0a0a0;
        pax_draw_circle(&buf, color, x, buf.height - 10, 4);
        // Move to the next dot's position.
        x += delta;
    }
}

void app_main() {
    // Init HW.
    bsp_init();
    bsp_rp2040_init();
    buttonQueue = get_rp2040()->queue;
    
    // Init GFX.
    pax_buf_init(&buf, NULL, 320, 240, PAX_BUF_16_565RGB);
    
    // TODO: Power on LED region and show something on them.
    
    // Init NVS.
    nvs_flash_init();
    nvs_handle_t handle;
    nvs_open("system", NVS_READWRITE, &handle);
    uint8_t has_watched = 0;
    nvs_get_u8(handle, "sponsors", &has_watched);
    ESP_LOGI(TAG, "User has%s watched demo before.", has_watched ? "" : " not");
    
    // Show the badger animation.
    for (int i = 0; i < num_anim; i++) {
        display_logo(anim_start_regions[i], anim_end_regions[i]);
        disp_flush();
    }
    
    // Show the event sponsors.
    for (int i = 0; i < num_event; i++) {
        display_logo(event_start_regions[i], event_end_regions[i]);
        // Show a progress bar so people know how long to expect.
        display_progress(i, num_event);
        disp_flush();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
    // TODO: Show hardware sponsors.
    
    // Set an NVS variable.
    nvs_set_u8(handle, "sponsors", 1);
    nvs_commit(handle);
    nvs_close(handle);
    
    // Done!
    pax_background(&buf, -1);
    disp_flush();
    
    // Go to launcher.
    REG_WRITE(RTC_CNTL_STORE0_REG, 0);
    esp_restart();
}
