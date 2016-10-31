/*
 * Weather information in openweathermap compatible json format
 * with some extra
 * @link http://openweathermap.org/stations
 */
String jsonOutput() {
  String j = "";

  j+="{\n";

  j += "\"millis\":";
  j += stationMillis;
  j+=",\n"; 
  j += "\"stationId\":\"";
  j += stationID;
  j+="\",\n"; 
  j += "\"windDirection\":";
  j += stationWindDirection;
  j+=",\n"; 
  j += "\"windDirectionName\":\"";
  j += stationWindDirectionName;
  j+="\",\n";  
  j += "\"windSpeed\":";
  j += stationWindSpeed;
  j+=",\n";  
  j += "\"windGust\":";
  j += stationWindGust;
  j+=",\n";   
  j += "\"humidity\":";
  j += stationHumidity;
  j+=",\n";
  j += "\"temperature\":";
  j += stationTemp;
  j+=",\n";  
  j += "\"heatIndex\":";
  j += stationHeatIndex;
  j+=",\n";
  j += "\"pressure\":";
  j += stationPressure;
  j+=",\n";
  j += "\"dewpoint\":";
  j += dewPoint(stationTemp, stationHumidity);
  j+="\n";  
  j+="}\n";
  return j;
}

/*
 * Text output
 */
String textOutput() {
  String message = "";
  if (TX20ValidData) {
    message += "Wind: ";
    message += stationWindDirectionName;
    message += " %\t ";
    message += stationWindSpeed;
    message += " m/s %\t";
    message += " Wind gust:";
    message += stationWindGust;
    message += " m/s\n";
  }
  message += "Humidity: ";
  message += stationHumidity;
  message += " %\t";
  message += "Temperature: ";
  message += stationTemp;
  message += " C\t";
  message += "Heat index: ";
  message += stationHeatIndex;
  message += " C\n";    

  message += "Pressure: ";
  message += stationPressure;
  message += " hPa\n";

  return message;
}

/*
 * WEBSERVER page not found
 */
void handleNotFound(){
  digitalWrite(LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED, 0);
}

/*
 * Send json data with POST method to my webserver
*/ 
void sendRawAjax() {
  HTTPClient http;
  http.begin( privateAjaxUrl );
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST("json=" + urlencode(jsonOutput()));
  http.end();  
}

/*
 * Url Encode for sending post/get to webserver
 * @param string original string
 * @return string encoded string
*/
String urlencode(String str) {
  String encodedString="";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i =0; i < str.length(); i++){
    c=str.charAt(i);
    if (c == ' '){
      encodedString+= '+';
    } else if (isalnum(c)){
      encodedString+=c;
    } else{
      code1=(c & 0xf)+'0';
      if ((c & 0xf) >9){
          code1=(c & 0xf) - 10 + 'A';
      }
      c=(c>>4)&0xf;
      code0=c+'0';
      if (c > 9){
          code0=c - 10 + 'A';
      }
      code2='\0';
      encodedString+='%';
      encodedString+=code0;
      encodedString+=code1;
      //encodedString+=code2;
    }
    yield();
  }
  return encodedString;   
}

/*
 * OpenWeatherMap data service
 * Send data with authentication over POST
 * 
 * @link http://openweathermap.org/stations
 */
void sendOpenWeatherMap() {
  if (owm_auth_user == "") {
    return;
  }
  
  Serial.println("Open Weather map data:");
  String postData = "";
  postData += "temp=";
  postData += stationTemp;
  postData += "&humidity=";
  postData += stationHumidity;
  postData += "&pressure=";
  postData += stationPressure;
  if (TX20ValidData==true) { 
    postData += "&wind_dir=";
    postData += (int) stationWindDirection * 22.5;
    postData += "&wind_speed=";
    postData += stationWindSpeed;
    postData += "&wind_gust=";
    postData += stationWindGust;
  }  
  postData += "&lat=" + owm_stationLat;
  postData += "&long=" + owm_stationLong;
  postData += "&alt=" + owm_stationAlt;
  postData += "&name=" + urlencode(owm_stationName);
  Serial.println(postData);
  HTTPClient http;
  http.begin(owm_url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setAuthorization(owm_auth_user, owm_auth_pass);
  int httpCode = http.POST( postData );
  Serial.print("code: ");
  Serial.println(httpCode);
  http.end();
}

/*
 * Weather Underground data service
 * Send data with authentication over POST
 * 
 * @link http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol
 */
void sendWunderground() {
  if (wg_station_id == "") {
    return;
  }
  Serial.println("Weather Underground data:");
  String postData = "";
  postData += "action=updateraw";
  postData += "&ID="+wg_station_id;
  postData += "&PASSWORD="+wg_station_key;
  postData += "&dateutc=now";
  if (TX20ValidData==true) {
    postData += "&winddir=";
    postData += (int) stationWindDirection * 22.5;
    postData += "&windspeedmph=";
    postData += stationWindSpeed * 2.2369362920544;
    postData += "&windgustmph=";
    postData += stationWindGust * 2.2369362920544;     
  }
  postData += "&humidity=";
  postData += stationHumidity;
  postData += "&dewptf=";
  postData += dht.convertCtoF(dewPoint(stationTemp, stationHumidity));  
  postData += "&tempf=";
  postData += dht.convertCtoF(stationTemp);
  postData += "&baromin=";
  postData += stationPressure * 0.02953;
  Serial.println(postData); 

  HTTPClient http;
  http.begin(wg_url + postData);
  int httpCode = http.GET();
  String content = http.getString();
  Serial.print("code: ");
  Serial.println(httpCode);
  Serial.println(content);
  http.end();
}
