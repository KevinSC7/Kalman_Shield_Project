#define RTIERRA_KM 6378.0F


bool codGpsCurrentData(){
    while (gpsSerial.available() > 0){
		gps.encode(gpsSerial.read());
        if(gpsSerial.overflow()){
            Serial.println("GPS OVERFLOW");
            gpsSerial.flush();
            return false;
        }
	}

    return true;
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
    Serial.println("No updated");
    return 0;
}

bool gpsShowAllCod(){   //Solo las importantes
    Serial.print("LAT=");       Serial.println(gps.location.lat(), 6);
    Serial.print("LONG=");      Serial.println(gps.location.lng(), 6);
    Serial.print("ALT_M=");     Serial.println(gps.altitude.meters());
    Serial.print("ALT_KM=");    Serial.println(gps.altitude.kilometers());
    Serial.print("ALT_CM=");    Serial.println(gps.altitude.value());
    Serial.print("DATE=");      Serial.println(gps.date.value());
    Serial.print("TIME=");      Serial.println(gps.time.value());
    Serial.print("NSAT=");      Serial.println(gps.satellites.value());
    Serial.print("SPD_KNOTS="); Serial.println(gps.speed.value());
    Serial.print("SPD_MS=");    Serial.println(gps.speed.mps());
    Serial.print("SPD_KMH=");   Serial.println(gps.speed.kmph());

    return true;
}

float degreesToRadians(float degrees) {
	return (degrees*PI)/180;
}
	
float radiansToDegrees(float rad) {
	return (rad*180)/PI;
}
//https://www.genbeta.com/desarrollo/como-calcular-la-distancia-entre-dos-puntos-geograficos-en-c-formula-de-haversine
//ONLINE calculo: https://www.tutiempo.net/calcular-distancias.html
double distancia(double lat0, double lon0, double decimalLatitud, double decimalLongitud){//Haversine
    double deltaLat = degreesToRadians(lat0 - decimalLatitud);
    double deltaLon = degreesToRadians(lon0 - decimalLongitud);
    double a = pow(sin(deltaLat/2), 2) + cos(degreesToRadians(lat0))*cos(degreesToRadians(decimalLatitud))*pow(sin(deltaLon/2), 2);
    double c = 2*atan2(sqrt(a), sqrt(1-a));
    return RTIERRA_KM * c;
}

//https://www.movable-type.co.uk/scripts/latlong.html
double getBearing(double lat0, double lon0, double decimalLatitud, double decimalLongitud){//Mi direccion origen-donde_estoy respecto a origen-norte_geografico
    double deltaLon = lon0 - decimalLongitud;
    double y = sin(deltaLon) * cos(decimalLatitud);
    double x = cos(lat0)*sin(decimalLatitud) - sin(lat0)*cos(decimalLatitud)*cos(deltaLon);
    double bering = atan2(y, x);
    return ((bering*180/PI + 360) / 360);
}
