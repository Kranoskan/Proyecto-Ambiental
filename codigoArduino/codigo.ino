
#include <SPI.h>
#include <WiFi101.h>
#include <MQTT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

const int trigPin = 3;
const int echoPin = 2;

long duration;
int distance;

char ssid[] = "*";
char pass[] = "*";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);


void setup(void)
{
  Serial.begin(9600);
  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
    // start wifi and mqtt
  WiFi.begin(ssid, pass);
  client.begin("platinumvulture693.cloud.shiftr.io", net);
  client.onMessage(messageReceived);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //connect();
  delay(1000);
}


void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "platinumvulture693", "VQrRjf9gXs2Exnmi")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("Distancia");
  client.subscribe("Caida");
}



void messageReceived(String &topic, String &payload) {
}

void loop() {

  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  client.loop();
  delay(10);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

  if (!client.connected()) {
    connect();
  }
  sensors_event_t  accelerometerData;
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);


  printEvent(&accelerometerData);



  int8_t boardTemp = bno.getTemp();
  Serial.println();
  Serial.println(boardTemp);


  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  String caida=printEvent(&accelerometerData);
  
  client.publish("Distancia", String(distance));
  client.publish("Caida", String(caida));
}




String printEvent(sensors_event_t* event) {
  double x = 0, y = 0 , z = 0; 
 
  if (event->type == SENSOR_TYPE_ACCELEROMETER){
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  if((x<-11 || x>11)|| (y<-11 || y>11) || (z<-11 || z>11)){
    return "Cayendo";

  }
  return "NoCae";
}