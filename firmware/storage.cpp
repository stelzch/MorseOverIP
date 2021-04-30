#include "storage.h"

void storage_init(void) {
    EEPROM.begin(sizeof(Settings));
}

void storage_read_settings(void) {
    byte *resultPtr = (byte *) &settings;

    for (int i = 0; i < sizeof(Settings); i++) {
        resultPtr[i] = EEPROM.read(i);
    }

    // Null the last byte of the two strings to ensure they are ended
    // correctly even if the EEPROM contains garbage bytes
    settings.wifi_ssid[127] = 0x00;
    settings.wifi_psk[127] = 0x00;

}

void storage_save_settings(void) {
    byte *ptr = (byte *) &settings;

    for (int i = 0; i < sizeof(Settings); i++) {
        EEPROM.write(i, ptr[i]);
    }

    EEPROM.commit();
}
