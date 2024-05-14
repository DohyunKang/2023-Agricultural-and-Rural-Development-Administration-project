<?php
// 데이터베이스 연결 설정
$servername = "localhost";
$username = " ";
$password = " ";
$dbname = " ";

// 데이터베이스 연결
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// 데이터베이스에서 데이터 읽어오기
$sql = "SELECT * FROM sensor_data";
$result = $conn->query($sql);

$data = array();

if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        // 데이터 배열에 추가
        $deviceName = $row['device_name'];
        $sensorNumber = $row['sensor_number'];
        $temperature = round($row['temperature'], 1);
        $co2 = round($row['co2'], 1);
        $windVelocity = round($row['wind_velocity'],1 );
        $timestamp = $row['timestamp'];

        $data[$deviceName]["sensor$sensorNumber"] = array(
            "temperature" => round($temperature,1),
            "co2" => round($co2 ,1),
            "wind_velocity" => round($windVelocity, 1),
            "timestamp" => $timestamp
        );
    }
}

// 데이터를 JSON 형식으로 반환
header("Content-Type: application/json");
echo json_encode($data);


// 데이터베이스 연결 종료
$conn->close();
?>
