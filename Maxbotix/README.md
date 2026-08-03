Mesure niveau d'eau capteur MaxBotix
Ce programme permet de mesurer le niveau d'eau ou de distance utilisant un capteur ultrasonique MaxBotix connecté à un datalogger MayFly.

À intervalles réguliers (10 s par défaut), le système :

Active l'alimentation du capteur MaxBotix.
Réalise plusieurs mesures de tension via l'ADS1115.
Calcule une tension moyenne afin de réduire le bruit de mesure.
Convertit la tension mesurée en distance (mm) selon la formule du constructeur.
Associe la mesure à un horodatage provenant du module RTC DS3231 (horloger interne MayFly).
Enregistre les données dans un fichier CSV sur la carte microSD.
Affiche les résultats dans le moniteur série.
Format du fichier CSV
DateTime,Tension_V,Distance_mm 2026-07-16 10:54:00,1.2456,1930 2026-07-16 10:54:10,1.2471,1932
