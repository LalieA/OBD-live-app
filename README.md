# Module OBD-II pour l'éco-conduite

Ce dépôt contient toute la documentation permettant de réaliser un prototype reliant un véhicule automobile et une application mobile permettant d’utiliser les données de la voiture en temps réel, dans le but de conseiller l’utilisateur dans sa conduite et l'inviter à adopter une conduite éco-responsable.

Vous trouverez dans ce README toutes les informations relatives à la construction du prototype, son installation, et son utilisation. Une documentation plus détaillée du projet dans son ensemble est également disponible sous forme de wiki.

## Construction du prototype
### Matériel et branchements
Nous avons réalisé ce prototype avec le matériel suivant :
- 1x [Arduino Uno](https://www.gotronic.fr/art-carte-arduino-uno-12420.htm)
- 1x [module Bluetooth HC05](https://www.gotronic.fr/art-module-bluetooth-hc05-26097.htm)
- 1x [kit CAN OBD-II](https://www.gotronic.fr/art-kit-obd-ii-can-bus-114991438-27341.htm) ([Documentation](https://docs.longan-labs.cc/1030001/))
- 1x [Breadboard](https://www.gotronic.fr/art-plaque-de-montage-rapide-6858.htm) et [câbles jumper](https://www.gotronic.fr/art-pack-de-cables-de-connexion-12411.htm)
- 1x résistance 1kΩ
- 1x résistance 2,2kΩ

Voici le schéma des branchements :

![Branchements](doc/schematics.png)

- Les deux résistances permettent de créer un pont diviseur de tension de 5V vers 3,3V afin de pouvoir fonctionner avec le module Bluetooth
- Le module Serial CAN Bus du kit CAN OBD-II possède un connecteur Grove et peut être directement connecté à l'Arduino via un shield Grove, ce qui simplifie encore plus le schéma de branchements.
- Les broches correspondant aux lignes CAN de l'OBD-II sont les broches 6 (CAN HIGH) et 14 (CAN LOW).

### Téléversement du code source

Depuis l'IDE Arduino, ouvrir le projet disponible dans le dossier `Arduino/main`.

Connecter l'Arduino par USB, compiler et téléverser le code source.

![](./doc/upload_firmware.png)


## Installation sur un véhicule automobile : trouver la prise de diagnostic OBD-II


## Utilisation du prototype, récupération des données de diagnostic
### Depuis le moniteur série Arduino

### Beta : Depuis l'application mobile


## Limitations connues
Ce projet n'étant par nature qu'un prototype, il y a des limitations connues à prendre en compte si l'on souhaite l'utiliser en conditions réelles. Ces limitations connues sont décrites dans le wiki.
