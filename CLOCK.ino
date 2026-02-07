#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h> 
#include <Wire.h>
#include <Adafruit_AHTX0.h> 

#define TFT_CS    7
#define TFT_DC    2
#define TFT_RST   1
#define TFT_MOSI  6  
#define TFT_SCLK  4  

#define TOUCH_PIN 1   
#define LED_PIN   3 
#define SPEAKER   0   

#define I2C_SDA   8
#define I2C_SCL   9

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Adafruit_AHTX0 aht;

int hours = 8;
int seconds = 50;
int minutes = 59;
bool ledState = false;
bool lastTouchState = false;
int alarmoff = false;

void setup() {
  Serial.begin(115200);
  pinMode(1, INPUT);
  pinMode(3, OUTPUT);
  pinMode(0, OUTPUT);

  Wire.begin(I2C_SDA, I2C_SCL);
  
  tft.init(240, 320);           
  tft.setRotation(1);      
  tft.invertDisplay(false); 


  uint8_t madctl_data = 0x28; 
  tft.sendCommand(0x36, &madctl_data, 1);
  // -------------------------------------

  tft.fillScreen(ST77XX_BLACK);
  aht.begin();

}

void loop() {
  if (hours == 9 && minutes >= 0 && minutes <= 5){
    if (alarmoff == false){
      digitalWrite(SPEAKER, HIGH);
    } else{
      digitalWrite(SPEAKER, LOW);
      digitalWrite(LED_PIN, LOW);
    }
    
  }
  tft.setCursor(127, 100);
  tft.setTextColor(ST77XX_MAGENTA); 
  tft.setTextSize(5);
  tft.print(hours);
  tft.print(":");
  tft.print(minutes);

  seconds ++;
  if (hours == 24){
    tft.fillRect(127, 100, 200, 40, ST77XX_BLACK);
    hours = 0;
    Serial.println(hours);
  } else {
    if (seconds == 60){
      tft.fillRect(127, 100, 200, 40, ST77XX_BLACK);
      seconds = 0;
      minutes ++;
      Serial.println(minutes);
      if (minutes == 60){
        tft.fillRect(127, 100, 200, 40, ST77XX_BLACK);
        hours ++;
        Serial.println(hours);
        minutes = 0;
        Serial.println(minutes);
      }
    }
  }

  sensors_event_t humidity, temp;
  if (aht.getEvent(&humidity, &temp)) {
    tft.fillRect(160, 0, 80, 40, ST77XX_BLACK);
    tft.setCursor(150, 10);
    tft.setTextColor(ST77XX_MAGENTA);
    tft.setTextSize(2);
    tft.print((int)temp.temperature);
    tft.print(" C");
  }
  bool currentTouchState = digitalRead(TOUCH_PIN);

  if (currentTouchState == HIGH && lastTouchState == LOW) {
    ledState = !ledState;            
    digitalWrite(LED_PIN, ledState); 
    alarmoff = !alarmoff;
  }
  lastTouchState = currentTouchState;
  delay(1000); 
}