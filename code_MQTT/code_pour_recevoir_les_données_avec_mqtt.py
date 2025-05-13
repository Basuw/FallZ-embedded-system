## Exemple  :   Abonnement à un courtier MQTT
##                 - connexion (authentification + clientID)
##                 - gestion des "callback" (connexion, abonnement)
##                 - demande d'abonnement
##---------------------------------------------------------------------

# Imports
import paho.mqtt.client as mqtt
import sys
from sys import exit
from datetime import datetime

# Identifiants
APPLICATION_ID = "detecteur-chute"
TENANT_ID = "fallz"  # Par défaut, sauf si tu as une instance privée
DEVICE_ID = "arduino-mkr-1300"
USERNAME = "detecteur-chute@fallz"  # Identifiant MQTT
PASSWORD = "NNSXS.NCTIDYFPFEARRQXK7T4OQ43SVRYNGNVW2DEDSHI.UQ2ZUWOFOTC6GT5SSZLJU5ZFHHKEDDTMS6ZPF4F44ZLQERJXFLPQ"

# Paramétres globaux du script
MQTT_HOST_NAME = "eu2.cloud.thethings.industries"       # Hôte du courtier
MQTT_HOST_PORT = 1883                                   # Port du courtier
MQTT_CLIENT_ID = "Subscribe_DashBoard_1"                # Client ID
TOPIC = f"v3/{APPLICATION_ID}@{TENANT_ID}/devices/{DEVICE_ID}/up"

# Colored prints
def prGreen(prt): print("\033[92m{}\033[00m" .format(prt))
def prPurple(prt): print("\033[95m{}\033[00m" .format(prt))
def prCyan(prt): print("\033[96m{}\033[00m" .format(prt))

# Le "callback" on_log : gestion des informations de log.
def on_log(client, userdata, level, buf):
    prCyan(" [on_log]        - " + buf)

# Le "callback" on_connect : gestion de la réponse du courtier (CONNACK)
def on_connect(client, userdata, flags, rc):
    prGreen(" [on_connect]    - Reponse du broker ( CONNACK )   : " + mqtt.connack_string(rc))

# Le "callback" on_disconnect : réponse du courtier à la deconnexion
def on_disconnect(client, userdata, rc):
    if rc != 0:
        prGreen(" [on_disconnect] - Déconnexion inattendue")
    else:
        prGreen(" [on_disconnect] - Déconnexion du broker")    

# Le "callback" on_subscribe : réponse du courtier à la demande d'abonnement
def on_subscribe(client, userdata, mid, granted_qos):
    prGreen(" [on_subscribe]  - Reponse du broker : mid = " + str(mid) + ", QOS = " + str(granted_qos))

# Le "callback" on_message : message recu du courtier
def on_message(client, userdata, msg):
    prGreen(f" {datetime.now().strftime("%d/%m/%Y %H:%M:%S")}  ==>  [on_message]    - Message reçu     : " + str(msg.payload))
    prGreen("                   du sujet (topic) : " + msg.topic)
    prGreen("                   avec une QoS     : " + str(msg.qos))

# Creation du client MQTT, des "callback" et de l'authentification
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1, MQTT_CLIENT_ID, protocol=mqtt.MQTTv311)
client.username_pw_set(USERNAME, PASSWORD)
client.on_log        = on_log
client.on_connect    = on_connect
client.on_disconnect = on_disconnect
client.on_subscribe  = on_subscribe
client.on_message    = on_message

print(sys.version)

# Connexion au courtier
try:
    client.connect(MQTT_HOST_NAME, MQTT_HOST_PORT)
except:
    prPurple(" [connect]       - Connexion au broker echouée")
    exit(0)

# Abonnement à un sujet 
(result, mid) = client.subscribe(TOPIC, qos=0)
prPurple(" [subscribe]     - Code erreur de la demande : " + mqtt.error_string(result))
prPurple(" [subscribe]     - ID pour la demande (mid)  : " + str(mid))

# Boucle : La méthode loop_forever() bloque le programme 
# et est utile lorsque le programme doit s'exécuter indéfiniment.
# La fonction loop_forever() gère également les reconnexions automatiques.
client.loop_forever()