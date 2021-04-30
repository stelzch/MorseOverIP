#pragma once
#include <Arduino.h>

/* Beep for the specified duration in microseconds at frequency freq specified
 * int Hertz. Also needs the pin number.
 */
void beep(int freq, int durationMicros, int pin);

/* Convert single HEX character to INT
 */
int from_hex(char c);

/* Seeks zero-terminated string for a character.
 * Returns pointer to found character or NULL if it was not found.
 */
const char *seek_until(const char *ptr, char character);


/* Reads at most maxLength bytes from the ('&' or 0)-terminated string src into dst.
 * Characters that are percent-encoded are decoded.
 * Returns a pointer to the first byte of src that has not been read yet. 
 * If an error occurs, it returns null
 */
const char *read_urlencoded(const char *src, char *dst, int maxLength);

/* Parses an int from a character array.
 */
int parse_int(const char *string);
