#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <esp_now.h>
#include <WiFi.h>

Adafruit_MPU6050 mpu;


uint8_t receiverAddress[] = {your ESP wifi Address};

typedef struct PacketData {
  int8_t pitch;
  int8_t roll;
} PacketData;

PacketData dataToSend;
esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Статус передачи: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "УСПЕШНО" : "ОШИБКА ДОСТАВКИ");
}

void setup() {
  Serial.begin(115200);
  
 
  Wire.begin(21, 22);


  if (!mpu.begin()) {
    Serial.println("MPU6050 не найден! Проверьте проводку.");
    while (1) delay(10);
  }
  Serial.println("MPU6050 успешно инициализирован!");


  WiFi.mode(WIFI_STA);

 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Ошибка инициализации ESP-NOW!");
    return;
  }


  esp_now_register_send_cb(OnDataSent);


  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Не удалось добавить приемник!");
    return;
  }
  Serial.println("Перчатка полностью готова к работе!");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  float pitch_f = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;
  float roll_f  = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;

  dataToSend.pitch = (int8_t)constrain(pitch_f, -90, 90);
  dataToSend.roll  = (int8_t)constrain(roll_f, -90, 90);


  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &dataToSend, sizeof(dataToSend));

  delay(50); 
}