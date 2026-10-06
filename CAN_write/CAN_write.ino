#include <LiquidCrystal.h>
#include <SPI.h>
#include <mcp2515.h>

const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define CS_PIN 10  
MCP2515 mcp2515(CS_PIN);

#define UP_BUTTON_PIN 7
#define DOWN_BUTTON_PIN 6
#define LEFT_BUTTON_PIN 5
#define RIGHT_BUTTON_PIN 4
#define switch_right  1
#define switch_left  0

struct can_frame canMsg1;

void setup() {
      Serial.begin(115200);
  while (!Serial);

  canMsg1.can_id = 0x101;
  canMsg1.can_dlc = 1;
  canMsg1.data[0] = 0x01;
  canMsg1.data[1] = 0x02; 
  canMsg1.data[2] = 0x03;
  canMsg1.data[3] = 0x04; 
  canMsg1.data[4] = 0x05;
  canMsg1.data[5] = 0x06;
  canMsg1.data[6] = 0x07;
  canMsg1.data[7] = 0x08;
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.println("Example: Write to CAN");

    pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(switch_right, INPUT_PULLUP);
    pinMode(switch_left, INPUT_PULLUP);
    lcd.begin(20, 4);
      
}
void loop() {
   lcd.setCursor(0,0);
        lcd.print("CONTROLE: "); 
    canMsg1.can_id = 0x101;
     lcd.setCursor(0,2);
        lcd.print("BUTTON: ");
    if (digitalRead(switch_left) == 0 || digitalRead(switch_right) == 0) {
       if (digitalRead(switch_left) == 0 ) {
        lcd.setCursor(0,1);
        lcd.print(" Mirror Left"); 
       }
       else{
         lcd.setCursor(0,1);
        lcd.print(" Mirror Right"); 
       }
        buttonUp();
        buttonDown();
        buttonLeft();
        buttonRight();
        delay(100);
    }
    else{
        lcd.setCursor(0, 3);
        lcd.print("No message found ");
   
    }
}

void buttonUp() {
  
    if (digitalRead(UP_BUTTON_PIN) == 0) {
        if (digitalRead(switch_left) == 0) {
            canMsg1.data[0] = 0x01; 
        } else {
            canMsg1.data[0] = 0x05; 
        }
        mcp2515.sendMessage(&canMsg1);
        Serial.println("Messages UP");
        lcd.setCursor(0, 3);
        lcd.print("   message UP    ");
        delay(1000);
        lcd.clear();
    }
}

void buttonDown() {
    if (digitalRead(DOWN_BUTTON_PIN) == 0) {
        if (digitalRead(switch_left) == 0) {
            canMsg1.data[0] = 0x02;
        } else {
            canMsg1.data[0] = 0x06;
        }
        mcp2515.sendMessage(&canMsg1);
        Serial.println("Messages DOWN");
        lcd.setCursor(0, 3);
        lcd.print("   message DOWN   ");
         delay(1000);
         lcd.clear();
    }
}

void buttonLeft() {
    if (digitalRead(LEFT_BUTTON_PIN) == 0) {
        if (digitalRead(switch_left) == 0) {
            canMsg1.data[0] = 0x03;  
        } else {
            canMsg1.data[0] = 0x07; 
        }
        mcp2515.sendMessage(&canMsg1);
        Serial.println("  Messages LEFT   ");
        lcd.setCursor(0, 3);
        lcd.print("   message LEFT  ");
         delay(1000);
         lcd.clear();
        
    }
}

void buttonRight() {
    if (digitalRead(RIGHT_BUTTON_PIN) == 0) {
        if (digitalRead(switch_left) == 0) {
            canMsg1.data[0] = 0x04; 
        } else {
            canMsg1.data[0] = 0x08; 
        }
        mcp2515.sendMessage(&canMsg1);
        Serial.println("Messages RIGHT");
        lcd.setCursor(0, 3);
        lcd.print("   message RIGHT   ");
         delay(1000);
         lcd.clear();
    }
}
