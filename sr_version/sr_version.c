// Copyright 2025 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H
#include "sr_version.h"
#include "version.h"
#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif

// these sizes must be larger than the origin strings for `strncpy` to work
#define VERSION_VERSION_BUFFER_SIZE 24
#ifdef VIA_PROTOCOL_VERSION
#    define VERSION_OUTPUT_BUFFER_SIZE 96
#else
#    define VERSION_OUTPUT_BUFFER_SIZE 64
#endif

// Initialise as an empty string
static char version_string[VERSION_OUTPUT_BUFFER_SIZE] = {'\0'};

bool process_keycode_sr_version(keyrecord_t *record) {
    if (record->event.pressed) {
        if (version_string[0] == '\0') {
            char qmk_ver[VERSION_VERSION_BUFFER_SIZE];
#ifdef QMK_VERSION_MAJOR
            snprintf(qmk_ver, sizeof(qmk_ver), "%d.%d.%d", QMK_VERSION_MAJOR, QMK_VERSION_MINOR, QMK_VERSION_PATCH);
#else
            snprintf(qmk_ver, sizeof(qmk_ver), "%s", QMK_VERSION);
            char *dash_pos = strchr(qmk_ver, '-');
            if (dash_pos) {
                *dash_pos = '\0'; // Crop at first '-'
            }
#endif

            char build_date[11];
            snprintf(build_date, sizeof(build_date), "%.10s", QMK_BUILDDATE);

#ifdef VIA_PROTOCOL_VERSION
#    ifdef VIAL_PROTOCOL_VERSION
            snprintf(version_string, sizeof(version_string), "%s:%s @ %s [%s] & Vial %lu", QMK_KEYBOARD, QMK_KEYMAP, qmk_ver, build_date, VIAL_PROTOCOL_VERSION);
#    else
            snprintf(version_string, sizeof(version_string), "%s:%s @ %s [%s] & VIA %d", QMK_KEYBOARD, QMK_KEYMAP, qmk_ver, build_date, VIA_PROTOCOL_VERSION);
#    endif
#else
            snprintf(version_string, sizeof(version_string), "%s:%s @ %s [%s]", QMK_KEYBOARD, QMK_KEYMAP, qmk_ver, build_date);
#endif
        }
#ifdef CONSOLE_ENABLE
        wait_ms(50);
        uprintf("%s\n", version_string);
        wait_ms(50);
#endif
        // Don't use `SEND_STRING` -> has a bug for __AVR__ builds
        send_string_with_delay(version_string, 0);
    }
    return false;
}

bool process_record_sr_version(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case COMMUNITY_MODULE_SEND_VERSION: {
#    ifdef CONSOLE_ENABLE
            print("*** COMMUNITY_MODULE_SEND_VERSION\n");
#    endif
            return process_keycode_sr_version(record);
        }

    }
    return true;
}
