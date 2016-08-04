SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";


CREATE TABLE iot_weather_raw (
  `id` int(11) NOT NULL,
  `time` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Stored time',
  `millis` bigint(20) DEFAULT NULL COMMENT 'MC running ms',
  `wind_direction` tinyint(4) DEFAULT NULL COMMENT 'Wind direction number',
  `wind_direction_name` varchar(16) DEFAULT NULL COMMENT 'Wind direction string',
  `wind_speed` int(11) DEFAULT NULL COMMENT 'Wind speed km/h',
  `humidity` int(11) DEFAULT NULL COMMENT 'Humidity %',
  `temperature` int(11) DEFAULT NULL COMMENT 'Temp',
  `heat_index` int(11) DEFAULT NULL COMMENT 'Heat index',
  `pressure` decimal(10,2) DEFAULT NULL COMMENT 'Pressure mPa'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Wemos D1 WS';


ALTER TABLE iot_weather_raw
  ADD PRIMARY KEY (`id`),
  ADD KEY `time` (`time`);


ALTER TABLE iot_weather_raw
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
