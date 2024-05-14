<?php
// 데이터베이스 연결 설정
$servername = "localhost"; // 데이터베이스 서버 주소
$username = " "; // 사용자 이름
$password = " "; // 비밀번호
$dbname = " "; // 데이터베이스 이름

// 데이터베이스 연결 생성
$conn = new mysqli($servername, $username, $password, $dbname);
// 연결 확인
if ($conn->connect_error) {
    die("데이터베이스 연결 실패: " . $conn->connect_error);
}

// 쿼리 생성
$sql = "SELECT device_name, sensor_number, temperature, humidity, timestamp FROM sensor_data WHERE device_name = 'DEVICE_1' AND sensor_number = 2 ORDER BY timestamp DESC";

// 쿼리 실행
$result = $conn->query($sql);

// 테이블 형태로 데이터 출력
if ($result->num_rows > 0) {
    echo "<table>";
    echo "<tr><th>Device Name</th><th>Sensor Number</th><th>Temperature</th><th>Humidity</th><th>Timestamp</th></tr>";
    // 결과 출력
    while ($row = $result->fetch_assoc()) {
        echo "<tr><td>".$row['device_name']."</td><td>".$row['sensor_number']."</td><td>".$row['temperature']."</td><td>".$row['humidity']."</td><td>".$row['timestamp']."</td></tr>";
    }
    echo "</table>";
} else {
    echo "해당하는 데이터가 없습니다.";
}

// 연결 종료
$conn->close();
?>
