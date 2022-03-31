struct usuario{
  String user;
  String nombreApellidos;
  String psw;
};

bool usuariosBegin(){
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
	File file = LittleFS.open("/MisUsuarios.txt", "r");
  if(!file){
    Serial.println("No existe 'root', creo carpeta /MisUsuarios.txt y guardo a 'root'");
    file = LittleFS.open("/MisUsuarios.txt", "w");
    usuario *u=new usuario();
		u->user="root";
		u->nombreApellidos="root";
		u->psw="root";
    file.println(u->user+" "+u->nombreApellidos+" "+u->psw);
  }else{
    Serial.println("Ya hay datos del usuario en SPIFFS");
  }
  file.close();
  return true;
}

usuario* getUser(){ //Debe existir como minimo un usuario o no existe el fichero
  usuario *u = new usuario();           //Struct local
  String line="";
  File file = LittleFS.open("/MisUsuarios.txt", "r"); //Abro el fichero que supuestamente existe
  if(!file)return u;                                  //No existe el fichero (ni usuarios)
  if(file.available()){                               //Sigue accesible
    line = file.readStringUntil((char)13);            //Leo la unica fila que hay
  }
  file.close();
  if(line.isEmpty())return u;                         //Sino hay nada pero existe el file -> return NULL
  //Siempre deberia de existir un usuario si existe el file, es solo por seguridad
  u->user = buscarBloque(line, ' ', 1);               //Bloque 1: nombre del usuario
  u->nombreApellidos = buscarBloque(line, ' ', 2);    //Bloque 2: los nombres y apellidos (separados por comas)
  u->psw = buscarBloque(line, ' ', 3);                //Bloque 3(ultimo): contiene la contraseña
  return u;                                           //Existe el usuario lo retorna
}

bool setDatosUsuario(String _user, String _NyA, String _psw){
  if(_user.isEmpty() || _psw.length()<8 || _user.length()>20 || _psw.length()>20 || _NyA.length()>20) return false;
  File file = LittleFS.open("/MisUsuarios.txt", "w+");
  if(!file)return false;
  file.println(_user+" "+_NyA+" "+_psw);
  file.close();
  return true;
}

