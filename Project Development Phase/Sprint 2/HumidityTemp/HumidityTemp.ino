#include <WiFiManager.h>
#include "time.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP280.h>
#include <math.h>
//#include "soc/soc.h"
//#include "soc/rtc_cntl_reg.h"
#define DHTPIN 4
#define DHTTYPE DHT11
#define BMP_SDA 21
#define BMP_SCL 22
#define rainAnalog 35
#define rainDigital 34
#define moistureDigital 32

Adafruit_BMP280 bmp280;
DHT dht(DHTPIN, DHTTYPE);
int lcdColumns = 16;
int lcdRows = 2;
float h;
float t;
int soilMoisture;
bool firstConnect = true;
LiquidCrystal_I2C lcd1(0x27, lcdColumns, lcdRows);
LiquidCrystal_I2C lcd2(0x23, lcdColumns, lcdRows);
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 19800;
unsigned long startMillis1;
unsigned long startMillis2;
unsigned long currentMillis;
const unsigned long period1 = 3000;
const unsigned long period2 = 1000;
//26, 27, 25, 33
int relay1 = 33;
int relay2 = 25;
int relay3 = 26;
int relay4 = 27;

void displayLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("No Internet");
    lcd1.setCursor(0, 1);
    lcd1.print("Connect Again");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %I:%M:%S ");
  lcd1.setCursor(0, 0);
  lcd1.print("Time:");
  lcd1.print(&timeinfo, "%I:%M:%S");
  lcd1.setCursor(0, 1);
  lcd1.print("Date:");
  lcd1.print(&timeinfo, "%b/%d/%Y");
}

void displayToggleOnRain() {
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("H:");
  lcd2.print(h);
  lcd2.print(" ");
  lcd2.print("T:");
  lcd2.print(t);

  lcd2.setCursor(4, 1);
  lcd2.print("Raining!");
}

void displaySensorReadings() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  float altitude = bmp280.readAltitude(1011.18);
  float pressure = (bmp280.readPressure() / 100);
  int rainAnalogVal = analogRead(rainAnalog);
  int rainDigitalVal = digitalRead(rainDigital);
  soilMoisture = digitalRead(moistureDigital);
  Serial.println(rainAnalogVal);
  Serial.println(rainDigitalVal);
  if (rainDigitalVal == 0) {
    displayToggleOnRain();
  }
  else {
    lcd2.setCursor(0, 0);
    lcd2.print("H:");
    lcd2.print(h);
    lcd2.print(" ");
    lcd2.print("T:");
    lcd2.print(t);
    Serial.print(h);
    Serial.print(t);

    lcd2.setCursor(0, 1);
    lcd2.print("P:");
    lcd2.print(pressure);
    lcd2.print(" ");
    lcd2.print("A:");
    lcd2.print(altitude);
  }
}

void wifiNotConnected() {
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Please connect");
  lcd1.setCursor(0, 1);
  lcd1.print("192.168.4.9");

  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("Please connect");
  lcd2.setCursor(0, 1);
  lcd2.print("192.168.4.9");
}

void setup() {
  Serial.begin(115200);
  WiFiManager wm;
  wm.resetSettings();
  bool res;
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  lcd1.begin();
  lcd1.backlight();
  lcd2.begin();
  lcd2.backlight();
  bmp280.begin(0x76);
  pinMode(rainDigital, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(moistureDigital, INPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if (firstConnect) {
    Serial.println("Wifi display function called");
    wifiNotConnected();
  }
  res = wm.autoConnect("Grow Smart", "password");
  if (!res) {
    Serial.println("Failed to connect");
  }
  else{
    Serial.println("connected");
    firstConnect = false;
  }
  displayLocalTime();
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
  dht.begin();
  startMillis1 = millis();
  startMillis2 = millis();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - startMillis1 >= period1) {
    displaySensorReadings();
    startMillis1 = currentMillis;
  }
  if (currentMillis - startMillis2 >= period2) {
    displayLocalTime();
    startMillis2 = currentMillis;
  }

}
