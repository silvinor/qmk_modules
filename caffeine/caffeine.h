// Copyright 2025 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#ifndef COMMUNITY_MODULE_SILVINOR_CAFFEINE_ENABLE
#    define COMMUNITY_MODULE_SILVINOR_CAFFEINE_ENABLE

void matrix_scan_caffeine(void);
void housekeeping_task_caffeine(void);

#    if defined(RGB_MATRIX_ENABLE) || defined(RGB_LIGHT_ENABLE)

#        ifndef MINMAX
#            define MINMAX(i, MIN, MAX) ((i) < (MIN) ? (MIN) : ((i) > (MAX) ? (MAX) : (i)))
#        endif

static inline RGB adjust_to_brightness(uint8_t r, uint8_t g, uint8_t b, uint8_t min, uint8_t max) {
    RGB ret;
#        ifdef RGB_MATRIX_ENABLE
    uint8_t brightness = MINMAX( rgb_matrix_get_val(), min, max );
#        elif RGB_LIGHT_ENABLE
    uint8_t brightness = MINMAX( rgblight_get_val(), min, max );
#        endif
    ret.r = (uint16_t)(r * (brightness / 255));
    ret.g = (uint16_t)(g * (brightness / 255));
    ret.b = (uint16_t)(b * (brightness / 255));
    return ret;
}

#    endif

#    ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_caffeine(void);
#    endif

bool process_keycode_caffeine_on(keyrecord_t *record);
bool process_keycode_caffeine_off(keyrecord_t *record);
bool process_keycode_caffeine_toggle(keyrecord_t *record);
bool process_record_caffeine(uint16_t keycode, keyrecord_t *record);

#endif
