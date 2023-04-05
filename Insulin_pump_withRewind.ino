/* This program was written by David Humphries for NEA Engineering:
 Anjali Pal
 Elyssa Snively
 Natalie Salema
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

int bluePin = 2;    //IN1 on the ULN2003 Board, BLUE end of the Blue/Yellow motor coil
int pinkPin = 3;    //IN2 on the ULN2003 Board, PINK end of the Pink/Orange motor coil
int yellowPin = 4;  //IN3 on the ULN2003 Board, YELLOW end of the Blue/Yellow motor coil
int orangePin = 5;  //IN4 on the ULN2003 Board, ORANGE end of the Pink/Orange motor coil
int oneunit = 48;   //number of times through the loop (steps) to dispense one unit
int dose = 0;       //the number of units the patient requests 
int total_dose = 0;   //counts the total dose
int dose_switch = 9;
int confirm_switch = 10;
int confirm = 0;
int i = 0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

static const unsigned char PROGMEM image_data_CATACat[] = {   
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x38, 0x33, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0x5f, 0xf7, 0xdf, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xf7, 0xeb, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xc8, 0x7c, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xf7, 0xc7, 0x9f, 0x7f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xf8, 0xf8, 0xfd, 0xdf, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xf8, 0x7f, 0x1f, 0xbf, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xfb, 0xfc, 0x3f, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xfb, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xf8, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0x7d, 0x7f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xf7, 0xef, 0x9f, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xf6, 0xa8, 0x0f, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xf7, 0x8b, 0x87, 0xc4, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xf3, 0x17, 0xf9, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xfb, 0x03, 0xfe, 0x70, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf9, 0x01, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xfe, 0x80, 0x7f, 0xfc, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xfe, 0x80, 0x3f, 0xff, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xa0, 0x3f, 0xff, 0x8b, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x0e, 0x3e, 0xb0, 0x7f, 0xff, 0x8d, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x2f, 0x8f, 0x30, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xe7, 0xc1, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf7, 0xe1, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf3, 0xf3, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xfb, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf3, 0xc1, 0xcf, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xef, 0x9f, 0xf3, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe1, 0xdf, 0xf1, 0xff, 0x37, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe6, 0x3f, 0xfe, 0xff, 0xcb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xe4, 0x3b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf3, 0xf7, 0xef, 0xe1, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf1, 0x00, 0xf9, 0x6b, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf9, 0x84, 0x3c, 0x33, 0xfb, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf9, 0xc0, 0x1f, 0x3f, 0xfd, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf8, 0xe1, 0x02, 0x0f, 0xfe, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x71, 0x80, 0x07, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0xb9, 0xe0, 0x07, 0xf0, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x59, 0xff, 0xe3, 0xf8, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x9c, 0xff, 0xf3, 0xf1, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0x7f, 0xf9, 0xc3, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0x7f, 0xf8, 0x83, 0x9b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x3f, 0xfc, 0x08, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xbf, 0xff, 0x00, 0x3c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x9f, 0xff, 0xc0, 0x1e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xcf, 0xff, 0xc0, 0x0f, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xe7, 0xff, 0xed, 0x87, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xf7, 0xff, 0xf7, 0xf3, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0x77, 0xff, 0xff, 0xfc, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc9, 0x43, 0xff, 0xff, 0xfe, 0x7c, 0x3f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0x3e, 0x9f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xcf, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xec, 0x3f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};  


void setup() {  
 Serial.begin(9600);  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  pinMode(bluePin, OUTPUT);
  pinMode(pinkPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(orangePin,OUTPUT);
  pinMode(dose_switch, INPUT);
  pinMode(confirm_switch, INPUT);

  // Draw bitmap on the screen
  display.clearDisplay();
  display.drawBitmap(0, 0, image_data_CATACat, 128, 64, 1);
  //display.startscrollright(0x00, 0x0F);
  display.display();
  delay(2000);
  
  // Display static text
  display.clearDisplay();  
  display.setFont();
  display.setTextSize(1);             
  display.setTextColor(WHITE);
  display.setCursor(20,5);
  display.println("NEA ENGINEERING");
  //display.display();    
  display.setCursor(15,26);
  display.setFont(&FreeSerif12pt7b);    
  display.println("INSULIN");
  display.setCursor(30,54);
  display.setFont(&FreeSerif12pt7b);    
  display.println("PUMP");
  display.display();
  delay(2000);  
}

void loop() {

  digitalWrite(bluePin, LOW);
  digitalWrite(pinkPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(orangePin, LOW);

  display.clearDisplay();  
  display.setFont();
  display.setTextSize(1);             
  display.setTextColor(WHITE);
  display.setCursor(20,5);
  display.println("NEA ENGINEERING");
  display.setCursor(12,26);
  display.setFont(&FreeSerif12pt7b);    
  display.println("SET DOSE");
  display.setCursor(55,54);
  display.setFont(&FreeSerif12pt7b);    
  display.println(dose);
  display.display();
  
  i = 2;
  
  while (digitalRead(confirm_switch) == HIGH && digitalRead(dose_switch) == HIGH){
   //set display
    if (i != 0){
    display.clearDisplay();  
    display.setFont();
    display.setTextSize(1);             
    display.setTextColor(WHITE);
    display.setCursor(20,5);
    display.println("NEA ENGINEERING");
    display.setCursor(12,38);
    display.setFont(&FreeSerifBoldItalic9pt7b);    
    display.println("REWINDING");
    display.display();
    i--;
    }

    //reverse motor
      digitalWrite(bluePin, LOW);
      digitalWrite(pinkPin, LOW);
      digitalWrite(yellowPin, LOW);
      digitalWrite(orangePin, HIGH);
      delay(5);
     
      digitalWrite(bluePin, LOW);
      digitalWrite(pinkPin, LOW);
      digitalWrite(yellowPin, HIGH);
      digitalWrite(orangePin, LOW);
      delay(5);
      
      digitalWrite(bluePin, LOW);
      digitalWrite(pinkPin, HIGH);
      digitalWrite(yellowPin, LOW);
      digitalWrite(orangePin, LOW);
      delay(5);
      
      digitalWrite(bluePin, HIGH);
      digitalWrite(pinkPin, LOW);
      digitalWrite(yellowPin, LOW);
      digitalWrite(orangePin, LOW);
      delay(5); 
      
      dose = 0;
  }

  if (digitalRead(confirm_switch) == LOW && digitalRead(dose_switch) == HIGH){
    dose++;
    delay(300);
  }
  else if (digitalRead(confirm_switch) == HIGH && digitalRead(dose_switch) == LOW){
    total_dose = dose*oneunit;
   while (total_dose != 0){
      dose = 0;
      digitalWrite(bluePin, HIGH);
      digitalWrite(pinkPin, LOW);
      digitalWrite(yellowPin, LOW);
      digitalWrite(orangePin, LOW);
      delay(5);
     
      digitalWrite(bluePin, LOW);
      digitalWrite(pinkPin, HIGH);
      digitalWrite(yellowPin, LOW);
      digitalWrite(orangePin, LOW);
      delay(5);
      
      digitalWrite(bluePin, LOW);
      digitalWrite(pinkPin, LOW);
      digitalWrite(yellowPin, HIGH);
      digitalWrite(orangePin, LOW);
      delay(5);
      
      digitalWrite(bluePin, LOW);
      digitalWrite(pinkPin, LOW);
      digitalWrite(yellowPin, LOW);
      digitalWrite(orangePin, HIGH);
      delay(5);
      
      total_dose--;
      }
     }
      else {
      }
    }

  
