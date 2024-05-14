#include <dummy.h>

#include <ArduinoJson.h>
#include <esp_now.h>
//#include "HX711.h" // 로드셀
#include <WiFi.h>
#include <HTTPClient.h>

/*
//찾은 캘리브레이션값을 넣어 줍니다.
#define calibration_factor 12000
*/
//DT(DOUT)로 사용하는 핀
#define LOADCELL_DOUT_PIN 15
//SCK(CLK)로 사용하는 핀
#define LOADCELL_SCK_PIN 4
 
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  5        //Time ESP32 will go to sleep (in seconds)
RTC_DATA_ATTR int bootCount = 0;

//HX711 scale;

/* 
//최신값을 몇개 저장해 두는지 개수.
int nValueCount = 15;
//최신값을 저장해 두는 배열. 위의 변수 값과 같은 크기를 설정한다.
float fValue[15];
float tare = 0;
float resultstorage[5];
boolean param = false;
*/



/*
slave 1-1 ~ 1-3 
uint8_t slave1MAC[] = {0xC8,0xF0,0x9E,0x28,0x40,0xAC}; //3개의 멀티플렉서와 연결된 칩
uint8_t slave2MAC[] = {0xC0,0x49,0xEF,0xBC,0x0A,0x78}; // MAC주소 바꿔야 함
uint8_t slave3MAC[] = {0xC8,0xF0,0x9E,0x28,0x27,0x58}; // MAC주소 바꿔야 함

slave 2-1 ~ 2-3
uint8_t slave1MAC[] = {0x94,0x3C,0xC6,0x10,0x68,0x50}; //3개의 멀티플렉서와 연결된 칩
uint8_t slave2MAC[] = {0xC8,0xF0,0x9E,0x28,0x4A,0x4C}; // MAC주소 바꿔야 함
uint8_t slave3MAC[] = {0xC8,0xF0,0x9E,0x28,0x28,0x98}; // MAC주소 바꿔야 함

slave 3-1 ~ 3-3 MAC 주소
 uint8_t slave1MAC[] = {0xC8,0xF0,0x9E,0x28,0x41,0x78}; //3개의 멀티플렉서와 연결된 칩
 uint8_t slave2MAC[] = {0xC8,0xF0,0x9E,0x28,0x35,0x6C}; // MAC주소 바꿔야 함
 uint8_t slave3MAC[] = {0xC0,0x49,0xEF,0xBC,0x0A,0x94}; // MAC주소 바꿔야 함
*/

//처음 설치 slave code
uint8_t slave1MAC[] = {0xC8,0xF0,0x9E,0x28,0x41,0x78}; //3개의 멀티플렉서와 연결된 칩
uint8_t slave2MAC[] = {0xC8,0xF0,0x9E,0x28,0x35,0x6C}; // MAC주소 바꿔야 함
uint8_t slave3MAC[] = {0xC0,0x49,0xEF,0xBC,0x0A,0x94}; // MAC주소 바꿔야 함

//wifi 코드
const char* ssid = " ";       // 여기에 WiFi SSID를 입력하세요.
const char* password = " "; // 여기에 WiFi 비밀번호를 입력하세요.

WiFiClient client;

// PHP 서버 정보
const char* serverUrl = " ";

// 디바이스 정보
const char* deviceName = "DEVICE_3";
//const int deviceName = 1;

struct SensorData {
  float temperature;
  float humidity;
  float ppm;
  float kg;
};

SensorData sensorData[3];


const int TRANSMISSION_INTERVAL = 1000;
unsigned long previousTransmissionTime = 0;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float tmp;
  float hum;
  float ppm;
  float kg;
}struct_message;

// Create a struct_message called myData
struct_message myData;

struct_message board1;
struct_message board2;
struct_message board3;
// Create an array with all the structures
struct_message boardsStruct[3]={board1, board2, board3};


esp_now_peer_info_t peerInfo;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  // Serial.print("Packet received from: ");
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.println(macStr);
  // memcpy(&myData, incomingData, sizeof(myData));
  // Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

  // Update the structures with the new incoming data
  if (mac_addr[0] == slave1MAC[0] && mac_addr[1] == slave1MAC[1] &&
      mac_addr[2] == slave1MAC[2] && mac_addr[3] == slave1MAC[3] &&
      mac_addr[4] == slave1MAC[4] && mac_addr[5] == slave1MAC[5])
    {
      Serial.print("Packet received from: ");
      snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
      Serial.println(macStr);
      memcpy(&myData, incomingData, sizeof(myData));
      Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
     
      boardsStruct[0].tmp = myData.tmp;
      boardsStruct[0].hum = myData.hum;
      boardsStruct[0].ppm = myData.ppm;
      boardsStruct[0].id = myData.id;  

       }
    

  if (mac_addr[0] == slave2MAC[0] && mac_addr[1] == slave2MAC[1] &&
      mac_addr[2] == slave2MAC[2] && mac_addr[3] == slave2MAC[3] &&
      mac_addr[4] == slave2MAC[4] && mac_addr[5] == slave2MAC[5])
    {
      Serial.print("Packet received from: ");
      snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
      Serial.println(macStr);
      memcpy(&myData, incomingData, sizeof(myData));
      Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

      boardsStruct[1].tmp = myData.tmp;
      boardsStruct[1].hum = myData.hum;
      boardsStruct[1].ppm = myData.ppm;
      boardsStruct[1].id = myData.id;  

       }
      
  if (mac_addr[0] == slave3MAC[0] && mac_addr[1] == slave3MAC[1] &&
      mac_addr[2] == slave3MAC[2] && mac_addr[3] == slave3MAC[3] &&
      mac_addr[4] == slave3MAC[4] && mac_addr[5] == slave3MAC[5])
    {
      Serial.print("Packet received from: ");
      snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
      Serial.println(macStr);
      memcpy(&myData, incomingData, sizeof(myData));
      Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

      boardsStruct[2].tmp = myData.tmp;
      boardsStruct[2].hum = myData.hum;
      boardsStruct[2].ppm = myData.ppm;
      boardsStruct[2].id = myData.id;       
    
}
}
 
void setup() {
  Serial.println(WiFi.macAddress());
  print_wakeup_reason();

  //Initialize Serial Monitor
  Serial.begin(115200);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);

  //Serial.println("HX711 kg demo");
 
  /*
  //HX711 객체를 초기화 한다.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //설정된 캘리브레이션 값을 설정 한다.
  scale.set_scale(calibration_factor);
  //영점 잡기. 현재 값을 0으로 둔다.
  scale.tare();
  */

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    esp_deep_sleep_start();
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);


  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Register slave
  //esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, slave1MAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  // Add slave
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    esp_deep_sleep_start();
    return;
  }

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  print_wakeup_reason();
  //Set timer to 5 seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
}
 
void loop() {
  int connectingCount = 0;
  unsigned long currentMillis = millis();

  /*
  //값 임시 저장
  float fValueTemp = 0.0;
  //값 합산
  float fValueSum = 0.0;
 
  
  //scale.get_units() returns a float
  fValueTemp = scale.get_units();
  //읽은 값을 합산값에 넣어 준다.
  fValueSum = fValueTemp;
  
  int i;
  for(i = 0; i < nValueCount; i = i + 1)
  {
    if(i > 0)
    {//0보다 클때만 계산
      //앞번호일수록 오래된 데이터이다.
      
      //기존에 저장된 데이터 합산
      fValueSum = fValueSum + fValue[i];
 
      //값을 앞으로 한칸씩 민다.
      fValue[i - 1] = fValue[i];
    }
  }
 
  //맨 마지막에 최신값을 넣어 준다.
  fValue[nValueCount - 1] = fValueTemp;
  float cal = fValueSum / nValueCount;
  //합산값을 평균을 낸다.

  // 다항식 보정 값 -> 보간 다항식은 스프레드시트나 Excel에서 만들면 된다.
  //float result = -0.0000588*pow(cal,4)+0.00451*pow(cal,3)-0.0672*pow(cal,2)+1.16*cal;
  */
  // 보정 전 값, 즉 센서에서 측정하는 값이 0 아래면 바로 초기화, 초기화시에는 반드시 상판위에 물건을 치워야한다.
  /*
  if (cal <= -0.1){
    Serial.println("Tare Start");
    scale.tare();
    delay(1000);
    Serial.println("tare for zero point");
  }
  // 물건을 올려놓기 전 깨끗한 상태에서 보정 값이 0 아래일 때 측정하고자 하는 물체의 보정 값을 제대로 잡아주기 위해서 넣었다. 0.18 이라는 보정 수치는 사용자 마음대로 정하면 된다.
  else{
    if(result <= 0){
      tare = result;
      //Serial.print(result-tare, 1);
     // Serial.println(" kg");
    }else{
        //Serial.print(result-tare, 1);
        //Serial.print(" kg");
        //Serial.println();
    }
  }
  */
  if (currentMillis - previousTransmissionTime >= TRANSMISSION_INTERVAL) {
    previousTransmissionTime = currentMillis;

    DynamicJsonDocument jsonDocument(4096);
    jsonDocument["device_name"] = deviceName;

    JsonArray sensorsArray1 = jsonDocument.createNestedArray("sensors");
    JsonArray sensorsArray2 = jsonDocument.createNestedArray("sensors");
    JsonArray sensorsArray3 = jsonDocument.createNestedArray("sensors");

    
    // 1번 Slave Data to Json
    if(boardsStruct[0].id == 1 && boardsStruct[0].tmp != 0)
    {
      JsonObject sensorObject1 = sensorsArray1.createNestedObject();
      sensorObject1["sensor_number"] = boardsStruct[0].id;
      sensorObject1["temperature"] = boardsStruct[0].tmp;
      sensorObject1["humidity"] = boardsStruct[0].hum;
      sensorObject1["co2"] = boardsStruct[0].ppm;
      //sensorObject1["kg"] = result - tare;

      Serial.printf("device_num value%d: %d \n",1, boardsStruct[0].id);
      Serial.printf("temp value %d: %f \n", boardsStruct[0].id, boardsStruct[0].tmp);
      Serial.printf("humi value %d: %f \n", boardsStruct[0].id, boardsStruct[0].hum);
      Serial.printf("co2 value %d: %f \n", boardsStruct[0].id, boardsStruct[0].ppm);
      Serial.println();
    }
    

    // 2번 Slave Data to Json
    if(boardsStruct[1].id == 2 && boardsStruct[1].tmp != 0)
    {
      JsonObject sensorObject2 = sensorsArray2.createNestedObject();
      sensorObject2["sensor_number"] = boardsStruct[1].id;
      sensorObject2["temperature"] = boardsStruct[1].tmp;
      sensorObject2["humidity"] = boardsStruct[1].hum;
      sensorObject2["co2"] = boardsStruct[1].ppm;
      //sensorObject2["kg"] = result - tare;

      Serial.printf("device_num value%d: %d \n",2, boardsStruct[1].id);
      Serial.printf("temp value %d: %f \n", boardsStruct[1].id, boardsStruct[1].tmp);
      Serial.printf("humi value %d: %f \n", boardsStruct[1].id, boardsStruct[1].hum);
      Serial.printf("co2 value %d: %f \n", boardsStruct[1].id, boardsStruct[1].ppm);
      Serial.println();
    }

    // 3번 Slave Data to Json
    if(boardsStruct[2].id == 3 && boardsStruct[2].tmp != 0)
    {
      JsonObject sensorObject3 = sensorsArray3.createNestedObject();
      sensorObject3["sensor_number"] = boardsStruct[2].id;
      sensorObject3["temperature"] = boardsStruct[2].tmp;
      sensorObject3["humidity"] = boardsStruct[2].hum;
      sensorObject3["co2"] = boardsStruct[2].ppm;
      //sensorObject3["kg"] = result - tare;

      Serial.printf("device_num value%d: %d \n",3 , boardsStruct[2].id);
      Serial.printf("temp value %d: %f \n", boardsStruct[2].id, boardsStruct[2].tmp);
      Serial.printf("humi value %d: %f \n", boardsStruct[2].id, boardsStruct[2].hum);
      Serial.printf("co2 value %d: %f \n", boardsStruct[2].id, boardsStruct[2].ppm);
      Serial.println();

      String jsonData;
    serializeJson(jsonDocument, jsonData);
    

    //WiFi.mode(WIFI_AP);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);

      Serial.print("Connecting to WiFi...");  
      Serial.print("Attempt_");
      connectingCount++;
      Serial.println(connectingCount);
      WiFi.begin(ssid, password);
      if(connectingCount >= 10){
        Serial.println("Reboot");
        connectingCount = 0;
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);   // Wi-Fi 모드 끄기
        WiFi.mode(WIFI_AP_STA);
        esp_deep_sleep_start();
      }
    }

    Serial.println("Connected to WiFi");
    connectingCount = 0;
    Serial.print("ConnectingCount Reset >> ");
    Serial.println(connectingCount);
    delay(1000);
    
    // HTTP 요청 생성
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    Serial.println("\n");
    Serial.println(jsonData);

    // 데이터 전송
    int httpResponseCode = http.POST(jsonData);
    
    
    if (httpResponseCode == 200) {
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      String response = http.getString();
      //Serial.println(response);
      //Serial.print("\t\t\t\t");
      //Serial.println(httpResponseCode);
    http.end(); 
    } 
    else {
      Serial.printf("Error code: %s\n", http.errorToString(httpResponseCode).c_str());
      WiFi.disconnect();
      esp_deep_sleep_start();
    }

    
  // Wi-Fi 연결 끊기
      WiFi.disconnect();
      Serial.println("Disconnected from WiFi");
      WiFi.mode(WIFI_OFF);   // Wi-Fi 모드 끄기
      WiFi.mode(WIFI_AP_STA);
    }

    delay(600000);
    //delay(3000);
  }
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
