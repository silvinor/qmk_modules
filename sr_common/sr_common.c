// Copyright 2023 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H
#include "sr_common.h"

bool kc_register_code_2(keyrecord_t *record, uint16_t data1, uint16_t data2) {
    if (record->event.pressed) {
        register_code(data1);
        register_code(data2);
    } else {
        unregister_code(data2);
        unregister_code(data1);
    }
    return false;  // Skip all further processing of this key
}

bool kc_register_code_3(keyrecord_t *record, uint16_t data1, uint16_t data2, uint16_t data3) {
    if (record->event.pressed) {
        register_code(data1);
        register_code(data2);
        register_code(data3);
    } else {
        unregister_code(data3);
        unregister_code(data2);
        unregister_code(data1);
    }
    return false;  // Skip all further processing of this key
}

#ifdef RGB_MATRIX_ENABLE // only need this for RGB MATRIX rn, but may change
bool process_record_sr_common(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Set all keys to yellow on EEPROM reset command
        case QK_CLEAR_EEPROM:
            if (record->event.pressed) {
                if (!rgb_matrix_is_enabled()) {
                    rgb_matrix_enable();
                }
                rgb_matrix_set_color_all(RGB_YELLOW);
                rgb_matrix_update_pwm_buffers();
                wait_ms(500); // 1/2 sec
            }
            return true; // let QMK do the rest
            break;

        // Set all keys to red on EEPROM reset command
        case QK_BOOT:
            if (record->event.pressed) {
                if (!rgb_matrix_is_enabled()) {
                    rgb_matrix_enable();
                }
                rgb_matrix_set_color_all(RGB_RED);
                rgb_matrix_update_pwm_buffers();
                wait_ms(500); // 1/2 sec
            }
            return true; // let QMK do the rest
            break;
    }
    return true;
}
#endif // RGB_MATRIX_ENABLE
