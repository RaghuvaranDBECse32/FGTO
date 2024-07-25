#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Constants
#define DHTPIN 7      // Pin where DHT-11 is connected
#define DHTTYPE DHT11 // DHT-11 type
#define SOIL_SENSOR_PIN A0 // Soil sensor pin

// Initialize I2C LCD at address 0x27 for 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Function to read temperature and humidity
void readTempHumidity(int &temp, int &humidity) {
    humidity = dht.readHumidity();
    temp = dht.readTemperature();
}

// Function to update LCD with temperature, humidity, and soil moisture
void updateLCD(int temp, int humidity, int sensorValue) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("H: " + String(humidity) + "%, T: " + String(temp) + "C");

    lcd.setCursor(0, 1);
    lcd.print("Soil: " + String(sensorValue));
    lcd.print(", Fan: ");
    lcd.print((humidity >= 60) ? "ON" : "OFF");
}

void setup() {
    Serial.begin(9600);
    dht.begin();
    lcd.init(); // initialize the LCD
    lcd.backlight(); // turn on the LCD backlight
    
    // Set the control pins for outputs
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
}

void loop() {
    int humidity = dht.readHumidity();
    int temp = dht.readTemperature();

    int sensorValue = analogRead(SOIL_SENSOR_PIN);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" °C");

    // Fan control based on humidity
    digitalWrite(3, (humidity >= 60) ? HIGH : LOW);

    // Pump control based on soil sensor value
    digitalWrite(4, (sensorValue >= 750) ? HIGH : LOW);

    updateLCD(temp, humidity, sensorValue);

    delay(1000); // Delay for 1 second
}