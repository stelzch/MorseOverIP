#pragma once
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "storage.h"
#include "util.h"


void webserver_setup(void);
void webserver_loop(void);
