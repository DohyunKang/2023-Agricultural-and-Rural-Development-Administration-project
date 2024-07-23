# INU Sensor Data Project

## 프로젝트 설명

이 프로젝트는 Arduino를 사용하여 벌집의 온도와 습도, 로드셀을 이용한 무게 변화를 측정하고, PHP와 MySQL 데이터베이스를 통해 실시간으로 1분마다 웹사이트에 측정값을 저장하는 시스템입니다. 

## 주요 기능

- **온도 및 습도 센서**: 벌집 내부의 온도와 습도를 측정합니다.
- **로드셀(무게 센서)**: 벌집의 무게 변화를 측정합니다.
- **실시간 데이터 저장**: 측정된 데이터를 PHP와 MySQL을 사용하여 1분마다 데이터베이스에 저장하고, 이를 웹사이트에서 확인할 수 있습니다.

## 설치 및 실행 방법

1. **Arduino 설정**:
    - Arduino에 온도, 습도, 로드셀 센서를 연결합니다.
    - 적절한 코드를 작성하여 센서 데이터를 읽고, 이를 시리얼 통신을 통해 전송합니다.

2. **서버 설정**:
    - PHP 및 MySQL이 설치된 웹 서버를 준비합니다.
    - 데이터베이스에 측정값을 저장할 테이블을 생성합니다.
    
    ```sql
    CREATE TABLE sensor_data (
        id INT AUTO_INCREMENT PRIMARY KEY,
        sensor_type VARCHAR(50),
        value FLOAT,
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
    ```

3. **웹사이트 설정**:
    - 서버에서 센서 데이터를 받아 데이터베이스에 저장하는 PHP 스크립트를 작성합니다.
    - 실시간으로 데이터베이스의 데이터를 조회하고 시각화하는 웹 페이지를 작성합니다.

## 웹사이트 주요 화면

### 메인 페이지
<img width="700" alt="2" src="https://private-user-images.githubusercontent.com/166014987/351328478-c51c6b14-0f60-4ca3-88c3-67f3b11befcd.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjE3Mzg5OTgsIm5iZiI6MTcyMTczODY5OCwicGF0aCI6Ii8xNjYwMTQ5ODcvMzUxMzI4NDc4LWM1MWM2YjE0LTBmNjAtNGNhMy04OGMzLTY3ZjNiMTFiZWZjZC5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwNzIzJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDcyM1QxMjQ0NThaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1mYWRlMjEzNjU3M2Q3MWU1MzhiMDAzNTRjMGNkNTJiNGY3NTY0MGMyZTVjODJjODk5YTFjZmQ4ODVlM2MxM2M1JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.W0UtX3UGiCJP-x6BlbDJCJEGGFeaj5WMom3hTAwQFsQ">      

### 센서 데이터 그래프


### 비 카운터


### 실시간 이미지


### 센서 데이터 테이블


## 기술 스택

- **하드웨어**: Arduino(esp32), 온도 및 습도 센서(dht33), co2 측정 센서(MQ135), 로드셀
- **백엔드**: PHP, MySQL
- **프론트엔드**: HTML, CSS, JavaScript
