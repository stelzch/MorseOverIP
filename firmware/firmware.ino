/** Adapted from https://github.com/programmer131/ESP8266-gBridge-TLS/blob/master/esp8266_gbridgeTLS/esp8266_gbridgeTLS.ino
 *  */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "constants.h"
#include "storage.h"
#include "frontend.h"
#include "util.h"
#include "secrets.h"

bool speakerEnabled = false;
int freq = 1200;
int T_half_micros;
int time_spent = 0;
int t_start;

bool adhoc_mode = false;
Settings settings;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
  while(!client.connected()) {
    Serial.println("Reconnection attempt...");
    
    if (client.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Connection established");

      client.subscribe("morse/black/onoff");
    } else {
      Serial.print("Connection failed, return code = ");
      Serial.print(client.state());
      Serial.println(", try again in 5 seconds");
      delay(5000);
    }
  }
    
}

void callback(char *topic, byte* payload, unsigned int length) {
  char firstChar = (char) payload[0];
  if (firstChar == '1') {
    speakerEnabled = true;
  } else if (firstChar == '0') {
    speakerEnabled = false;
  }
}

void setup() {
  storage_init();
  delay(2000);
  Serial.begin(115200);

  storage_read_settings();
  Serial.printf("Read settings, connecting to %s with key %s. Default frequency: %i\n",
          settings.wifi_ssid, settings.wifi_psk, settings.freq);

  if (settings.freq >= FREQ_MIN && settings.freq <= FREQ_MAX) {
      freq = settings.freq;
      T_half_micros = 1000 * 1000 / freq / 2;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(settings.wifi_ssid, settings.wifi_psk);

  while (WiFi.status() != WL_CONNECTED && millis() < 15 * 1000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  adhoc_mode = WiFi.status() != WL_CONNECTED;
  if (adhoc_mode) {
    Serial.println("Connection to WiFi failed, spinning up AdHoc net...");
    webserver_setup();
  } else {
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
    reconnect();
  }

  
  pinMode(SPEAKER_PIN, OUTPUT);
  
}

void loop() {
  if (adhoc_mode) {
    webserver_loop();
    return;
  }
  
  if (speakerEnabled) {
      time_spent = micros() - t_start;
      const int diff = T_half_micros - time_spent;
      if (diff > 0) delayMicroseconds(diff);
      digitalWrite(SPEAKER_PIN, HIGH);
      delayMicroseconds(T_half_micros);
      digitalWrite(SPEAKER_PIN, LOW);
  }

  t_start = micros();
  client.loop();
}
