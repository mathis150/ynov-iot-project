import paho.mqtt.client as mqtt
import requests
import json
import time

MQTT_BROKER = "localhost"  # Remplacez par l'adresse de votre broker
MQTT_PORT = 1883
MQTT_USER = "admin"
MQTT_PASSWORD = "admin"
MQTT_TOPIC = "iot/sensors"  # Remplacez par votre topic

API_URL = "http://iot-ynov.mathis-lenoir.net/publish_data.php"
data_buffer = [
    {
        "device": "raspberry_ynov",
        "monitoring": {},
        "data": {}
    }
]

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connecté au broker MQTT")
        client.subscribe(MQTT_TOPIC)
    else:
        print(f"Échec de la connexion, code: {rc}")

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode("utf-8"))
        print(f"Message reçu: {payload}")
        data_buffer.append(payload)
    except json.JSONDecodeError:
        print("Erreur de décodage JSON")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)

#import threading
#threading.Thread(target=send_data, daemon=True).start()

client.loop_forever()