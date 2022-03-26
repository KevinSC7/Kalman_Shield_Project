
bool APbegin(){
    byte timeOut = 0;
    Serial.println("Iniciando LittleFS");
    while(!LittleFS.begin()){
            Serial.print(".");
            delay(100);
        timeOut++;
        if(timeOut == 7)break;
        }
    if(timeOut == 7){
        Serial.println("ERROR: Sistema SPIFFS no funciona");
        return false;
    }else{
        Serial.println();
        Serial.println("Sistema FS iniciado");
    }
    File file = LittleFS.open("/DatosAP.txt", "r");
    if(!file){
        Serial.println("No existen datos (primera carga), creo carpeta /DatosAP.txt y guardo la SSID y psw por defecto");
        file = LittleFS.open("/DatosAP.txt", "w");
        file.print(ssid);
        file.print(" ");
        file.println(password);
    }else{
        Serial.println("Ya hay datos del AP en SPIFFS");
    }
    file.close();
    return true;
}

String getSSID(){
    String r="";
    File file = LittleFS.open("/DatosAP.txt", "r");             //Abro el fichero que supuestamente existe
    if(!file) return "";                                        //Sino existe o no se puede abrir -> empty
    if(file.available()){
        r=file.readStringUntil((char)13);                       //Primera linea contiene los valores guardados
    }
    r = buscarBloque(r, ' ', 1);                                //El primer bloque es el SSID
    file.close();
    return r;                                                   //Sino se pudo usar -> empty, sino el valor
}

String getPSW(){
    String r="";
    File file = LittleFS.open("/DatosAP.txt", "r");             //Abro el fichero que supuestamente existe
    if(!file) return "";                                        //Sino existe o no se puede abrir -> empty
    if(file.available()){
        r=file.readStringUntil((char)13);                       //Primera linea contiene los valores guardados
    }
    r = buscarBloque(r, ' ', 2);                                //El segundo bloque es el password
    file.close();
    return r;                                                   //Sino se pudo usar -> empty, sino el valor
}

bool setDatosAP(String nuevaSSID, String nuevaPSW){
    if(nuevaPSW.length() < 8 || nuevaPSW.length() > 20)return false;    //Req. rendimiento y Req. seguridad
    if(nuevaSSID.isEmpty() || nuevaSSID.length() > 20)return false;     //Req. rendimiento
    File file = LittleFS.open("/DatosAP.txt", "w+");
    if(!file)return false;
    file.print(nuevaSSID);
    file.print(" ");
    file.println(nuevaPSW);
    file.close();
    return true;
}
