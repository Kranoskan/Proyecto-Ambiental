#!/usr/bin/env pybricks-micropython
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import (Motor, TouchSensor, ColorSensor, InfraredSensor, UltrasonicSensor, GyroSensor)
from pybricks.parameters import Port, Stop, Direction, Button, Color
from pybricks.tools import wait, StopWatch, DataLog
from pybricks.robotics import DriveBase
from pybricks.media.ev3dev import SoundFile, ImageFile
import pybricks

ev3 = EV3Brick()
motor = Motor(Port.B)

from umqtt.simple import MQTTClient

# Configurar el cliente MQTT
mqtt_server = "platinumvulture693.cloud.shiftr.io"
client_id = "EV3"
username = "platinumvulture693"
password = "VQrRjf9gXs2Exnmi"

# Crear un objeto MQTTClient y conectarse al broker MQTT
client = MQTTClient(client_id, mqtt_server, user=username, password=password)
client.connect()

topic = "conectado"
message = "conectado"
client.publish(topic, message)
gradosRotados=0;
def callback(topic, message):
    #print("Mensaje recibido en el tema {}: {}".format(topic, message))
    orden=str(message)
    ordenProcesada=orden[2:3]
    print(ordenProcesada)
    #ev3.speaker.beep()
    if ordenProcesada=="s":
        #motor.run_target(500, 700) #500 grados por segundo hasta alcanzar 90 grados
        #motor.run_target(1000,anguloMaximoArriba)
        motor.run(1000)
        #while a<2:
            #motor.run(1000)
            #wait(10)
            #print(motor.angle())
    if ordenProcesada=="b":
        motor.run(-1000)
        #anguloMaximoAbajo=0
        #motor.run_target(-1000,anguloMaximoAbajo)
    if ordenProcesada=="p":
        motor.stop()
    #print(motor.angle())

    
client.set_callback(callback)
client.subscribe("subir")
client.subscribe("bajar")
client.subscribe("parar")
while True:
    client.check_msg()