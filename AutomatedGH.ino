#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// From BME280 Documentation:
#define BME_SCK 14 //-
#define BME_MISO 12//-
#define BME_MOSI 13//-
#define BME_CS 15//-

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); //-
//.

int temp;
int humi;
int soil;

int delayTime = 100;

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  

byte waterSymbol[8] = {
	0b00000,
	0b00000,
	0b01110,
	0b10001,
	0b10001,
	0b10001,
	0b01110,
	0b00000
};
byte emptySymbol[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b01010,
	0b00100,
	0b01010,
	0b00000,
	0b00000
};


void setup() {
    Serial.begin(115200);
    while(!Serial); 

    pinMode(16, OUTPUT);   
    pinMode(0, OUTPUT);

    lcd.init();
    lcd.clear();         
    lcd.backlight();  
    lcd.createChar(0, waterSymbol);
    lcd.createChar(1, emptySymbol);

    unsigned status;
    status = bme.begin();  
    // Error detection.
    if (!status) {
        lcd.setCursor(0, 0);
        lcd.print("Could not find a");
        lcd.setCursor(0, 1);
        lcd.print("valid BME280");
        while (1) delay(10);
    }
    digitalWrite(16, 1);
    digitalWrite(0, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Climate Monitor");
    delay(5000);
    lcd.clear();
    digitalWrite(16, 0);
    digitalWrite(0, 0);
}


void loop() { 
    printValues();
    delay(delayTime);
}

void printValues() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Temp     Humi");
    delay(200);
    lcd.setCursor(0, 1);
    float temp = bme.readTemperature();
    lcd.print(temp); // Print temperatiure reading.
    lcd.print((char)223); // A degree symbol.
    lcd.print("C");
    lcd.setCursor(9, 1);
    lcd.print(bme.readHumidity()); // Prints humidity reading.
    lcd.print("%");
    // Needs to delay 3 seconds.
    delay(3000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" L  | Soil |  H ");
    delay(200);
    lcd.setCursor(7, 0);
    soil = analogRead(0);
    int soilMaped = map(soil, 0, 1024, 0, 16);
    lcd.setCursor(0, 1);
    for (int i = 1; i <= 16; i++)
    {
      if (soilMaped >= i)
      {
        lcd.write(byte(0));
      }
      else
      {
        lcd.write(byte(1));
      }
    }
    if (soil < 200)
    {
      digitalWrite(16, 1);
    }
    else
    {
      digitalWrite(16, 0);
    }
   
    if (temp > 32 || temp < 10)
    {
      digitalWrite(0, HIGH);
    }
    else
    {
      digitalWrite(0, LOW);
    }
    delay(3000);
}

