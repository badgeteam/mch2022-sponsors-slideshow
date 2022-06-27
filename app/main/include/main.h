
#include "hardware.h"
#include "pax_gfx.h"
#include "ili9341.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#pragma once

void disp_flush();
void display_logo(const void *start, const void *end);
void display_progress(int part, int total);
void skip_display();
void skip_check(xQueueHandle buttonQueue, BaseType_t delay);
