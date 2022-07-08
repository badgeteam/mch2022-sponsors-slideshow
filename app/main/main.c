
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
extern const uint8_t logo2_db_png_start[]             asm("_binary_2_logo_db_png_start");
extern const uint8_t logo2_mullvad_vpn_png_start[]    asm("_binary_2_logo_mullvad_vpn_png_start");
extern const uint8_t logo2_secura_png_start[]         asm("_binary_2_logo_secura_png_start");
extern const uint8_t logo2_schubergphilis_png_start[] asm("_binary_2_logo_schubergphilis_png_start");
extern const uint8_t logo2_true_png_start[]           asm("_binary_2_logo_true_png_start");
extern const uint8_t logo3_a2b_kuc_png_start[]        asm("_binary_3_logo_a2b_kuc_png_start");
extern const uint8_t logo3_access42_png_start[]       asm("_binary_3_logo_access42_png_start");
extern const uint8_t logo3_at_computing_png_start[]   asm("_binary_3_logo_at_computing_png_start");
extern const uint8_t logo3_protagio_png_start[]       asm("_binary_3_logo_protagio_png_start");
extern const uint8_t logo3_s_rm_png_start[]           asm("_binary_3_logo_s_rm_png_start");
extern const uint8_t logo3_s_unit_png_start[]         asm("_binary_3_logo_s_unit_png_start");
extern const uint8_t logo3_stegen_png_start[]         asm("_binary_3_logo_stegen_png_start");
extern const uint8_t logo3_transip_png_start[]        asm("_binary_3_logo_transip_png_start");
extern const uint8_t logo3_zerocopter_png_start[]     asm("_binary_3_logo_zerocopter_png_start");

extern const uint8_t logo1_adyen_png_end[]          asm("_binary_1_logo_adyen_png_end");
extern const uint8_t logo1_computest_png_end[]      asm("_binary_1_logo_computest_png_end");
extern const uint8_t logo1_deloitte_png_end[]       asm("_binary_1_logo_deloitte_png_end");
extern const uint8_t logo2_db_png_end[]             asm("_binary_2_logo_db_png_end");
extern const uint8_t logo2_mullvad_vpn_png_end[]    asm("_binary_2_logo_mullvad_vpn_png_end");
extern const uint8_t logo2_secura_png_end[]         asm("_binary_2_logo_secura_png_end");
extern const uint8_t logo2_schubergphilis_png_end[] asm("_binary_2_logo_schubergphilis_png_end");
extern const uint8_t logo2_true_png_end[]           asm("_binary_2_logo_true_png_end");
extern const uint8_t logo3_a2b_kuc_png_end[]        asm("_binary_3_logo_a2b_kuc_png_end");
extern const uint8_t logo3_access42_png_end[]       asm("_binary_3_logo_access42_png_end");
extern const uint8_t logo3_at_computing_png_end[]   asm("_binary_3_logo_at_computing_png_end");
extern const uint8_t logo3_protagio_png_end[]       asm("_binary_3_logo_protagio_png_end");
extern const uint8_t logo3_s_rm_png_end[]           asm("_binary_3_logo_s_rm_png_end");
extern const uint8_t logo3_s_unit_png_end[]         asm("_binary_3_logo_s_unit_png_end");
extern const uint8_t logo3_stegen_png_end[]         asm("_binary_3_logo_stegen_png_end");
extern const uint8_t logo3_transip_png_end[]        asm("_binary_3_logo_transip_png_end");
extern const uint8_t logo3_zerocopter_png_end[]     asm("_binary_3_logo_zerocopter_png_end");

const void *event_start_regions[] = {
    logo1_adyen_png_start,
    logo1_computest_png_start,
    logo1_deloitte_png_start,
    logo2_db_png_start,
    logo2_mullvad_vpn_png_start,
    logo2_secura_png_start,
    logo2_schubergphilis_png_start,
    logo2_true_png_start,
    logo3_a2b_kuc_png_start,
    logo3_access42_png_start,
    logo3_at_computing_png_start,
    logo3_protagio_png_start,
    logo3_s_rm_png_start,
    logo3_s_unit_png_start,
    logo3_stegen_png_start,
    logo3_transip_png_start,
    logo3_zerocopter_png_start,
};

const void *event_end_regions[] = {
    logo1_adyen_png_end,
    logo1_computest_png_end,
    logo1_deloitte_png_end,
    logo2_db_png_end,
    logo2_mullvad_vpn_png_end,
    logo2_secura_png_end,
    logo2_schubergphilis_png_end,
    logo2_true_png_end,
    logo3_a2b_kuc_png_end,
    logo3_access42_png_end,
    logo3_at_computing_png_end,
    logo3_protagio_png_end,
    logo3_s_rm_png_end,
    logo3_s_unit_png_end,
    logo3_stegen_png_end,
    logo3_transip_png_end,
    logo3_zerocopter_png_end,
};

extern const uint8_t logom_allnet_png_start[]       asm("_binary_m_logo_allnet_png_start");
extern const uint8_t logom_bosch_png_start[]        asm("_binary_m_logo_bosch_png_start");
extern const uint8_t logom_espressif_png_start[]    asm("_binary_m_logo_espressif_png_start");
extern const uint8_t logom_lattice_png_start[]      asm("_binary_m_logo_lattice_png_start");
extern const uint8_t logom_rpi_png_start[]          asm("_binary_m_logo_rpi_png_start");

extern const uint8_t logom_allnet_png_end[]       asm("_binary_m_logo_allnet_png_end");
extern const uint8_t logom_bosch_png_end[]        asm("_binary_m_logo_bosch_png_end");
extern const uint8_t logom_espressif_png_end[]    asm("_binary_m_logo_espressif_png_end");
extern const uint8_t logom_lattice_png_end[]      asm("_binary_m_logo_lattice_png_end");
extern const uint8_t logom_rpi_png_end[]          asm("_binary_m_logo_rpi_png_end");

const void *badge_start_regions[] = {
    logom_allnet_png_start,
    logom_bosch_png_start,
    logom_espressif_png_start,
    logom_lattice_png_start,
    logom_rpi_png_start,
};

const void *badge_end_regions[] = {
    logom_allnet_png_end,
    logom_bosch_png_end,
    logom_espressif_png_end,
    logom_lattice_png_end,
    logom_rpi_png_end,
};

const size_t num_anim  = sizeof(anim_start_regions)  / sizeof(const void *);
const size_t num_event = sizeof(event_start_regions) / sizeof(const void *);
const size_t num_badge = sizeof(badge_start_regions) / sizeof(const void *);
static uint8_t has_watched = 0;

#define EVENT_IMAGE_TIME pdMS_TO_TICKS(1000)
#define BADGE_IMAGE_TIME pdMS_TO_TICKS(1000)

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
    float delta = (float) buf.width / total;
    // Draw dots.
    float x     = (delta - 4) / 2;
    for (int i = 0; i < total; i++) {
        // Make the dot darker when it's selected.
        pax_col_t color = part == i ? 0xff000000 : 0xffc0c0c0;
        pax_draw_rect(&buf, color, x, buf.height - 10, 4, 4);
        // Move to the next dot's position.
        x += delta;
    }
}

// Displays the skip slideshow text, but only if it has been watched before.
void skip_display() {
    if (has_watched) {
        const pax_font_t *font = pax_font_saira_regular;
        pax_draw_text(&buf, 0xff000000, font, 18, 5, 5, "Press 🅷 to skip.");
    }
}

// Allows exiting the slideshow, but only if it has been watched before.
void skip_check(xQueueHandle buttonQueue, BaseType_t delay) {
    if (has_watched) {
        BaseType_t time = pdMS_TO_TICKS(esp_timer_get_time() / 1000);
        BaseType_t end  = time + delay;
        do {
            rp2040_input_message_t msg;
            if (xQueueReceive(buttonQueue, &msg, end - time)
                && msg.input == RP2040_INPUT_BUTTON_HOME && msg.state) {
                // Go to launcher.
                REG_WRITE(RTC_CNTL_STORE0_REG, 0);
                esp_restart();
            }
            
            time = pdMS_TO_TICKS(esp_timer_get_time() / 1000);
        } while (time < end);
    } else {
        vTaskDelay(delay);
    }
}

void app_main() {
    // Init HW.
    bsp_init();
    bsp_rp2040_init();
    buttonQueue = get_rp2040()->queue;
    
    // Init GFX.
    pax_buf_init(&buf, NULL, 320, 240, PAX_BUF_16_565RGB);
    
    // Power on LED region.
    uint8_t leds[15];
    memset(&leds, 0, sizeof(leds));
    gpio_set_direction(GPIO_SD_PWR, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_SD_PWR, true);
    // Init LEDs to off.
    ws2812_init(GPIO_LED_DATA);
    ws2812_send_data(leds, sizeof(leds));
    
    // Init NVS.
    nvs_flash_init();
    nvs_handle_t handle;
    nvs_open("system", NVS_READWRITE, &handle);
    has_watched = 0;
    nvs_get_u8(handle, "sponsors", &has_watched);
    ESP_LOGI(TAG, "User has%s watched demo before.", has_watched ? "" : " not");
    
    // Show the badger animation.
    for (int i = 0; i < num_anim; i++) {
        display_logo(anim_start_regions[i], anim_end_regions[i]);
        // If the user has watched before, allow them to skip.
        skip_display();
        skip_check(buttonQueue, 0);
        disp_flush();
        // On frames 23 -> 27, show something on the leds.
        if (i >= 23) {
            int part0 = 127 * (i - 22) / 5;
            uint8_t part = part0;
            
            /*
            // Center of the kite: green.
            leds.leds[0].green = part;
            // Left of the kite: red.
            leds.leds[1].red   = part;
            // Top of the kit: blue.
            leds.leds[2].blue  = part;
            // Right of the kite: yellow.
            leds.leds[3].red   = part;
            leds.leds[3].green = part;
            // Bottom of the kite: blue.
            leds.leds[4].blue  = part;
            */
            
            // Center of the kite: green.
            leds[3*0 + 1] = part;
            // Left of the kite: red.
            leds[3*1 + 0] = part;
            // Top of the kit: blue.
            leds[3*2 + 2]  = part;
            // Right of the kite: yellow.
            leds[3*3 + 0] = part;
            leds[3*3 + 1] = part;
            // Bottom of the kite: blue.
            leds[3*4 + 2] = part;
            
            // TODO
            // ws2812_send_data(leds, sizeof(leds));
        }
    }
    
    // Show the event sponsors.
    for (int i = 0; i < num_event; i++) {
        display_logo(event_start_regions[i], event_end_regions[i]);
        // Show a progress bar so people know how long to expect.
        display_progress(i, num_event+num_badge);
        skip_display();
        disp_flush();
        skip_check(buttonQueue, EVENT_IMAGE_TIME);
    }
    
    // Show the badge sponsors.
    for (int i = 0; i < num_badge; i++) {
        display_logo(badge_start_regions[i], badge_end_regions[i]);
        // Show a progress bar so people know how long to expect.
        display_progress(num_event+i, num_event+num_badge);
        skip_display();
        disp_flush();
        skip_check(buttonQueue, BADGE_IMAGE_TIME);
    }
    
    // Done!
    pax_background(&buf, -1);
    disp_flush();
    
    // Set an NVS variable.
    nvs_set_u8(handle, "sponsors", 1);
    nvs_commit(handle);
    nvs_close(handle);
    
    // Go to launcher.
    REG_WRITE(RTC_CNTL_STORE0_REG, 0);
    esp_restart();
}
