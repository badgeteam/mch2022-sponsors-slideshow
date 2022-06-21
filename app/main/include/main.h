
#include "hardware.h"
#include "pax_gfx.h"
#include "ili9341.h"

#pragma once

void disp_flush();
void display_logo(const void *start, const void *end);
void display_progress(int part, int total);
