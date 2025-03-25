import paho.mqtt.client as mqtt
import requests
import json
import time
import re

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPICS = ["iot/sensors"]

API_URL = "http://iot-ynov.mathis-lenoir.net/publish_data.php"

data_buffer = []

MQTT_USER = "admin"
MQTT_PASSWORD = "admin"

def clean_json(payload):
    try:
        cleaned_payload = re.sub(r'\\[\\{', '{', payload)
        cleaned_payload = re.sub(r'\\}\\]', '}', cleaned_payload)
        return json.loads(cleaned_payload)
    except json.JSONDecodeError:
        print("Erreur de décodage JSON après nettoyage")
        return None

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connecté au broker MQTT")
        for topic in MQTT_TOPICS:
            client.subscribe(topic)
            print(f"Abonné au topic: {topic}")
    else:
        print(f"Échec de la connexion, code: {rc}")

def on_message(client, userdata, msg):
    try:
        raw_payload = msg.payload.decode("utf-8")
        payload = clean_json(raw_payload)
        if payload:
            print(f"Message reçu sur {msg.topic}: {payload}")
            data_buffer.append(payload)
    except Exception as e:
        print(f"Erreur lors du traitement du message MQTT: {e}")

def send_data():
    while True:
        if data_buffer:
            data_to_send = data_buffer.copy()
            try:
                response = requests.post(API_URL, json=data_to_send)
                print(f"Données envoyées: {data_to_send}")
                print(f"Réponse API: {response.status_code}, {response.text}")
                if response.status_code == 200:
                    data_buffer.clear()
            except requests.RequestException as e:
                print(f"Erreur lors de l'envoi des données: {e}")
        time.sleep(60)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
client.connect(MQTT_BROKER, MQTT_PORT, 60)

import threadings
threading.Thread(target=send_data, daemon=True).start()

client.loop_forever()