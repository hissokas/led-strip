#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
// #include <pb_decode.h>

#include "settings.h"

// Constants.
// ----------------------------------------------------------------------------

const int PORT = 5555;

const int PIN_RED = D1;
const int PIN_GREEN = D2;
const int PIN_BLUE = D3;

// Setting up.
// ----------------------------------------------------------------------------

WiFiUDP udp;
WiFiEventHandler wifiConnectedHandler, wifiGotIpHandler, wifiDisconnected;

void setupPins() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
}

void setupSerial() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("Serial is ready.");
}

void setupWiFi() {
    wifiConnectedHandler = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected& event) {
        Serial.println("Connected to Wi-Fi.");
    });
    wifiGotIpHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
        Serial.print("Got IP: ");
        Serial.println(WiFi.localIP());
    });
    wifiDisconnected = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
        Serial.println("Disconnected from Wi-Fi.");
    });
    WiFi.begin(SSID, KEY);
}

void setupMDNS() {
    Serial.print("Setting up mDNS. Hostname: ");
    Serial.println(WiFi.hostname());
    if (MDNS.begin(WiFi.hostname().c_str())) {
        Serial.println("Registering mDNS service.");
        MDNS.addService("led", "udp", PORT);
    } else {
        Serial.println("mDNS responder error.");
    }
}

void setupUDP() {
    if (udp.begin(PORT)) {
        Serial.println("UDP started.");
    } else {
        Serial.println("UDP start error.");
    }
}

void setup() {
    setupPins();
    setupSerial();
    setupWiFi();
    setupMDNS();
    setupUDP();
}

// Running.
// ----------------------------------------------------------------------------

void handlePacket() {
    const int packetSize = udp.parsePacket();
    if (packetSize == 0) {
        return;
    }

    unsigned char buffer[100];
    const int length = udp.read(buffer, sizeof(buffer));
    if (length == 0) {
        return;
    }

    // TODO.
}

void loop() {
    handlePacket();
}
