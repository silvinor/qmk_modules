// Copyright 2025 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H
#include "sr_caffeine.h"
#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif

#define RGB_MATRIX_MINIMUM_BRIGHTNESS 64

#if defined(RGB_MATRIX_ENABLE) || defined(LED_CAFFEINE_PIN)
#    ifndef CAFFEINE_BLINK_DELAY
#        define CAFFEINE_BLINK_DELAY 1000
#    endif // CAFFEINE_BLINK_DELAY
#endif

#ifdef LED_CAFFEINE_PIN
#    ifndef LED_PIN_ON_STATE
#        define LED_PIN_ON_STATE 1
#    endif
#endif

#ifndef CAFFEINE_KEY_DELAY
#    define CAFFEINE_KEY_DELAY 59000 // 1 sec shy of 1 minute
#endif

#ifndef CAFFEINE_KEY_CODE
#    define CAFFEINE_KEY_CODE KC_RIGHT_CTRL // Right Control
#endif

bool     is_caffeine_on        = false; // No Screen Saver
uint32_t timer_caffeine_buffer = 0;     // No Screen Saver timer buffer

#ifdef RGB_MATRIX_ENABLE
uint8_t caffeine_key_index = UINT8_MAX;
rgb_t   caffeine_color;
uint8_t caffeine_color_loop = 0;
bool    is_blink_rgb_on     = false; // Used by blink LED timer buffer
#endif

#ifdef LED_CAFFEINE_PIN
bool is_blink_led_on = false; // Used by blink LED timer buffer
#endif

#if defined(RGB_MATRIX_ENABLE) || defined(LED_CAFFEINE_PIN)
uint32_t timer_blink_buffer = 0; // Blink LED timer buffer
#endif

#ifdef MOUSEKEY_ENABLE
uint8_t mouse_loop = 0; // keep track of the jiggle mouse movements
#endif

/**
 * Internal keycode pusher ---------------------------------------------------
 */
void __caffeine_tap_jiggle(bool mouseMove) {
#ifdef CONSOLE_ENABLE
    print("*** Caffeine Tap/Jiggle\n");
#endif

    // --- tap a key ---
    tap_code(CAFFEINE_KEY_CODE);

#ifdef MOUSEKEY_ENABLE
    // --- mouse jiggle ---
    //       ┌───┐
    //       │ 0 │      0 -> Up Down
    //   ┌───┼───┼───┐  1 -> Right Left
    //   │ 3 │ ╳ │ 1 │  2 -> Down Up
    //   └───┼───┼───┘  3 -> Left Right
    //       │ 2 │
    //       └───┘
    if (mouseMove) {
        switch (mouse_loop) {
            case 1:
                tap_code(KC_MS_RIGHT);
                tap_code(KC_MS_LEFT);
                break;
            case 2:
                tap_code(KC_MS_DOWN);
                tap_code(KC_MS_UP);
                break;
            case 3:
                tap_code(KC_MS_LEFT);
                tap_code(KC_MS_RIGHT);
                break;
            default:
                tap_code(KC_MS_UP);
                tap_code(KC_MS_DOWN);
                break;
        }
        mouse_loop = (mouse_loop + 1) % 4;
    }
#endif // MOUSEKEY_ENABLE
}

/**
 * Blink State Has Changed ---------------------------------------------------
 */
#if defined(RGB_MATRIX_ENABLE) || defined(LED_CAFFEINE_PIN)
__attribute__((weak)) void blink_changed_sr_caffeine(bool is_blink_on) {
#    ifdef RGB_MATRIX_ENABLE
    if (is_blink_on) {
        HSV hsv;
        hsv.h = caffeine_color_loop * 51;
        hsv.s = 255;
        hsv.v = MINMAX(rgb_matrix_get_val() + RGB_MATRIX_VAL_STEP, RGB_MATRIX_MINIMUM_BRIGHTNESS, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
        // set the new color
        caffeine_color      = hsv_to_rgb(hsv);
        caffeine_color_loop = (caffeine_color_loop + 1) % 6;
    }
#    endif
}
#endif

/**
 * Loop Scan -----------------------------------------------------------------
 * !! : if you're using as a module, you will need to call this from within a `matrix_scan_user` function
 *   #ifdef COMMUNITY_MODULE_CAFFEINE_ENABLE
 *   void matrix_scan_user(void) {}
 *   #endif
 */
void matrix_scan_sr_caffeine(void) {
    if (is_caffeine_on) {
#if defined(RGB_MATRIX_ENABLE) || defined(LED_CAFFEINE_PIN)
        // Check for elapsed timer
        if (sync_timer_elapsed32(timer_blink_buffer) > CAFFEINE_BLINK_DELAY) {
            timer_blink_buffer = sync_timer_read32(); // reset timer
#    ifdef RGB_MATRIX_ENABLE
            is_blink_rgb_on = !is_blink_rgb_on;
#        ifndef LED_CAFFEINE_PIN // stop double up on `blink_changed_sr_caffeine` call
            blink_changed_sr_caffeine(is_blink_rgb_on);
#        endif
#    endif
#    ifdef LED_CAFFEINE_PIN
            is_blink_led_on = !is_blink_led_on;
            blink_changed_sr_caffeine(is_blink_led_on);
#    endif
        }
#endif

#ifdef LED_CAFFEINE_PIN
        // Set the pin state. Not the best place for this but don't know there is a better place.
        gpio_write_pin(LED_CAFFEINE_PIN, is_blink_led_on ? LED_PIN_ON_STATE : !LED_PIN_ON_STATE);
#endif

        /* Keycode and Mouse Jiggle code */
        if (sync_timer_elapsed32(timer_caffeine_buffer) > CAFFEINE_KEY_DELAY) { // default = 59 sec
            timer_caffeine_buffer = sync_timer_read32();                        // reset timer
            __caffeine_tap_jiggle(true);
        }
    }
}

/**
 * Loop Housekeeping ---------------------------------------------------------
 * Note : if you're using as a module, this will get called by the module compile
 */
void housekeeping_task_sr_caffeine(void) {
#ifdef RGB_MATRIX_ENABLE
    // switch off blinking if RGB has been toggled off
    if (is_blink_rgb_on && !rgb_matrix_is_enabled()) {
        is_blink_rgb_on = false;
        if (caffeine_key_index != UINT8_MAX) {
            rgb_matrix_set_color(caffeine_key_index, RGB_OFF);
        }
    }
#endif // RGB_MATRIX_ENABLE

#ifdef LED_CAFFEINE_PIN
    if (!is_caffeine_on && is_blink_led_on) {
        is_blink_led_on = false;
    }
#endif // LED_CAFFEINE_PIN
}

/**
 * rgb_matrix_indicators Call-in ---------------------------------------------
 * !! : if you're using as a module, you will need to call this from within a `rgb_matrix_indicators_user` function
 *   #ifdef COMMUNITY_MODULE_CAFFEINE_ENABLE
 *   bool rgb_matrix_indicators_user(void) {}
 *   #endif
 */
#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_sr_caffeine(void) {
    // assume that `matrix_scan_sr_caffeine` is running to toggle `is_blink_rgb_on`
    if ((caffeine_key_index != UINT8_MAX) && is_blink_rgb_on) {
        rgb_matrix_set_color(caffeine_key_index, caffeine_color.r, caffeine_color.g, caffeine_color.b);
    }
    return true;
}
#endif

/**
 * led_update_user Call-in ---------------------------------------------------
 * !! : if you're using as a module, you will need to call this from within a `led_update_user` function
 *   #ifdef LED_CAFFEINE_PIN
 *   bool led_update_user(led_t led_state) {}
 *   #endif
 */
#ifdef LED_CAFFEINE_PIN
bool led_update_sr_caffeine(led_t led_state) {
    if (is_caffeine_on) {
        gpio_write_pin(LED_CAFFEINE_PIN, is_blink_led_on ? LED_PIN_ON_STATE : !LED_PIN_ON_STATE);
    }
    return true;
}
#endif

/**
 * Switch ON Caffeine --------------------------------------------------------
 */
bool process_keycode_sr_caffeine_on(keyrecord_t *record) {
    if (record->event.pressed) {
#ifdef CONSOLE_ENABLE
        print("Caffeine ON\n");
#endif
        __caffeine_tap_jiggle(false); // dummy tap the default keycode so that the kb registers a key tap
#ifdef RGB_MATRIX_ENABLE
        // bind the key that was pressed
        caffeine_key_index = g_led_config.matrix_co[record->event.key.row][record->event.key.col];
#endif
        // start the timer
        timer_caffeine_buffer = sync_timer_read32();
        is_caffeine_on        = true;
        // is_blink_rgb_on & is_blink_led_on remain false
    }
    return false;
}

/**
 * Switch OFF Caffeine -------------------------------------------------------
 */
bool process_keycode_sr_caffeine_off(keyrecord_t *record) {
    if (record->event.pressed) {
#ifdef CONSOLE_ENABLE
        print("Caffeine OFF\n");
#endif

        is_caffeine_on = false;
#ifdef RGB_MATRIX_ENABLE
        // caffeine_key_index = UINT8_MAX;
        is_blink_rgb_on = false;
#endif
#ifdef LED_CAFFEINE_PIN
        is_blink_led_on = false;
#endif
    }
    return false;
}

/**
 * TOGGLE Caffeine on/off ----------------------------------------------------
 */
bool process_keycode_sr_caffeine_toggle(keyrecord_t *record) {
    if (record->event.pressed) {
        if (is_caffeine_on) {
            return process_keycode_sr_caffeine_off(record);
        } else {
            return process_keycode_sr_caffeine_on(record);
        }
    }
    return false;
}

/**
 * process_record Call-in ----------------------------------------------------
 * Note : if you're using as a module, this will get called by the module compile
 */
bool process_record_sr_caffeine(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case COMMUNITY_MODULE_CAFFEINE_ON: {
#ifdef CONSOLE_ENABLE
            print("*** COMMUNITY_MODULE_CAFFEINE_ON\n");
#endif
            return process_keycode_sr_caffeine_on(record);
            break;
        }

        case COMMUNITY_MODULE_CAFFEINE_OFF: {
#ifdef CONSOLE_ENABLE
            print("*** COMMUNITY_MODULE_CAFFEINE_OFF\n");
#endif
            return process_keycode_sr_caffeine_off(record);
            break;
        }

        case COMMUNITY_MODULE_CAFFEINE_TOGGLE: {
#ifdef CONSOLE_ENABLE
            print("*** COMMUNITY_MODULE_CAFFEINE_TOGGLE\n");
#endif
            return process_keycode_sr_caffeine_toggle(record);
            break;
        }
    }
    return true;
}
