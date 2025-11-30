#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT sensor
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Fan pin and thresholds
#define FANPIN 3
#define FAN_ON_TEMP 35
#define FAN_OFF_TEMP 30

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Intro screen
  display.setCursor(15, 20); // Area kuning
  display.print("Hello Fauzan,");
  display.setCursor(5, 35);
  display.print("Have a Nice Day :)");
  display.display();
  delay(3000);

  // Setelah intro, masuk init
  display.clearDisplay();
  display.setCursor(20, 4);
  display.print("Automatic Fan");
  display.display();
  delay(1500);

  dht.begin();
  pinMode(FANPIN, OUTPUT);
  digitalWrite(FANPIN, LOW);
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Sensor Error!");
    display.display();
    delay(2000);
    return;
  }

  // Normalisasi nilai ke panjang bar
  int barSuhu = map((int)suhu, 0, 50, 0, 100);
  int barKelembapan = map((int)kelembapan, 0, 100, 0, 100);

  display.clearDisplay();
  display.setTextSize(1);

  // Header (bagian kuning)
  display.setCursor(0, 0);
  display.print("T:");
  display.print(suhu, 1);
  display.write(247);
  display.print("C");

  display.setCursor(70, 0);
  display.print("H:");
  display.print(kelembapan, 0);
  display.print("%");

  // Bar suhu
  display.setCursor(0, 20);
  display.print("Temp");
  display.drawRect(0, 30, 100, 8, SSD1306_WHITE);
  display.fillRect(0, 30, barSuhu, 8, SSD1306_WHITE);

  // Bar kelembapan
  display.setCursor(0, 42);
  display.print("Humid");
  display.drawRect(0, 52, 100, 8, SSD1306_WHITE);
  display.fillRect(0, 52, barKelembapan, 8, SSD1306_WHITE);

  // Fan status
  if (suhu > FAN_ON_TEMP) {
    digitalWrite(FANPIN, HIGH);
    display.setCursor(105, 52);
    display.print("ON ");
  } else if (suhu < FAN_OFF_TEMP) {
    digitalWrite(FANPIN, LOW);
    display.setCursor(105, 52);
    display.print("OFF");
  }

  display.display();
  delay(2000);
}
