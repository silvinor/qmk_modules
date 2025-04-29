// Copyright 2023 Silvino R. (@silvinor)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

bool kc_register_code_2(keyrecord_t *record, uint16_t data1, uint16_t data2);
bool kc_register_code_3(keyrecord_t *record, uint16_t data1, uint16_t data2, uint16_t data3);

// Just in case you need to call it from non-modules
bool process_record_sr_common(uint16_t keycode, keyrecord_t *record);
