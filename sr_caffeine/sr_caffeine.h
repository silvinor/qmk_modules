// Copyright 2025 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if defined(RGB_MATRIX_ENABLE) || defined(RGB_LIGHT_ENABLE)
#    include "color.h"
#endif

#if defined(RGB_MATRIX_ENABLE) || defined(LED_CAFFEINE_PIN)
void blink_changed_sr_caffeine(bool is_blink_on); // weak
#endif

void matrix_scan_sr_caffeine(void);

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_sr_caffeine(void);
#endif

#ifdef LED_CAFFEINE_PIN
bool led_update_sr_caffeine(led_t led_state);
#endif

void housekeeping_task_sr_caffeine(void); // called by Community Modules

#if defined(RGB_MATRIX_ENABLE) || defined(RGB_LIGHT_ENABLE)

#    ifndef MINMAX
#        define MINMAX(i, MIN, MAX) ((i) < (MIN) ? (MIN) : ((i) > (MAX) ? (MAX) : (i)))
#    endif

static inline rgb_t adjust_to_brightness(uint8_t r, uint8_t g, uint8_t b, uint8_t min, uint8_t max) {
    rgb_t ret;
#    ifdef RGB_MATRIX_ENABLE
    uint8_t brightness = MINMAX(rgb_matrix_get_val(), min, max);
#    elif RGB_LIGHT_ENABLE
    uint8_t brightness = MINMAX(rgblight_get_val(), min, max);
#    endif
    ret.r = (uint16_t)(r * (brightness / 255));
    ret.g = (uint16_t)(g * (brightness / 255));
    ret.b = (uint16_t)(b * (brightness / 255));
    return ret;
}

#endif // RGB_MATRIX_ENABLE || RGB_LIGHT_ENABLE

bool process_keycode_sr_caffeine_on(keyrecord_t *record);
bool process_keycode_sr_caffeine_off(keyrecord_t *record);
bool process_keycode_sr_caffeine_toggle(keyrecord_t *record);
bool process_record_sr_caffeine(uint16_t keycode, keyrecord_t *record); // called by Community Modules
