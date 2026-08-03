# Programming Clock

## Script de configuration de l'horloge RTC (DS3231)

Ce programme Arduino permet d'initialiser et de vérifier le fonctionnement de l'horloge temps réel (**DS3231 RTC**) intégrée au **Mayfly Data Logger**.

### Fonctionnalités
- Initialise la communication avec le module DS3231 via le bus I²C.
- Configure la date et l'heure du RTC à partir d'une valeur définie dans le programme.
- Lit la date et l'heure stockées dans le RTC.
- Affiche la date, l'heure et le jour de la semaine dans le moniteur série toutes les secondes.
### Utilisation
1. Insérer la pile de sauvegarde du RTC.
2. Modifier la date et l'heure dans la variable `sync_dt`.
3. Téléverser le programme sur le Mayfly.
4. Vérifier dans le moniteur série que l'heure est correctement réglée et évolue normalement.

## Mesure niveau d'eau capteur MaxBotix

Ce programme permet de mesurer le niveau d'eau ou de distance utilisant un capteur ultrasonique MaxBotix connecté à un datalogger MayFly.

À intervalles réguliers (10 s par défaut), le système :

1. Active l'alimentation du capteur MaxBotix.
2. Réalise plusieurs mesures de tension via l'ADS1115.
3. Calcule une tension moyenne afin de réduire le bruit de mesure.
4. Convertit la tension mesurée en distance (mm) selon la formule du constructeur.
5. Associe la mesure à un horodatage provenant du module RTC DS3231 (horloger interne MayFly).
6. Enregistre les données dans un fichier CSV sur la carte microSD.
7. Affiche les résultats dans le moniteur série.

### Format du fichier CSV

DateTime,Tension_V,Distance_mm
2026-07-16 10:54:00,1.2456,1930
2026-07-16 10:54:10,1.2471,1932
