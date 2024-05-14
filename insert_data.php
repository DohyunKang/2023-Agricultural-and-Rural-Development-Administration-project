<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

$data = json_decode(file_get_contents('php://input'), true);

// 데이터베이스에 입력받은 데이터를 삽입하는 코드

// 수신된 데이터 사용 예시
$deviceName = $data['device_name'];

// 센서 데이터 배열에서 데이터 추출
$sensors = $data['sensors'];

foreach ($sensors as $sensor) {
    $sensorNumber = $sensor['sensor_number']; // 센서 번호 추가
    $temperature = round($sensor['temperature'], 1);
    $co2 = round($sensor['co2'], 1);
    $windVelocity = round($sensor['wind_velocity'], 1);

    // 데이터 출력
    echo "Device Name: " . $deviceName . "<br>";
    echo "Sensor Number: " . $sensorNumber . "<br>";
    echo "Temperature: " . $temperature . "<br>";
    echo "CO2: " . $co2 . "<br>";
    echo "Wind Velocity: " . $windVelocity . "<br>";

    // 데이터베이스에 저장 예시 (MySQL 사용)
    $servername = "localhost";
    $username = " ";
    $password = " ";
    $dbname = " ";

    // 데이터베이스 연결
    $conn = new mysqli($servername, $username, $password, $dbname);
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // 데이터 삽입 쿼리
    $sql = "INSERT INTO sensor_data (device_name, sensor_number, temperature, co2, wind_velocity)
            VALUES ('$deviceName', $sensorNumber, $temperature, $co2, $windVelocity)";

    if ($conn->query($sql) === TRUE) {
        echo "Data inserted successfully into the database.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    // 데이터베이스 연결 종료
    $conn->close();
}
?>
