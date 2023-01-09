# Projet Objets Connectés

Adaptateur CAN-bluetooth pour écoconduite

## Manuel d'installation

Afin de pouvoir profité des fonctionnalités proposées dans ce projet, il vous faut installer dans Arduino la [bibliotèque](https://github.com/Longan-Labs/Serial_CAN_Arduino) créé par les fabriquant de l'adaptateur CAN.

Il vous suffit ensuite de clone de repertoire.

## Manuel d'utilisation

Pour accéder aux valeurs du véhicule, voici le schéma électrique à respecter :
Ce schéma comprend :
 - Une carte Arduino UNO
 - Un [Serial CAN Bus module](https://docs.longan-labs.cc/1030001/)
 - Une prise OBD


Après avoir effectué les branchements, il vous faut lancer la carte Arduino après avoir téléversé et de démarrer votre voiture. (Dans l'état actuel, il faut bien attendre le retour `set filt ok` de la carte avant de démarrer la voiture.)

Les valeurs sont ensuite à lire sur le Serial terminal de la carte, les PID étant indenté dans l'ordre du tableau `pids[]` situé dans `can_config.h`.

Si vous voulez obtenir des valeurs via des PIDs spécifique, nous vous invitons à modifié le programme en crééant un nouveau tableau `pids[]` et modifié la fonction `convert_from_bytes_to_unite` ainsi que le programme principal `loop()`.
