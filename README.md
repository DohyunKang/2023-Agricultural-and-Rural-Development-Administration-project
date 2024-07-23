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
![메인 페이지](file-XgOzVJ9PibDOcXVHjJ9uTeEc)

### 센서 데이터 그래프
![센서 데이터 그래프](file-k3KDgL4vun0Svxnd01IuzNMT)

### 비 카운터
![비 카운터](file-oFH2Oit6J7TgJ89fAYpI8dSd)

### 실시간 이미지
![실시간 이미지](file-S45CM8tW6ppWBSlhb8aaPmw3)

### 센서 데이터 테이블
![센서 데이터 테이블](file-sthjz4sKEgXS97JK9xBu8Dt6)

## 기술 스택

- **하드웨어**: Arduino, 온도 및 습도 센서, 로드셀
- **백엔드**: PHP, MySQL
- **프론트엔드**: HTML, CSS, JavaScript

## 기여 방법

1. 이 저장소를 포크합니다.
2. 새로운 브랜치를 생성합니다. (`git checkout -b feature/새로운기능`)
3. 변경 사항을 커밋합니다. (`git commit -am 'Add 새로운기능'`)
4. 브랜치에 푸시합니다. (`git push origin feature/새로운기능`)
5. 풀 리퀘스트를 생성합니다.

## 라이선스

이 프로젝트는 MIT 라이선스 하에 배포됩니다. 자세한 내용은 `LICENSE` 파일을 참조하세요.
