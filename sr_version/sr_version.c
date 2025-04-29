// Copyright 2025 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H
#include "sr_version.h"
#include "version.h"

// these sizes must be larger than the origin strings for `strncpy` to work
#define VERSION_VERSION_BUFFER_SIZE 30
#define VERSION_OUTPUT_BUFFER_SIZE 80

// Initialise as an empty string
static char version_string[VERSION_OUTPUT_BUFFER_SIZE] = {'\0'};

bool process_record_sr_version(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case COMMUNITY_MODULE_SEND_VERSION:
            if (record->event.pressed) {
                if (version_string[0] == '\0') {
                    char qmkver[VERSION_VERSION_BUFFER_SIZE];
                    snprintf(qmkver, sizeof(qmkver), "%s", QMK_VERSION);
                    char *dash_pos = strchr(qmkver, '-');
                    if (dash_pos) {
                        *dash_pos = '\0'; // Crop at first '-'
                    }

                    char build_date[11];
                    snprintf(build_date, sizeof(build_date), "%.10s", QMK_BUILDDATE);

                    snprintf(version_string, sizeof(version_string), "%s:%s @ %s [%s]", QMK_KEYBOARD, QMK_KEYMAP, qmkver, build_date);
                }
                SEND_STRING(version_string);
            }
            return false;
    }
    return true;
}
