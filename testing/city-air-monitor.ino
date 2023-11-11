#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// CONFIGS
#define WIFI_SSID "hello123"
#define WIFI_PASSWORD "hello123"
#define API_KEY "AIzaSyAnVjkhiPUXtsexWzpWUVxANJGxaIilqTI"
#define DATABASE_URL "https://city-air-quality-monitor-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo; // Firebase Data Object
FirebaseAuth auth; // Firebase Auth Object
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

#include <DHT.h>
#include <DHT_U.h>
#define DHT11_PIN 4
float hum, temp;
DHT dht(DHT11_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi:");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Configs
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}


void loop() {
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Store data to Realtime Database
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/humidity", hum)){
      Serial.println("Humidity data sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/temperature", temp)){
      Serial.println("Humidity data sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}
