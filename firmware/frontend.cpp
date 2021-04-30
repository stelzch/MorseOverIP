#include "constants.h"
#include "frontend.h"

ESP8266WebServer server(80);
const char *AP_SSID = "MorseOverIP-black";
const char *AP_PSK = "MorseOverIP";

void webserver_handle_root(void) {
  File f = LittleFS.open("/index.html", "r");

  if (!f) {
    server.send(500, "text/plain", "Internal Server Error - Could not find web interface on file system");
  }
  
  server.send(200, "text/html", f.readString());
  f.close();
}

void webserver_handle_beep(void) {
  String body = server.arg("plain");
  int freq = body.toInt();

  if (freq >= FREQ_MIN && freq <= FREQ_MAX) {
    beep(freq, 500 * 1000, SPEAKER_PIN); // 500ms
    server.send(200, "text/plain", "BEEPED");
  } else {
    server.send(400, "text/plain", "Frequency value illegal");
  }
}

int parse_request_body(const char *body) {
    
    // Read WiFi SSID
    const char *ptr = seek_until(body, '=') + 1;
    if (ptr == nullptr) return -1;
    ptr = read_urlencoded(ptr, settings.wifi_ssid, 126);
    if (*ptr == 0x00) return -1;

    // Read WiFi PSK
    ptr = seek_until(ptr, '=') + 1;
    if (ptr == nullptr) return -1;
    ptr = read_urlencoded(ptr, settings.wifi_psk, 126);
    if (*ptr == 0x00) return -1;

    // Read Freq
    ptr = seek_until(ptr, '=') + 1;
    if (ptr == nullptr) return -1;
    settings.freq = parse_int(ptr);

    return 0;
}


void webserver_handle_submit(void) {
    const char *reqBody = server.arg("plain").c_str();
    int retval = parse_request_body(reqBody);
    Serial.printf("Settings = { ssid = '%s', psk = '%s', freq = %i }\n", settings.wifi_ssid, settings.wifi_psk, settings.freq);

    if (retval == 0) {
        storage_save_settings();
        delay(1000);
        server.send(200, "text/plain", "OK, restarting");
        ESP.restart();
    } else {
        server.send(400, "text/plain", "Your input was incorrect. Please correct it by going back");
    }

}

void webserver_setup(void) {
  LittleFS.begin();
  WiFi.softAP(AP_SSID, AP_PSK);
  server.on("/", webserver_handle_root);
  server.on("/beep", webserver_handle_beep);
  server.on("/submit", webserver_handle_submit);
  server.begin();
}

void webserver_loop(void) {
  server.handleClient();
}
