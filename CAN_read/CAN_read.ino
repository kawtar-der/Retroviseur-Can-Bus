#include <SPI.h>
#include <mcp2515.h>

#define CS_PIN 10  // Chip Select pin for MCP2515
MCP2515 mcp2515(CS_PIN);

#define MIRROR_UP_COMMAND 0x01
#define MIRROR_DOWN_COMMAND 0x02
#define MIRROR_LEFT_COMMAND 0x03
#define MIRROR_RIGHT_COMMAND 0x04

// Motor A
#define enA  9
#define in1  A0
#define in2  A1

// Motor B
#define enB  3
#define in3  A2
#define in4  A3

struct can_frame canMsg;

void setup() {
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");

  // Setup motor pins  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" ");
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");

    for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
      Serial.print(canMsg.data[i], HEX);
      Serial.print(" ");
    }

    Serial.println();

    // Check the CAN ID and process the message
    if (canMsg.can_id == 0x101) {
      // Message from Arduino 2 to control Motor 1
      if (canMsg.data[0] == MIRROR_UP_COMMAND) {
        MIRROR_UP();
        Serial.println("Message UP ");
      } else if (canMsg.data[0] == MIRROR_DOWN_COMMAND) {
        MIRROR_DOWN();
        Serial.println("Message DOWN ");
      } else if (canMsg.data[0] == MIRROR_LEFT_COMMAND) {
        MIRROR_LEFT();
        Serial.println("Message LEFT ");
      } else if (canMsg.data[0] == MIRROR_RIGHT_COMMAND) {
        MIRROR_RIGHT();
      Serial.println("Message RIGHT ");
      }
    }
  }
}

void MIRROR_UP() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 200);
  delay(500);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}

void MIRROR_DOWN() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 200);
  delay(500);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}

void MIRROR_LEFT() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 200);
  delay(500);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}

void MIRROR_RIGHT() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 200);
  delay(500);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}
