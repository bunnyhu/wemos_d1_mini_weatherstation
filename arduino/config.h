// wifi setup every row : SSID, Password
const char* wifi_ssid[][2] = { 
  {"ssid","password"}, 
  {"other_ssid","password"} 
};

// How many row in wifi_ssid array
const int wifi_ssid_max = 2;

// OpenWeather private settings 
const String owm_stationName = "Your weather station";
const String owm_stationLat = "0.0";
const String owm_stationLong = "0.0";
const String owm_stationAlt = "0";
const char* owm_auth_user = "ow_user";
const char* owm_auth_pass = "password";

// Wunderground private settings
const String wg_url = "http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php?";
const String wg_station_id = "YOUR_STATION_ID";
const String wg_station_key = "station_key";

const String privateAjaxUrl = "http://yourwebsite/wemosdatagate.php";
const String stationID = "any_id_text"; 