#pragma once
#include <Arduino.h>
#include <EEPROM.h>

typedef struct Settings_ {
    char wifi_ssid[128];
    char wifi_psk[128];
    int freq;
} Settings;

void storage_init(void);

void storage_read_settings(void);
void storage_save_settings(void);

extern Settings settings;
