
/**
 * Pressure data read and set global variable
 */
void readBME() {
  String message = "";
  float p = bme.readPressure() / 100;
  //  int t = round(bme.readTemperature());
  if ((p < 1200) && (p > 800)) {
    stationPressure = p;
  }
}


/**
 * DHT data read and set global variable
 */
void readDHT() {
  String message = "";
  int h = round(dht.readHumidity());
  int t = round(dht.readTemperature());

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    int hic = round(dht.computeHeatIndex(t, h, false));     
    if ((t>-50) && (t<60)) {
      stationTemp = t;
    }
    if ((hic>-50) && (hic<60)) {
      stationHeatIndex = hic;
    }
    if ((h>0) && (h<150)) {
      stationHumidity = h;   
    }
  }
}


/**
 * TX20 data reading
 * @return bool success data reading
 */
boolean readTX20() {
    if (!TX20IncomingData) {
      return false;
    }  
    int bitcount=0;    
    sa=sb=sd=se=0;
    sc=0;sf=0;
    tx20RawDataS = "";
    
    for (bitcount=41; bitcount>0; bitcount--) {
      pin = (digitalRead(TX20_DATAPIN));
      if (!pin) {
        tx20RawDataS += "1";      
      } else {
        tx20RawDataS += "0";      
      }
      if ((bitcount==41-4) || (bitcount==41-8) || (bitcount==41-20)  || (bitcount==41-24)  || (bitcount==41-28)) {
        tx20RawDataS += " ";
      }      
      if (bitcount > 41-5){
        // start, inverted
        sa = (sa<<1)|(pin^1);
      } else
      if (bitcount > 41-5-4){
        // wind dir, inverted
        sb = sb>>1 | ((pin^1)<<3);
      } else
      if (bitcount > 41-5-4-12){
        // windspeed, inverted
        sc = sc>>1 | ((pin^1)<<11);
      } else
      if (bitcount > 41-5-4-12-4){
        // checksum, inverted
        sd = sd>>1 | ((pin^1)<<3);
      } else 
      if (bitcount > 41-5-4-12-4-4){
        // wind dir
        se = se>>1 | (pin<<3);
      } else {
        // windspeed
        sf = sf>>1 | (pin<<11);
      } 
          
      delayMicroseconds(1220);    
    }
    chk= ( sb + (sc&0xf) + ((sc>>4)&0xf) + ((sc>>8)&0xf) );chk&=0xf;
    delayMicroseconds(2000);  // just in case
    TX20IncomingData = false;  

    if (sa==4 && sb==se && sc==sf && sd==chk){     
      tx20Direction[sb] ++;
      tx20SpeedIndex ++;
      tx20Speed += sc;
      if (sc > tx20Gust) {
        tx20Gust = sc;
      }
      return true;
    } else {
      return false;
    }
}

/**
 * Reset global temp wind parameters
 * Using after store calculated data into the station variables
 */
void resetTX20() {
  for (int f=0; f<16; f++) {
    tx20Direction[f] = 0;    
  }
  tx20Speed = 0;
  tx20Gust = 0;
  tx20SpeedIndex = 0;
}

/**
 * Calculate the most specific wind data from last period
 */
void calculateWind() {     
  if (tx20SpeedIndex <= 0) {
    return;    
  }  
  // direction
  int specific = 0;
  for (int f=0; f<16; f++) {
    if (tx20Direction[f]>=specific) {
      specific = tx20Direction[f];
      stationWindDirection = f;    
      stationWindDirectionName = windDirectionNameEn[stationWindDirection];
    }
  }

  // speed 
  stationWindSpeed = (int) tx20Speed / tx20SpeedIndex;
  stationWindGust = tx20Gust;
  
  TX20ValidData = true;
}

