#include <LiquidCrystalRus.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>                  // Подключение библиотеки TinyGPS
#include <SoftwareSerial.h>             // Подключение библиотеки SoftwareSerial
float lat = 28.5458, lon = 77.1703;

String latit;
String longit;
String sat;
int RXPin = 2;                          // Указываем вывод подключения RX  
int TXPin = 3;                          // Указываем вывод подключения TX

int GPSBaud = 9600;                     // Указываем скорость передачи с NEO-6M

LiquidCrystal_I2C lcd(0x3f, 20, 4);

TinyGPSPlus gps;                        // Создание объекта 
SoftwareSerial gpsSerial(RXPin, TXPin); // Создайте последовательный связь под названием "gpsSerial"

void setup()
{
  Serial.begin(9600);                   // Запускаем последовательный порт с ПК на скорости 9600
  gpsSerial.begin(GPSBaud);             // Запустите последовательный порт с NEO-6M на скорости 9600
  lcd.begin(); 
  lcd.backlight();
  lcd.print("Shirota: ");
  lcd.setCursor(0,1);
  lcd.print("Dolgota: ");
  lcd.setCursor(0,2);
  lcd.print("Date:");
  lcd.setCursor(0,3);
  lcd.print("Sat: ");
}

void loop()
{
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  // Если нет данных в течении 5000 миллисекунд, пишем сообщение об ошибки
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
  
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    //while(gps.location.isValid()){
    latit = String(gps.location.lat(), 7);
    longit = String(gps.location.lng(), 7);
    sat = String(gps.satellites.value());
    lcd.setCursor(9, 0);
    lcd.print(latit);
    lcd.setCursor(9,1);
    lcd.print(longit);
    lcd.setCursor(9,2);
    lcd.print(String(gps.date.day())+"/"+String(gps.date.month())+"/"+String((gps.date.year())));
    lcd.setCursor(9,3);
    lcd.print(sat);
    delay(1200);
  }
  else
  {
    Serial.println("Location: Not Available");
    lcd.print("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
