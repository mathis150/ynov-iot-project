# Ynov IOT: Serre autonome

## Introduction

> Ce projet sert à créer une petite serre connectée capable d'être totalement autonome de son propriétaire.

## Installation de l'arduino

- Installer "Arduino IDE" si ce n'est pas encore fais.
- Brancher votre arduino après avoir respecter le schéma des branchements fournis
- Lancer l'IDE avec le projet, et sélectionner le port auquel est connecté votre arduino
- Injecter le code présent dans "arduino" :
  - Injecter le fichier "main.ino" dans l'Arduino Uno
  - Injecter le fichier "wi-fi.ino" dans l'ESP-8266

> Plus qu'à ouvrir le terminal et le voir fonctionner !

## Installation du raspberry

- Si ce n'est pas déjà fais installer linux et OpenSSH sur le raspberry
- Installer "MQTT" avec les commandes suivantes (Il ce lancera tout seul après intallation):
```bash
$ sudo apt update
$ sudo apt install -y mosquitto
```

- Après avoir installé MQTT et utilisé un Broker pour vérifier si tout est bien fonctionnel, installons Python :
```bash
$ sudo apt install -y python3
```

- Installez les librairies qui vont avec :
```bash
$ sudo apt install python3-pip libcurl4-openssl-dev libssl-dev libffi-dev python3-dev -y

$ sudo pip install paho-mqtt pycurl
```

- Après ça, utiliser la commande suivante dans le répertoire du script python du projet :
```bash
$ python3 main.py
```

## Installation du serveur web :

- Le serveur web utilise la librairie LAMP, pour l'installer sur windows, utilisez WAMP ou XAMPP, sous linux, utilisez la commande suivante :
```bash
$ sudo apt install apache2 mysql-server php libapache2-mod-php php-mysql -y
```

- Si tu souhaite un moyen de visualiser facilement ta base de donnée :
```bash
$ sudo apt install phpmyadmin -y
```

- Si PHPMyAdmin n'est pas accessible après installation à `<ton url>/phpmyadmin` alors il existe les commandes suivantes :
```bash
$ sudo ln -s /etc/phpmyadmin/apache.conf /etc/apache2/conf-available/phpmyadmin.conf
$ sudo a2enconf phpmyadmin
$ sudo systemctl reload apache2
```

## Après tout ça, comment accéder au panneau ?
> `<Ton url>/index.php`