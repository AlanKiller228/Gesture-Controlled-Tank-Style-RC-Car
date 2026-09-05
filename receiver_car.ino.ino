#include <esp_now.h>
#include <WiFi.h>


const int ENA = 27;
const int IN1 = 25;
const int IN2 = 26;

const int IN3 = 32;
const int IN4 = 33;
const int ENB = 4;

typedef struct PacketData {
  int8_t pitch;
  int8_t roll;
} PacketData;

PacketData receivedData;

void moveLeftMotor(int speed) {
  if (speed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
  } else if (speed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, abs(speed));
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }
}

void moveRightMotor(int speed) {
  if (speed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
  } else if (speed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(speed));
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  int pitch = receivedData.pitch; 
  int roll  = receivedData.roll;  

  int throttle = map(pitch, -90, 90, -255, 255);
  int steering = map(roll, -90, 90, -255, 255);

  int leftMotorSpeed  = throttle + steering;
  int rightMotorSpeed = throttle - steering;

  leftMotorSpeed  = constrain(leftMotorSpeed, -255, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, -255, 255);


  if (leftMotorSpeed > 0 && leftMotorSpeed < 150)   leftMotorSpeed = 150;
  if (leftMotorSpeed < 0 && leftMotorSpeed > -150)  leftMotorSpeed = -150;

  if (rightMotorSpeed > 0 && rightMotorSpeed < 150)  rightMotorSpeed = 150;
  if (rightMotorSpeed < 0 && rightMotorSpeed > -150) rightMotorSpeed = -150;

  moveLeftMotor(leftMotorSpeed);
  moveRightMotor(rightMotorSpeed);

  Serial.printf("Pitch: %d deg | Roll: %d deg | Left: %d | Right: %d\n", 
                pitch, roll, leftMotorSpeed, rightMotorSpeed);
}

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed!");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver Ready");
}

void loop() {
}