import paho.mqtt.client as mqtt
import pycurl
import json
import time
import re
import io

MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPICS = ["iot/sensors"]

API_URL = "http://iot-ynov.mathis-lenoir.net/publish_data.php"

data_buffer = []

MQTT_USER = "admin"
MQTT_PASSWORD = "admin"

def clean_json(payload):
    try:
        # Nettoyage des caractères échappés
        payload = payload.replace(r'\\{', '{').replace(r'\\}', '}').replace(r'\\[', '[').replace(r'\\]', ']')
        
        # Supprimer les caractères indésirables non imprimables
        payload = re.sub(r'[\x00-\x1F\x7F-\x9F]', '', payload)
        
        # Essayer de charger le JSON proprement
        return json.loads(payload)
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
        print(f"Message brut reçu sur {msg.topic}: {raw_payload}")
        
        # Nettoyer et charger le JSON
        payload = clean_json(raw_payload)
        if payload:
            print(f"Message décodé: {payload}")
            data_buffer.clear()
            data_buffer.append(payload)
        else:
            print("Échec de la décodification du message JSON.")
    except Exception as e:
        print(f"Erreur lors du traitement du message MQTT: {e}")

def send_data():
    while True:
        if data_buffer:
            data_to_send = json.dumps(data_buffer.copy())
            buffer = io.BytesIO()

            try:
                c = pycurl.Curl()
                c.setopt(c.URL, API_URL)
                c.setopt(c.POST, 1)
                c.setopt(c.HTTPHEADER, ["Content-Type: application/json"])
                c.setopt(c.POSTFIELDS, data_to_send)
                c.setopt(c.WRITEDATA, buffer)
                c.setopt(pycurl.HTTP_VERSION, pycurl.CURL_HTTP_VERSION_1_1)
                
                c.perform()
                http_code = c.getinfo(pycurl.RESPONSE_CODE)
                c.close()

                response_body = buffer.getvalue().decode('utf-8')
                print(f"Données envoyées: {data_to_send}")
                print(f"Réponse API: {http_code}, {response_body}")

                if http_code == 200:
                    data_buffer.clear()
            except pycurl.error as e:
                print(f"Erreur lors de l'envoi des données: {e}")
        
        time.sleep(60)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Lancer l'envoi des données en arrière-plan
import threading
threading.Thread(target=send_data, daemon=True).start()

client.loop_forever()