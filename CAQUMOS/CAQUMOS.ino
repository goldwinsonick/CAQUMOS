// CAQUMOS (City Air Quality Monitoring System)

// Libraries
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Pinouts
#define MQ_sensor 35
#define PM25_sensor 32
#define PM25_LED 33
#define DHT11_sensor 23

// Configs
// --- WIFI ---
#define WIFI_SSID "eduroam kentang"
#define WIFI_PASSWORD "potatopotato"
#define API_KEY "AIzaSyAnVjkhiPUXtsexWzpWUVxANJGxaIilqTI"
#define DATABASE_URL "https://city-air-quality-monitor-default-rtdb.asia-southeast1.firebasedatabase.app/"
FirebaseData fbdo; // Firebase Data Object
FirebaseAuth auth; // Firebase Auth Object
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

// --- MQ-135 ---
#define RL 47  //The value of resistor RL is 47K
#define m1 -0.2218 // Slope for CO
#define m2 -0.585 // Slope for NH4
#define m3 -0.3537 // Slope for CO2
#define b1 0.6865 // Intercept for CO
#define b2 1.17 // Intercept for NH4
#define b3 0.717 // Intercept for CO2
#define Ro 30 //Enter found Ro value (BELUM FINAL, NILAI EKSAK NANTI MALAM)
// --- PM2.5 ---
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
// --- DHT11 ---
float hum, temp;
DHT dht(DHT11_sensor, DHT11);

void setup() {
  Serial.begin(9600);

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
  
  pinMode(PM25_LED, OUTPUT);
  dht.begin();
}

void loop() {
  // MQ-135
  float VRL, Rs, ratio;
  VRL = analogRead(MQ_sensor)*(5.0/1023.0); //Measure the voltage drop and convert to 0-5V
  Rs = ((5.0*RL)/VRL)-RL; //Use formula to get Rs value
  ratio = Rs/Ro;  // find ratio Rs/Ro
  float ppm1 = pow(10, ((log10(ratio)-b1)/m1)); //use formula to calculate ppm CO
  float ppm2 = pow(10, ((log10(ratio)-b2)/m2)); //use formula to calculate ppm NH4
  float ppm3 = pow(10, ((log10(ratio)-b3)/m3)); //use formula to calculate ppm CO2
  Serial.print("PPM CO: ");
  Serial.println(ppm1);
  Serial.print("PPM NH4: ");
  Serial.println(ppm2);
  Serial.print("PPM CO2: ");
  Serial.println(ppm3);
  // PM2.5
  
  digitalWrite(PM25_LED, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(PM25_sensor); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(PM25_LED, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 170 * calcVoltage - 0.1;
  Serial.print("Dust: ");
  Serial.println(dustDensity); // unit: ug/m3

  // DHT11
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(hum);

  // FIREBASE
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Store data to Realtime Database
    // temp, hum, dustDensity, ppm1, ppm2, ppm3

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/humidity", hum)){
      Serial.println("Humidity Sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/temperature", temp)){
      Serial.println("Temperature Sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/pm25", dustDensity)){
      Serial.println("Dust Sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/carbon-monoxide", ppm1)){
      Serial.println("CO sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/ammonium", ppm2)){
      Serial.println("NH4 sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/carbon-dioxide", ppm3)){
      Serial.println("CO2 sent...");
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
  delay(5000);
}
