#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <Sodaq_DS3231.h>
#include <Adafruit_ADS1X15.h>

// =====================================================================
// Carte SD
// =====================================================================

// Broche Chip Select de la carte microSD sur la Mayfly
const int8_t SdSsPin = 12;

// Objet permettant de communiquer avec la carte SD
SdFat SD;

// Nom du fichier CSV sur la carte SD
const char fileName[] = "DATA.CSV";

// Nom du logger et en-tete du fichier CSV
const char loggerName[] = "Mayfly MaxBotix Logger";
const char dataHeader[] = "DateTime,Tension_V,Distance_mm";


// =====================================================================
// Capteur MaxBotix via ADS1115
// =====================================================================

// Objet permettant de communiquer avec l'ADS1115
Adafruit_ADS1115 ads;

// Tension d'alimentation du capteur MaxBotix
// Si le capteur est alimente en 3.3 V, laisser 3.3
// Si le capteur est alimente en 5 V, remplacer par 5.0
const float VCC = 3.3;

// Broche qui active l'alimentation du capteur
// HIGH = capteur alimente
// LOW  = capteur eteint
const int8_t SensorPower = 22;

// Canal analogique de l'ADS1115 utilise
// AIN3 correspond au canal 3
const int8_t SonarChannel = 3;

// Nombre de mesures utilisees pour calculer la moyenne
const int NbMesures = 10;


// =====================================================================
// Intervalle de mesure
// =====================================================================

// Intervalle entre deux mesures en millisecondes
// 10000 ms = 10 secondes
const unsigned long sampleinterval_ms = 10000;


// =====================================================================
// Creation du fichier de log
// =====================================================================

void setupLogFile()
{
  Serial.println("Initialisation de la carte SD...");

  if (!SD.begin(SdSsPin))
  {
    Serial.println("ERREUR SD : carte SD absente ou non detectee.");
    return;
  }

  Serial.println("SD.begin OK.");

  bool oldFile = SD.exists(fileName);

  File logFile = SD.open(fileName, FILE_WRITE);

  if (!logFile)
  {
    Serial.println("ERREUR SD : impossible d'ouvrir le fichier au demarrage.");
    Serial.print("Fichier : ");
    Serial.println(fileName);
    return;
  }

  if (!oldFile)
  {
    logFile.println(loggerName);
    logFile.println(dataHeader);
    Serial.println("Nouveau fichier cree avec en-tete.");
  }
  else
  {
    Serial.println("Fichier existant trouve.");
  }

  logFile.close();

  Serial.print("Fichier utilise : ");
  Serial.println(fileName);
}


// =====================================================================
// Ecriture d'une ligne dans le fichier
// =====================================================================

void logData(String rec)
{
  File logFile = SD.open(fileName, FILE_WRITE);

  if (!logFile)
  {
    Serial.println("ERREUR SD : impossible d'ouvrir le fichier pour ecriture.");
    Serial.print("Fichier : ");
    Serial.println(fileName);
    return;
  }

  logFile.println(rec);
  logFile.close();

  Serial.println("Ecriture SD OK.");
}


// =====================================================================
// Lecture du capteur MaxBotix
// =====================================================================

void readSonar(float &volts, float &distance_mm)
{
  // -------------------------------------------------------------------
  // 1. Allumage du capteur
  // -------------------------------------------------------------------

  digitalWrite(SensorPower, HIGH);

  // Temps de demarrage du capteur
  delay(500);


  // -------------------------------------------------------------------
  // 2. Lecture de plusieurs mesures analogiques
  // -------------------------------------------------------------------

  float somme_volts = 0.0;

  for (int i = 0; i < NbMesures; i++)
  {
    // Lecture brute sur AIN3 de l'ADS1115
    int16_t raw = ads.readADC_SingleEnded(SonarChannel);

    // Conversion de la valeur brute en tension
    float mesure_volts = ads.computeVolts(raw);

    // Addition de la tension mesuree
    somme_volts += mesure_volts;

    // Petite pause entre deux lectures
    delay(150);
  }


  // -------------------------------------------------------------------
  // 3. Calcul de la tension moyenne
  // -------------------------------------------------------------------

  volts = somme_volts / NbMesures;


  // -------------------------------------------------------------------
  // 4. Conversion tension vers distance
  // -------------------------------------------------------------------

  // Formule MaxBotix pour un capteur 5 m :
  //
  // distance_mm = Vout x 5120 / VCC
  
  distance_mm = volts / VCC;
  distance_mm = distance_mm / (1.0 / 5120.0);


  // -------------------------------------------------------------------
  // 5. Extinction du capteur
  // -------------------------------------------------------------------

  digitalWrite(SensorPower, LOW);
}


// =====================================================================
// Creation d'un enregistrement CSV
// =====================================================================

String createDataRecord()
{
  DateTime now = rtc.now();

  String data = "";


  // -------------------------------------------------------------------
  // Date au format YYYY-MM-DD
  // -------------------------------------------------------------------

  data += String(now.year());
  data += "-";

  if (now.month() < 10) data += "0";
  data += String(now.month());
  data += "-";

  if (now.date() < 10) data += "0";
  data += String(now.date());

  data += " ";


  // -------------------------------------------------------------------
  // Heure au format HH:MM:SS
  // -------------------------------------------------------------------

  if (now.hour() < 10) data += "0";
  data += String(now.hour());
  data += ":";

  if (now.minute() < 10) data += "0";
  data += String(now.minute());
  data += ":";

  if (now.second() < 10) data += "0";
  data += String(now.second());

  data += ",";


  // -------------------------------------------------------------------
  // Mesure MaxBotix
  // -------------------------------------------------------------------

  float volts = 0.0;
  float distance_mm = 0.0;

  readSonar(volts, distance_mm);

  data += String(volts, 4);
  data += ",";

  data += String(distance_mm, 0);

  return data;
}


// =====================================================================
// Setup
// =====================================================================

void setup()
{
  Serial.begin(57600);

  Wire.begin();
  rtc.begin();

  pinMode(SensorPower, OUTPUT);
  digitalWrite(SensorPower, LOW);

  if (!ads.begin(0x48))
  {
    Serial.println("ADS1115 non detecte !");
    while (1);
  }

  ads.setGain(GAIN_ONE);

  setupLogFile();

  Serial.println();
  Serial.println(loggerName);
  Serial.println(dataHeader);
}


// =====================================================================
// Loop
// =====================================================================

void loop()
{
  String dataRec = createDataRecord();

  logData(dataRec);

  Serial.println(dataRec);

  delay(sampleinterval_ms);
}
