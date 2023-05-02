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

char ssid[] = "Redmi Note 8T";
char pass[] = "aaas1234";
char link[] = "platinumvulture693.cloud.shiftr.io"; // dirección del shiftr.io, despues del https://
char name[] = "platinumvulture693"; // Nombre del shiftr.io
char token[] = "VQrRjf9gXs2Exnmi"; //la clave del token
char arduinoID[] = "arduino";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

float UMBRAL=2; //margen para detectar si está cayendo

float GRAVEDAD=9.8;

bool caidaB=false; // por si queremos que una vez se active sólo mandará mensaje de caida

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
  client.begin(link, net);
  client.onMessage(messageReceived);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //connect();
  delay(1000);
}

void reset(){
  caidaB=false;
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(arduinoID, name, token)) {
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
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  if (!client.connected()) {
    connect();
  }
  sensors_event_t  accelerometerData;
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  String caida=printEvent(&accelerometerData);
  Serial.print("\n Distancia: ");
  Serial.print(distance);
  //Serial.print("  Caida: ");
  //Serial.print(caida);
  delayMicroseconds(100000);
  if(!caidaB){
    if(distance>=25){
      client.publish("subir", "s");
      Serial.print("\n Subir");
    }
    if(distance<16){
      client.publish("bajar", "b");
      Serial.print("\n Bajar");
    }
    if(distance<18 && distance>=16){
      client.publish("mitad1", "m");
      Serial.print("\n Mitad1");
    }
    if(distance>=22 && distance<25){
      client.publish("mitad2", "n");
      Serial.print("\n Mitad2");
    }
    if(distance>=18 && distance<22){
      client.publish("mitad", "o");
      Serial.print("\n Mitad");
    }
  }else{
    client.publish("parar", "p");
  }
  //client.publish("Distancia", String(distance));
  //client.publish("Caida", String(caida));
}

String printEvent(sensors_event_t* event) {
  String salida="Cayendo";
  if(!caidaB){
    double x = 0, y = 0 , z = 0; 
    if (event->type == SENSOR_TYPE_ACCELEROMETER){
      x = event->acceleration.x;
      y = event->acceleration.y;
      z = event->acceleration.z;
    }
    float umbralCaida=GRAVEDAD+UMBRAL;
    if((x<-umbralCaida || x>umbralCaida)|| (y<-umbralCaida || y>umbralCaida) || (z<-umbralCaida || z>umbralCaida)){
      caidaB=true;
    }else{
      salida= "NoCae";
    }
  }
  return salida;
}