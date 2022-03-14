struct usuario{
  String user;
  String nombreApellidos;
  String psw;
};

usuario *inicial;
usuario *indice;
usuario *busqueda;

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
    inicial=new usuario();
		inicial->user="root";
		inicial->nombreApellidos="root";
		inicial->psw="root";
    file.println(inicial->user+" "+inicial->nombreApellidos+" "+inicial->psw);
  }
  file.close();
  return true;
}

String buscarBloque(String cadena, char delimitador, int numeroBloque){
  int i = 0, s, n = 0;                                  //Indice(i), siguiente(s) y bloque obtenido(n)
  String r = "";                                        //Valor a retornar
  if(numeroBloque < 1 || cadena.isEmpty())return "";    
  while(true){
    n++;
    s = cadena.indexOf(delimitador, i);                 //Desde i busca el delimitador
    if(s < 0){                                          //No hay mas bloques por buscar, return
      if(n == numeroBloque) return cadena.substring(i, cadena.length()); //El bloque ultimo coincide con el pedido, return bloque
      return "";                                        //El numero de bloque que pidio no existe (ej: hay 3 bloques y pides el 4), return vacio
    }
    r = cadena.substring(i, s);                         //Valor del bloque en el que estoy
    if(n == numeroBloque)return r;                      //Si coincide con el bloque devuelve el valor
    i = s+1;
  }
}

usuario* getUser(String nombreUsuario){
  usuario *u = NULL;
  String r = "";
  File file = LittleFS.open("/MisUsuarios.txt", "r"); //Abro el fichero que supuestamente existe
  if(!file) return u;                                //Sino existe el fichero
  String line="";
  while(file.available()) {                           //Mientras se pueda usar
    line = file.readStringUntil((char)13);            //Leer de fila en fila
    Serial.println("Datos linea: "+line);
    r = buscarBloque(line, ' ', 1);                   //El primer bloque es el del nombre de usuario
    if(r.equals(nombreUsuario)) break;                //Si coincide sale conservando el valor
    r = "";
  }
  file.close();                                       //Encontrado o no cerrar el fichero
  if(r.isEmpty())return u;                           //Sino existe retornará un struct NULL
  u->user = r;
  u->nombreApellidos = buscarBloque(line, ' ', 2);    //El bloque 2 son los nombres y apellidos (separados por comas)
  u->psw = buscarBloque(line, ' ', 3);                //El bloque ultimo (3) contiene la contraseña
  Serial.println("NombreApellidos: "+u->nombreApellidos);
  Serial.println("psw: "+u->psw);
  return u;                                          //Si existe el usuario lo retorna
}

