String datosGps;

String getGpsRawData(){
    String frame="";
    while (gpsSerial.available())
	{
		char data;
		data = gpsSerial.read();
        frame += data;
        if(gpsSerial.overflow()){
            gpsSerial.flush();
            return "";
        }
	}
    return frame;
}

bool codGpsCurrentData(){
    if(datosGps.isEmpty())datosGps=getGpsRawData();
    bool ok = true;
    for(unsigned int i = 0; i < datosGps.length(); i++){
        if(!gps.encode(datosGps[i])){
            ok = false;
            break;
        }
    }
    datosGps="";
    if(ok)return true;
    return false;
}

double getLatitude(){
    if(!codGpsCurrentData())return 0;
    if (gps.location.isUpdated() && gps.location.isValid())return gps.location.lat();
    else return 0;
}
double getLongitude(){
    if(!codGpsCurrentData())return 0;
    if (gps.location.isUpdated())return gps.location.lng();
    else return 0;
}
double getAltitudeMeters(){
    if(!codGpsCurrentData())return 0;
    if (gps.altitude.isUpdated())return gps.altitude.meters();
    else return 0;
}
double getAltitudeKM(){
    if(!codGpsCurrentData())return 0;
    if (gps.altitude.isUpdated())return gps.altitude.kilometers();
    else return 0;
}
int32_t getAltitudeCM(){
    if(!codGpsCurrentData())return 0;
    if (gps.altitude.isUpdated())return gps.altitude.value();
    else return 0;
}
uint32_t getDate(){
    if(!codGpsCurrentData())return 0;
    if (gps.date.isUpdated())return gps.date.value();
    else return 0;
}
uint16_t getYear(){
    if(!codGpsCurrentData())return 0;
    if (gps.date.isUpdated())return gps.date.year();
    else return 0;
}
uint8_t getMonth(){
    if(!codGpsCurrentData())return 0;
    if (gps.date.isUpdated())return gps.date.month();
    else return 0;
}
uint8_t getDay(){
    if(!codGpsCurrentData())return 0;
    if (gps.date.isUpdated())return gps.date.day();
    else return 0;
}
uint32_t getTime(){
    if(!codGpsCurrentData())return 0;
    if (gps.time.isUpdated())return gps.time.value();
    else return 0;
}
uint8_t getHours(){
    if(!codGpsCurrentData())return 0;
    if (gps.time.isUpdated())return gps.time.hour();
    else return 0;
}
uint8_t getMinutes(){
    if(!codGpsCurrentData())return 0;
    if (gps.time.isUpdated())return gps.time.minute();
    else return 0;
}
uint8_t getSeconds(){
    if(!codGpsCurrentData())return 0;
    if (gps.time.isUpdated())return gps.time.second();
    else return 0;
}
uint32_t getSat(){
    if(!codGpsCurrentData())return 0;
    if (gps.satellites.isUpdated())return gps.satellites.value();
    else return 0;
}
uint32_t getSpeed(){
    if(!codGpsCurrentData())return 0;
    if (gps.speed.isUpdated())return gps.speed.value();
    else return 0;
}
double getMetersPerSecond(){
    if(!codGpsCurrentData())return 0;
    if (gps.speed.isUpdated())return gps.speed.mps();
    else return 0;
}
double getKmH(){
    if(!codGpsCurrentData())return 0;
    if (gps.speed.isUpdated())return gps.speed.kmph();
    else return 0;
}
double getMagVarZone(){
    if(!codGpsCurrentData())return 0;
    if (magneticVariation.isUpdated()){
        String s = magneticVariation.value();
        return s.toDouble();
    }
    else return 0;
}

byte isMagneticVarNegative(){
    if(!codGpsCurrentData())return 0;//Si hay error
    if (magneticVariationNegative.isUpdated()){
        String s = magneticVariationNegative.value();
        if(s == "W")return 1;
        if(s == "E")return 2;
    }
    return 0;
}