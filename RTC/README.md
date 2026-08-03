Programming Clock
Script de configuration de l'horloge RTC (DS3231)
Ce programme Arduino permet d'initialiser et de vérifier le fonctionnement de l'horloge temps réel (DS3231 RTC) intégrée au Mayfly Data Logger.

Fonctionnalités
Initialise la communication avec le module DS3231 via le bus I²C.
Configure la date et l'heure du RTC à partir d'une valeur définie dans le programme.
Lit la date et l'heure stockées dans le RTC.
Affiche la date, l'heure et le jour de la semaine dans le moniteur série toutes les secondes.
Utilisation
Insérer la pile de sauvegarde du RTC.
Modifier la date et l'heure dans la variable sync_dt.
Téléverser le programme sur le Mayfly.
Vérifier dans le moniteur série que l'heure est correctement réglée et évolue normalement.
