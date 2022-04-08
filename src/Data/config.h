
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
    if(datos.isEmpty())return false;            //Datos vacio, nada que borrar
    String r = borrarBloque(datos, '/', nconf);
    if(datos == r)return false;                 //No ha cambiado las confs: aa/bb/ y bloque 3 -> aa/bb/
    file = LittleFS.open("/Configuraciones.txt", "w+");
    if(!file)return false;
    file.println(r);
    file.close();
    
    return true;
}

bool updateConfig(int nconf, String nueva){
    if(nconf > 10)return false;
    if(nueva.length() < 1) return false;                    //Al menos un caracter (aunque el formato sea erroneo)
    File file = LittleFS.open("/Configuraciones.txt", "r");
    if(!file) return false;
    String datos;
    if(file.available()){
        datos=file.readStringUntil((char)13);
    }
    file.close();
    if(datos.isEmpty())return false;                        //Nada que actualizar
    if(nueva == buscarBloque(datos, '/', nconf))return true;//Actualiza pero la conf es la misma
    String r = actualizarBloque(datos, '/', nconf, nueva);
    if(datos == r)return false;  //Vacio o no ha cambiado aunque nueva != conf: aa/bb/ , nueva:"cc" y bloque 3 -> aa/bb/
    file = LittleFS.open("/Configuraciones.txt", "w+");
    if(!file)return false;
    file.println(r);
    file.close();

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