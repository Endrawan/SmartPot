#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define SOIL_SENSOR A0
#define WATERING_ACTUATOR D0

#define HUMIDITY_BOUNDS 30
#define WATERING_TIME 2000

#define FIREBASE_HOST "<your-firebase-host-here>"
#define FIREBASE_AUTH "<your-firebase-auth-here>"
#define WIFI_SSID "<wifi-name>"
#define WIFI_PASSWORD "<wifi-password>"

int humidity;
bool wateringStatus = false;

void updateHumidity(int);
bool getWateringStatus();
bool setWateringStatus(bool);
void wateringThePlants(int);

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(SOIL_SENSOR, INPUT);
  pinMode(WATERING_ACTUATOR, OUTPUT);
}

void loop() {
  // Get sensor value
  humidity = map(analogRead(SOIL_SENSOR), 1023, 0, 0, 100);
  wateringStatus = getWateringStatus();
  updateHumidity(humidity);
  wateringThePlants(humidity);
  delay(1000);
}

void updateHumidity(int humidity) {
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Firebase.setInt("kelembapan", humidity);
  if (Firebase.failed()) {
    Serial.print("Update humidity to Firebase failed:");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Update humidity to Firebase successful");
  }
}

bool getWateringStatus() {
  bool status = Firebase.getBool("wateringStatus");
  Serial.print("WateringStatus: ");
  Serial.println(status); 
  return status;
}

bool setWateringStatus(bool status) {
  Firebase.setBool("wateringStatus", status);
  if (Firebase.failed()) {
    Serial.print("Update wateringStatus to Firebase failed:");
    Serial.println(Firebase.error());
    setWateringStatus(status);
  } else {
    Serial.println("Update wateringStatus to Firebase successful");
  }
}

void wateringThePlants(int humidity) {
 if(humidity <= HUMIDITY_BOUNDS || wateringStatus) {
    Serial.println("Watering...");
    digitalWrite(WATERING_ACTUATOR, HIGH);
    delay(WATERING_TIME);
    digitalWrite(WATERING_ACTUATOR, LOW);
    setWateringStatus(false);
  }
}
