<?php
const HTML_EOL = '</br>';
echo '<!DOCTYPE html>
<html>
<head>
</head>
<body>
';
try {
    $pdo = new PDO('mysql:host=localhost;dbname=your_db_name', 'user', 'password');
    $sql = 'SHOW FULL COLUMNS FROM iot_weather_raw';
    $st = $pdo->query($sql);
    echo '<table border=1>';
    $fields = $st->fetchAll(PDO::FETCH_ASSOC);
    echo '<tr>';
    foreach ($fields as $field) {
        if (empty($field['Comment'])) {
            echo '<td>'.$field['Field'].'</td>';
        } else {
            echo '<td>'.$field['Comment'].'</td>';
        }
    }
    echo '</tr>';        
    $sql = 'SELECT * FROM iot_weather_raw';
    $st = $pdo->query($sql);
    while ($row = $st->fetch(PDO::FETCH_BOTH)) {
        echo '<tr>';
        foreach ($fields as $field) {
            echo '<td>'.$row[$field['Field']].'</td>';
        }
        echo '</tr>';
    }
    echo '</table>';
} catch (PDOException $e) {
    echo 'Connection failed: ' . $e->getMessage();
}
echo '
</body>
</html>
';
