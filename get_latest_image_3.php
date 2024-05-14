<?php
// 이미지가 업로드되는 폴더 경로
$imageFolderPath = dirname(__FILE__) . '/beeCounter_image_3/';

// 폴더 내의 모든 파일 목록을 가져옵니다.
$files = scandir($imageFolderPath, SCANDIR_SORT_DESCENDING);

// 파일 목록에서 디렉토리와 상위 디렉토리를 제거합니다.
$files = array_diff($files, array('.', '..'));

// 파일 이름들을 정렬하여 최신 이미지가 가장 먼저 오도록 합니다.
$files = array_values($files);

// CORS 헤더 설정
header('Access-Control-Allow-Origin: *'); // 모든 도메인으로부터 접근 허용
header('Content-Type: application/json');

// 최신 이미지 파일 이름들을 JSON 형식으로 반환합니다.
header('Content-Type: application/json');
echo json_encode($files);
?>
