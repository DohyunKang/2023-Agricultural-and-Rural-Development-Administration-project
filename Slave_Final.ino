#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>
#include <MQ135.h>
#include <MQUnifiedsensor.h>
//slave code
#define DHTPIN1 22
#define DHTTYPE DHT11  
DHT dht1(DHTPIN1, DHTTYPE);

#define placa "ESP-32"
#define Voltage_Resolution 3.3
#define pin1 39

#define type "MQ-135"
#define ADC_Bit_Resolution 12

#define uS_TO_S_FACTOR 10000
#define TIME_TO_SLEEP  5 
RTC_DATA_ATTR int bootCount = 0;

float RatioMQ135CleanAir = 0.6;

struct SensorData {
  float temperature;
  float humidity;
  float ppm;
};

SensorData sensorData[3];

MQUnifiedsensor MQ135_1(placa, Voltage_Resolution, ADC_Bit_Resolution, pin1, type);

/*
master 1 mac 주소
uint8_t masterMAC[] = {0xC0,0x49,0xEF,0xBC,0x02,0x9C};

master 2 mac 주소
uint8_t masterMAC[] = {0xC8,0xF0,0x9E,0x28,0x27,0x3C};

master 3 mac 주소
uint8_t masterMAC[] = {0xB8,0xD6,0x1A,0x82,0x0F,0xB0};
*/

uint8_t masterMAC[] = {0xB8,0xD6,0x1A,0x82,0x0F,0xB0};


// Structure example to send data
// Must match the receiver structure
typedef struct struct_data {
    int id; //슬레이브의 번호
    float tmp;//센서값 항목 추가
    float hum;
    float ppm;
}struct_data;

// Create a struct_message called Data
struct_data myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status != ESP_NOW_SEND_SUCCESS){
    esp_deep_sleep_start();
  }
  else{
    delay(1000);
  }
}

void setup() {
  Serial.println(WiFi.macAddress());
  print_wakeup_reason();

  // Init Serial Monitor
  Serial.begin(115200);
  delay(100);

  Setup_DHT11();
  Setup_MQ135();
  Reset_SensorData();

  pinMode(36,INPUT);

  // Set device as a Wi-Fi Station

  WiFi.mode(WIFI_STA);
  //connectToWiFi();
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    esp_deep_sleep_start();
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, masterMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    esp_deep_sleep_start();
    return;
  }
}

void loop() {
  // Set values to send
  myData.id = 1;
  
  DHT11_Read(myData.id);
  MQ135_Read(myData.id);
  Serial.println();
  
  myData.tmp = sensorData[myData.id-1].temperature;//센서값 
  myData.hum = sensorData[myData.id-1].humidity;
  myData.ppm = sensorData[myData.id-1].ppm;
 
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(masterMAC, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    //delay(50000);
    delay(2000);
  }
  else {
    Serial.println("Error sending the data");
    esp_deep_sleep_start();
  }
  delay(100000);
  //delay(1000);
}

void Reset_SensorData(){
  Serial.println("Reset SensorData!");
  sensorData[0].temperature = 0.0;
  sensorData[0].humidity = 0.0;
  sensorData[0].ppm = 0.0;

  sensorData[1].temperature = 0.0;
  sensorData[1].humidity = 0.0;
  sensorData[1].ppm = 0.0;
  
  sensorData[2].temperature = 0.0;
  sensorData[2].humidity = 0.0;
  sensorData[2].ppm = 0.0;
  }

void Setup_DHT11(){
  Serial.println("Setup DHT11!");
  dht1.begin();
}

void Setup_MQ135() {
  Serial.println("Setup MQ135!");
  MQ135_1.setRegressionMethod(1);

  MQ135_1.setA(110.47); MQ135_1.setB(-2.862);

  MQ135_1.init();

  Serial.print("Calibrating");
  float calcR0_1 = 0;

  for(int i=1; i<=10; i++){
    MQ135_1.update();

    calcR0_1 += MQ135_1.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135_1.setR0(calcR0_1/10);

  Serial.println("done!");
}

void DHT11_Read(int id){
  float temperature_1 = dht1.readTemperature();
  float humidity_1 = dht1.readHumidity();

  Serial.print("Temperature_1 : ");
  Serial.print(temperature_1);
 
  Serial.print("\t Humidity_1 : ");
  Serial.print(humidity_1);

  sensorData[id-1].temperature = temperature_1;
  sensorData[id-1].humidity = humidity_1;
}

void MQ135_Read(int id){
  MQ135_1.update();
  float CO2_1 = MQ135_1.readSensor();
  
  Serial.print("\t MQ135_1 : ");
  Serial.print(CO2_1);

  sensorData[id-1].ppm = CO2_1;
}

// ESP32가 sleep 모드에서 wakeup 모드로 전환되는 이유 출력
void print_wakeup_reason(){
 esp_sleep_wakeup_cause_t wakeup_reason;
 wakeup_reason = esp_sleep_get_wakeup_cause();
 switch(wakeup_reason)
 {
  case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
  case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
  case 3  : Serial.println("Wakeup caused by timer"); break;
  case 4  : Serial.println("Wakeup caused by touchpad"); break;
  case 5  : Serial.println("Wakeup caused by ULP program"); break;
  default : Serial.println("Wakeup was not caused by deep sleep"); break;
 }
}
