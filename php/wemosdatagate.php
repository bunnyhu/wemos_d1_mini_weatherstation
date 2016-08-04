<?php
/**
 * Wemos D1 Weather Station
 * @version 1.0
 *
 * iot_weather_raw
 * 
 * |id|int(11)|Nem|
 * |time|timestamp|Igen|NULL
 * |millis|bigint(20)|Igen|NULL
 * |wind_direction|tinyint(4)|Igen|NULL
 * |wind_direction_name|varchar(16)|Igen|NULL
 * |wind_speed|int(11)|Igen|NULL
 * |humidity|int(11)|Igen|NULL
 * |temperature|int(11)|Igen|NULL
 * |heat_index|int(11)|Igen|NULL
 * |pressure|decimal(10,2)|Igen|NULL 
 */

$windDirectionNameEn = array("N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW","");

if (isset($_POST['json'])) {   
    $raw = $_POST['json'];
    $json = json_decode($raw, true);
    if (json_last_error() == JSON_ERROR_NONE) {
        try {
		    $pdo = new PDO('mysql:host=localhost;dbname=your_db_name', 'user', 'password');
            $sql = 'INSERT INTO iot_weather_raw '
                  .'(millis, wind_direction, wind_direction_name, wind_speed, humidity, temperature, heat_index, pressure) '
                  .'VALUES (:millis, :wind_direction, :wind_direction_name, :wind_speed, :humidity, :temperature, :heat_index, :pressure)';
            $st = $pdo->prepare($sql);
            if (
                is_numeric($json['millis']) && 
                is_numeric($json['windDirection']) && 
                is_numeric($json['windSpeed']) && 
                is_numeric($json['humidity']) && 
                is_numeric($json['temperature']) && 
                is_numeric($json['heatIndex']) && 
                is_numeric($json['pressure']) && 
                (array_search($json['windDirectionName'], $windDirectionNameEn, true)!==false)   
            ) {
                $st->bindParam(':millis', $json['millis']);
                $st->bindParam(':wind_direction', $json['windDirection']);
                $st->bindParam(':wind_direction_name', $json['windDirectionName']);
                $st->bindParam(':wind_speed', $json['windSpeed']);
                $st->bindParam(':humidity', $json['humidity']);
                $st->bindParam(':temperature', $json['temperature']);
                $st->bindParam(':heat_index', $json['heatIndex']);
                $st->bindParam(':pressure', $json['pressure']);
                if ($st->execute()) {
                    echo 'OK';
                } else {
                    echo 'ERROR';            
                }                
            } else {
                $st->bindParam(':wind_direction_name', "ERROR");
                $st->bindParam(':millis', NULL);
                $st->bindParam(':wind_direction', NULL);
                $st->bindParam(':wind_speed', NULL);
                $st->bindParam(':humidity', NULL);
                $st->bindParam(':temperature', NULL);
                $st->bindParam(':heat_index', NULL);
                $st->bindParam(':pressure', NULL);
                $st->execute();
                echo 'ERROR';                            
            }
        } catch (PDOException $e) {
            echo 'Connection failed: ' . $e->getMessage();
        }
    }
} else {
    echo 'ERROR';
}