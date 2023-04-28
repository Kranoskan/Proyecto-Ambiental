#!/usr/bin/env pybricks-micropython
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import (Motor, TouchSensor, ColorSensor, InfraredSensor, UltrasonicSensor, GyroSensor)
from pybricks.parameters import Port, Stop, Direction, Button, Color
from pybricks.tools import wait, StopWatch, DataLog
from pybricks.robotics import DriveBase
from pybricks.media.ev3dev import SoundFile, ImageFile
import pybricks

# This program requires LEGO EV3 MicroPython v2.0 or higher.
# Click "Open user guide" on the EV3 extension tab for more information.


# Create your objects here.
ev3 = EV3Brick()

from umqtt.simple import MQTTClient

# Configurar el cliente MQTT
mqtt_server = "platinumvulture693.cloud.shiftr.io"
client_id = "identificador-del-cliente"
username = "platinumvulture693"
password = "VQrRjf9gXs2Exnmi"

# Crear un objeto MQTTClient y conectarse al broker MQTT
client = MQTTClient(client_id, mqtt_server, user=username, password=password)
client.connect()

topic = "nombre-del-tema"
message = "mensaje-a-publicar"
client.publish(topic, message)