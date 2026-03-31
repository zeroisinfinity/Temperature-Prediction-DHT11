#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <Firebase_ESP_Client.h>
#include <time.h>

// ------------ WIFI ------------
#define WIFI_SSID     ""
#define WIFI_PASSWORD ""

// ------------ FIREBASE ------------
#define DATABASE_URL ""
#define API_KEY      ""

// ------------ DEVICE META ------------
#define DEVICE_ID "esp32_01"
#define FIRMWARE_VERSION "v1.0.0"

// ------------ DHT ------------
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastRead = 0;
const unsigned long interval = 5000;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  configTime(0, 0, "pool.ntp.org");

  while (time(nullptr) < 100000) {
    delay(500);
  }

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.signUp(&config, &auth, "", "");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

  if (!Firebase.ready()) return;

  if (millis() - lastRead >= interval) {
    lastRead = millis();

    float tempC = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(tempC) || isnan(humidity)) {
      Serial.println("Sensor read failed");
      return;
    }

    float tempF = tempC * 9.0 / 5.0 + 32.0;

    time_t now = time(nullptr);
    struct tm *timeinfo = gmtime(&now);

    char isoTime[30];
    strftime(isoTime, sizeof(isoTime),
             "%Y-%m-%dT%H:%M:%SZ", timeinfo);

    FirebaseJson json;

    json.set("device_id", DEVICE_ID);
    json.set("firmware", FIRMWARE_VERSION);

    json.set("temperature_c", tempC);
    json.set("temperature_f", tempF);
    json.set("humidity", humidity);

    json.set("timestamp", (int)now);
    json.set("iso_time", isoTime);

    json.set("hour", timeinfo->tm_hour);
    json.set("day_of_week", timeinfo->tm_wday);

    json.set("wifi_rssi", WiFi.RSSI());
    json.set("uptime_sec", millis() / 1000);

    if (Firebase.RTDB.pushJSON(&fbdo,
        "/devices/" DEVICE_ID "/telemetry",
        &json)) {
      Serial.println("Telemetry logged");
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}
