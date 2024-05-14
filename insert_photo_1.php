<?php

date_default_timezone_set('Asia/Seoul');

$str = file_get_contents('php://input');

$upload_path = __DIR__ . '/beeCounter_image_1/'; // 폴더 경로 지정

$latest_path = $upload_path . 'bee.jpg';

file_put_contents($latest_path, $str);

?>
