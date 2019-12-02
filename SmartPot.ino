#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define SOIL_SENSOR A0
#define WATERING_ACTUATOR D0

#define HUMIDITY_BOUNDS 30
#define WATERING_TIME 2000

#define FIREBASE_HOST "smartpot-42167.firebaseio.com"
#define FIREBASE_AUTH "o4kWDyvAIPkBDH0FjO1L2RiTz8XgxfDKDhzrK0AH"
#define WIFI_SSID "Redmi"
#define WIFI_PASSWORD "nadi30041999"

int humidity;
bool wateringStatus;

void updateHumidity(int);
bool getWateringStatus();
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
  updateHumidity(humidity);
  wateringThePlants(humidity);
  delay(1000);

  // update value
  //  Firebase.setFloat("number", 43.0);
  //  // handle error
  //  if (Firebase.failed()) {
  //      Serial.print("setting /number failed:");
  //      Serial.println(Firebase.error());
  //      return;
  //  }
  //  delay(1000);
  //
  //  // get value
  //  Serial.print("number: ");
  //  Serial.println(Firebase.getFloat("number"));
  //  delay(1000);
  //
  //  // remove value
  //  Firebase.remove("number");
  //  delay(1000);
  //
  //  // set string value
  //  Firebase.setString("message", "hello world");
  //  // handle error
  //  if (Firebase.failed()) {
  //      Serial.print("setting /message failed:");
  //      Serial.println(Firebase.error());
  //      return;
  //  }
  //  delay(1000);
  //
  //  // set bool value
  //  Firebase.setBool("truth", false);
  //  // handle error
  //  if (Firebase.failed()) {
  //      Serial.print("setting /truth failed:");
  //      Serial.println(Firebase.error());
  //      return;
  //  }
  //  delay(1000);
  //
  //  // append a new value to /logs
  //  String name = Firebase.pushInt("logs", n++);
  //  // handle error
  //  if (Firebase.failed()) {
  //      Serial.print("pushing /logs failed:");
  //      Serial.println(Firebase.error());
  //      return;
  //  }
  //  Serial.print("pushed: /logs/");
  //  Serial.println(name);
  //  delay(1000);
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

void getWateringStatus()

void wateringThePlants(int humidity) {
 if(humidity <= HUMIDITY_BOUNDS) {
    Serial.println("Watering...");
    digitalWrite(WATERING_ACTUATOR, HIGH);
    delay(WATERING_TIME);
    digitalWrite(WATERING_ACTUATOR, LOW);
  }
}
