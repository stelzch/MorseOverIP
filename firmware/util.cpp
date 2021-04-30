#include "util.h"

void beep(int freq, int durationMicros, int pin) {
 pinMode(pin, OUTPUT);
 int T_half_micros = 1000 * 1000 / freq / 2;
 bool state = HIGH;
 for (int i = 0; i < durationMicros / T_half_micros; i++) {
   delayMicroseconds(T_half_micros);
   digitalWrite(pin, state);
   state = !state;
 }
}

int from_hex(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return -1;
}

const char *seek_until(const char *ptr, char character) {
    while (*ptr != 0x00) {
        if (*ptr == character)
            return ptr;

        ptr++;
    }

    return nullptr;
}

const char *read_urlencoded(const char *src, char *dst, int maxLength) {
    int i = 0; 
    while (i < maxLength) {
        if (*src == '&' || *src == 0x00)
            return src;

        if (*src == '%') {
            // Must read at least two characters into the future
            if (i + 2 >= maxLength)
                return nullptr;

            int a = from_hex(*(src+1));
            int b = from_hex(*(src+2));

            // If they are not valid numbers, return error code
            if (a == -1 || b == -1)
                return nullptr;

            char value = a * 16 + b;
            *dst = value;

            // We consumed two additional characters
            src += 2;
            i += 2;
        } else {
            *dst = *src;
        }

        dst++;
        src++;
        i++;
    }
    *dst = 0x00;

    return src;
}

int parse_int(const char *string) {
    int result = 0;

    while (string != nullptr && *string != 0x00 && *string >= '0' && *string <= '9') {
        int digit = *string - '0';
        result = result * 10 + digit;

        string++;
    }

    return result;

}
