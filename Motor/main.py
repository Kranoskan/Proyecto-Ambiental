#!/usr/bin/env pybricks-micropython
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import (Motor, TouchSensor, ColorSensor,
                                 InfraredSensor, UltrasonicSensor, GyroSensor)
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

# Configuración del cliente MQTT
SERVER = 'your-mqtt-server.com'
PORT = 1883
USER = 'your-username'
PASSWORD = 'your-password'
CLIENT_ID = 'your-client-id'

# Función de callback para procesar los mensajes recibidos
def callback(topic, msg):
    print(topic.decode(), msg.decode())

# Conexión al servidor MQTT
client = MQTTClient('client', '192.168.0.123', user='username', password='mypassword')
client.set_callback(callback)
client.connect()

# Suscripción a un topic
client.subscribe(b'test/topic')

# Publicación de un mensaje
client.publish(b'test/topic', b'Hola, mundo!')

# Esperar mensajes
while True:
    client.wait_msg()