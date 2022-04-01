
bool configBegin(){
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
    File file = LittleFS.open("/Configuraciones.txt", "r");
    if(!file){
        Serial.println("No existe archivo Configuraciones.txt, se crea");
        file = LittleFS.open("/Configuraciones.txt", "w");
    }else{
        Serial.println("Ya hay datos del AP en SPIFFS");
    }
    file.close();
    return true;
}

String getConfig(int nconf){//Formato configuraciones-> 010000-tiempo/101100-tiempo/.../\n
    if(nconf < 1)return "";
    String r="";
    File file = LittleFS.open("/Configuraciones.txt", "r");
    if(!file) return "";
    if(file.available()){
        r=file.readStringUntil((char)13);
    }
    r = buscarBloque(r, '/', nconf);//si nconf > que el numero de configuraciones->""
    file.close();
    return r;
}

bool addNewConf(String config){
    File file = LittleFS.open("/Configuraciones.txt", "r");
    if(!file) return false;
    String datos;
    if(file.available()){
        datos=file.readStringUntil((char)13);
    }
    file.close();
    for (int i=1; i<11;i++){
        if(buscarBloque(datos, '/', i)==""){
            datos+=config+"/";
            break;
        }
    }
    Serial.println("add: "+datos);//datos esta sin retorno de carro
    file = LittleFS.open("/Configuraciones.txt", "w+");
    if(!file)return false;
    file.println(datos);
    file.close();

    return true;
}

bool deleteConf(int nconf){
    if(nconf > 10)return false;
    File file = LittleFS.open("/Configuraciones.txt", "r");
    if(!file) return false;
    String datos;
    if(file.available()){
        datos=file.readStringUntil((char)13);
    }
    file.close();
    if(buscarBloque(datos, '/', nconf)=="")return true;
    String parte1=buscarBloque(datos, '/', nconf-1);//Si nconf==0 -> vacio
    String parte2=buscarBloque(datos, '/', nconf+1);//Si el ultimo no existe(11) devuelve vacio
    datos=parte1+parte2;    //El primero: vacio+parte2, el ultimo: parte1+vacio, ultimo=primero: vacio+vacio
    Serial.print("delete: "+datos);
    Serial.println(".");
    return true;
}

bool updateConfig(int nconf, String nueva){
    if(nconf > 10)return false;
    File file = LittleFS.open("/Configuraciones.txt", "r");
    if(!file) return false;
    String datos;
    if(file.available()){
        datos=file.readStringUntil((char)13);
    }
    file.close();
    datos=buscarBloque(datos, '/', nconf);
    if(datos.isEmpty())return false;
    Serial.println("update: "+datos);
    return true;
}

bool resetDatosConfiguraciones(){
    Serial.println("resetConfiguraciones");
    if(!configBegin())return false;
    LittleFS.remove("/Configuraciones.txt");
    if(!configBegin()) {
		Serial.println("Error: en SPIFFS al cargar datos de las configuraciones");
        return false;
	}
    Serial.println("OK");
    return true;
}