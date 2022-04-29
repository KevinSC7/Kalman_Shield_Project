//INSTALACION IDE: https://www.luisllamas.es/como-programar-arduino-con-visual-studio-code-y-plaftormio/
//SPIFFS: https://esp8266-arduino-spanish.readthedocs.io/es/latest/filesystem.html#esquema-de-la-memoria-flash


#include <Arduino.h>
#include <ESP8266WiFi.h>		//Para configuración SSID, AP y uso de otras funciones
#include <ESPAsyncTCP.h>		//Necesario para realizar TCP asicronico
#include <ESPAsyncWebServer.h>	//Permite operar con peticiones web asincronicas
#include <LittleFS.h>			//Para operar con la memoria SPIFFS del ESP826612
#include <SoftwareSerial.h>		//Para usar dos puertos serie
#include <Wire.h>				//Para conexión I2C con el IMU

#define NO_ERROR 0
#define ERROR_PSW 1
#define ERROR_USER 2
#define ERROR_RETYPE_SSID 3
#define ERROR_RETYPE_USER 4
#define ERROR_DATA_AP 5
#define ERROR_DATA_USER 6
#define ERROR_CONF_ADD 7
#define ERROR_NO_SELECT_CHECKBOX 8
#define ERROR_NO_UPDATE 9
#define ERROR_NO_PRINTABLE 10
#define NO_RUN 0
#define RUN 1
#define MPU6050_adress 0x68

AsyncWebServer server(80);				//Puerto 80
SoftwareSerial gpsSerial(13, 15); // RX(D7), TX(D8) -> UART para GPS (GPIO_OK_Serial.PNG)

const char* ssid = "KalmanShield";		//Nombre baliza AP por defecto (RF1)
const char* password = "KalmanShield";	//8 caracteres (Req. seguridad) y por defecto esa contraseña (RF1)
String currentSSID = ssid;
String currentPSW = password;
bool autorizacion = false;				//Logeo correcto = true;
bool actualConf[5]={0,0,0,0,0};			//5 datos: async/sinc, ib, gb, ip, fus
bool ejecutando;						//RUN: ejecuta en main lo de actualConf, NO_RUN=STOP
long tiempo_ms;							//Ultimo dato de la configuracion actual, tasa de envio
int idConf;								//Indice de la actual configuracion
byte tipoErrorGestion;					//Tipo de error al cargar gestion
byte tipoErrorApGestion;				//Tipo de error al cargar gestion del AP
byte tipoErrorUserGestion;				//Tipo de error al cargar gestion del Usuario
byte tipoErrorLogin;					//Tipo de error al cargar login
String datos;							//Usos varios, datos globales

//GPS datos
String datosGps;

//MPU datos
double gx, gy, gz, giro_x, giro_y, giro_z;
double ax, ay, az, temperature;
double accel_x, accel_y, accel_z;
double angulo_pitch, angulo_roll, angulo_yaw;

//Sincronismo variables
unsigned long loop_timer, tiempo_ejecucion;

//Variables para interprete de comandos
String comandoRecibido;
String parametros;
int ins;

//ESTO ES CODIGO SEPARADO, TIENE QUE ESTAR ANTES DE LAS VARIABLES GLOBALES
#include "Data/herramientasParser.h" 	//Herramientas propias que ayudan a parsear Strings
#include "Data/usuarios.h"				//Control de datos del usuario sobre SPIFFS
#include "Data/SSID.h"					//Control de datos del AP sobre SPIFFS, contiene tambien IPAddress
#include "Data/config.h"				//Control de datos de las configuraciones
#include "Control/MPU6050_funciones.h"	//Funciones del sensor inercial MPU6050
#include "Control/Interprete.h"
//#include "Control/Simpson.h"			//Codigo de la regla de Simpson 1/3


#include "InterfacesArduino/login.h"
#include "InterfacesArduino/getGestion.h"
#include "InterfacesArduino/getApGestion.h"
#include "InterfacesArduino/getUserGestion.h"

#include "InterfacesArduino/confirm_AP_USER.h"
#include "InterfacesArduino/datosConfiguraciones.h"

void notFound(AsyncWebServerRequest *request) {	//Para URL no encontrada
    request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);							//Baudios del puerto serie el monitor
	gpsSerial.begin(9600);							//Baudios del puerto serie del GPS
	actualConf[0]=actualConf[1]=actualConf[2]=actualConf[3]=actualConf[4] = 0;
	datosGps = "";
	idConf = 0;										//No hay configuracion cargada
	tipoErrorLogin = NO_ERROR;						//Acceso a login sin errores
	tipoErrorGestion = NO_ERROR;					//Acceso a gestion sin errores
	tipoErrorApGestion = NO_ERROR;					//Acceso a Ap gestion sin errores
	tipoErrorUserGestion = NO_ERROR;				//Acceso a user gestion sin errores
	ejecutando = NO_RUN;							//No esta ejecutandose la configuracion
	tiempo_ms = 0L;									//Tasa de envio de datos 0ms

	//PREPARAR DATOS DEL AP
	if(!APbegin()) {								//Preparar carga SPIFFS seccion DatosAP.txt
		Serial.println("Error: en SPIFFS al cargar datos del AP");
	}else{
		currentSSID=getSSID(); currentPSW=getPSW();	//Si hay datos los carga, sino usará los por defecto
		Serial.println("Actual SSID="+currentSSID+" Actual psw="+currentPSW);
	}
	//CONFIGURAR Y LANZAR EL AP
    WiFi.mode(WIFI_AP);								//Establecer modo AP
	WiFi.softAPConfig(local_IP, gateway, subnet);	//Configuración (Poner antes de encender el AP, sino no funciona)
	WiFi.softAP(currentSSID, currentPSW, 1, 0, 1);	//Lanzamiento de la baliza AP (ssid, psw, ch, hidden, max_clients)
													//Si despues de enceder la baliza se configura no conecta
	Serial.println("AP configurado y lanzado");
	//PREPARAR DATOS
	
	if(!usuariosBegin()) {
		Serial.println("Error: en SPIFFS al cargar datos del usuario");
	}else{
		miUsuario=getUser();
		if(miUsuario){
			Serial.println(miUsuario->user);
			Serial.println(miUsuario->nombreApellidos);
			Serial.println(miUsuario->psw);
		}else{
			Serial.println("Error: Fallo en getUser");
		}
	}

	if(!configBegin()){
		Serial.println("Error: en SPIFFS al cargar datos de las configuraciones");
	}

	//FIN PREPARAR DATOS
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){	//Mi http:IP/
        request->redirect("/getLogin");
    });

    server.on("/getLogin", HTTP_GET, [] (AsyncWebServerRequest *request) {		//http:IP/getLogin
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		response->print(getLogin(tipoErrorLogin));	//Llamada a la intefaz login, inicialmente sin errores
        request->send(response);
    });

    server.on("/postLogin", HTTP_POST, [](AsyncWebServerRequest *request){		//http:IP/postLogin
		String nombreUsuario = request->arg("user");
		String pswUsuario = request->arg("psw");
		if(nombreUsuario == miUsuario->user){
			if(pswUsuario == miUsuario->psw){
				autorizacion = true;
				tipoErrorLogin = NO_ERROR;			//Usuario y contraseña correcta
				tipoErrorGestion = NO_ERROR;		//Entrada a gestion sin errores
				request->redirect("/getGestion");
			}else{
				tipoErrorLogin = ERROR_PSW;			//Usuario correcto, contraseña incorrecta
				request->redirect("/getLogin");
			}
		}else{
			tipoErrorLogin = ERROR_USER;			//El nombre de usuario no es correcto
			request->redirect("/getLogin");
		}
    });

	server.on("/logout", HTTP_POST, [](AsyncWebServerRequest *request){			//http:IP/logout
		autorizacion=false;
		request->redirect("/getLogin");
    });

	server.on("/getGestion", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getGestion
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(autorizacion){
			tipoErrorApGestion = NO_ERROR;	//Si cambio de pagina los errores de las otras desaparecen
			tipoErrorUserGestion = NO_ERROR;//Si cambio de pagina los errores de las otras desaparecen
			response->print(getGestion());
			request->send(response);
		}else{
			request->redirect("/getLogin");
		}
    });

	server.on("/getUserGestion", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getUserGestion
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(autorizacion){
			tipoErrorGestion = NO_ERROR;		//Si cambio de pagina los errores de las otras desaparecen
			tipoErrorApGestion = NO_ERROR;		//Se suele pasar por gestion, pero si se accede por url el error deberia de haber desaparecido
			response->print(getUserGestion());
			request->send(response);
		}else{
			request->redirect("/getLogin");
		}
    });

	server.on("/getApGestion", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getApGestion
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(autorizacion){
			tipoErrorGestion = NO_ERROR;
			tipoErrorUserGestion = NO_ERROR;
			response->print(getApGestion());
			request->send(response);
		}else{
			request->redirect("/getLogin");
		}
    });

	server.on("/postSSID", HTTP_POST, [](AsyncWebServerRequest *request){		//http:IP/postSSID
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			String d1 = request->arg("SSID");
			String d2 = request->arg("psw");
			String d3 = request->arg("retype");
			if(isValidString(d1) && isValidString(d2)){	//Todo char ascii es imprimible
				if(d2 != d3){
					tipoErrorGestion = ERROR_RETYPE_SSID;
					request->redirect("/getApGestion");
				}else{
					datos = d1+"/"+d2;
					tipoErrorGestion = NO_ERROR;
					request->redirect("/getConfirmSSID");
				}
			}else{										//Algun ascii no imprimible
				tipoErrorApGestion = ERROR_NO_PRINTABLE;
				request->redirect("/getApGestion");
			}
			
		}
    });

	server.on("/getConfirmSSID", HTTP_GET, [] (AsyncWebServerRequest *request) {//http:IP/getConfirmSSID
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			String t ="";
			t.concat((char)191);t+="Estas seguro de que quieres cambiar la configuraci";t.concat((char)243);t+="n del AP?";
			t+=" El AP se reestablecer";t.concat((char)225);t+=" y tendr";t.concat((char)225);t+=" que volver a conectarse con los datos nuevos.";
			t+="/"+datos;
			response->print(getConfirm(t));
			request->send(response);
		}
	});

	server.on("/postConfirmSSID", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postConfirmSSID
		//No se puede poner asi -> if(!autorizacion)request->redirect("/getLogin"); hace tb lo siguiente
		//Puede hacer dos request->send() y eso esta mal
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			String b = request->arg("env");
			if(b == "Confirmar"){
				String nssid = request->arg("ssid");
				String npsw = request->arg("psw");

				if(setDatosAP(nssid, npsw)){
					currentSSID=getSSID(); currentPSW=getPSW();
					WiFi.softAP(currentSSID, currentPSW, 1, 0, 1);
					request->redirect("/getGestion");
				}else{
					Serial.println("No se pudo establecer nuevos datos de configuracion AP");
					tipoErrorGestion = ERROR_DATA_AP;
					request->redirect("/getApGestion");
				}
			}else{//Cancelar
				request->redirect("/getApGestion");
			}
		}
    });

	server.on("/postUser", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postUser
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			String u = request->arg("Usuario");
			String n = request->arg("NombreApellidos");
			String p = request->arg("psw");
			String r = request->arg("retype");

			if(isValidString(u) && isValidString(n) && isValidString(p)){	//Todo char ascii es imprimible
				if(p != r){
					tipoErrorGestion = ERROR_RETYPE_USER;
					request->redirect("/getUserGestion");
				}else{
					datos = u+"/"+n+"/"+p;
					tipoErrorGestion = NO_ERROR;
					request->redirect("/getConfirmUser");
				}
			}else{															//Algun ascii no imprimible
				tipoErrorUserGestion = ERROR_NO_PRINTABLE;
				request->redirect("/getUserGestion");
			}
			
		}
    });

	server.on("/getConfirmUser", HTTP_GET, [] (AsyncWebServerRequest *request) {//http:IP/getConfirmUser
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			String t = "";
			t.concat((char)191);t+="Estas seguro de que quieres cambiar el nombre del usuario a: "+buscarBloque(datos, '/', 1)+
			", tu nombre y apellidos a: "+buscarBloque(datos, '/', 2)+" y la contrase";t.concat((char)241);t+="a del login?";
			t+="/"+datos;
			response->print(getConfirm(t));
			request->send(response);
		}
	});

	server.on("/postConfirmUser", HTTP_POST, [](AsyncWebServerRequest *request){//http:IP/postConfirmUser
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			String b = request->arg("env");
			if(b == "Confirmar"){
				String nuser = request->arg("nuser");
				String nya = request->arg("nya");
				String psw = request->arg("psw");
				if(setDatosUsuario(nuser, nya, psw)){
					miUsuario=getUser();
					if(miUsuario){
						Serial.println(miUsuario->user);
						Serial.println(miUsuario->nombreApellidos);
						Serial.println(miUsuario->psw);
					}else{
						Serial.println("Error: Fallo en getUser");
					}
					request->redirect("/getGestion");
				}else{
					Serial.println("No se pudo establecer nuevos datos de configuracion del usuario");
					tipoErrorGestion = ERROR_DATA_USER;
					request->redirect("/getUserGestion");
				}
			}else{//Cancelar
				request->redirect("/getUserGestion");
			}
		}
    });

	server.on("/postQuitarCargada", HTTP_POST, [] (AsyncWebServerRequest *request) { //http:IP/postQuitarCargada
		//Esta es para cuando se pulse "Nueva" para crear una nueva configuración
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			idConf = 0;						//No hay nada cargado
			ejecutando = NO_RUN;			//Si no esta cargada n o puede ejecutarse
			tiempo_ms = 0L;					//Si es asincronico da igual es 0, si es sincronico a 0
			resetArrayBool5();				//Quitar carga del array de booleanos
			request->redirect("/getGestion");//Vuelvo a gestion sin nada cargado
		}
	});

	server.on("/postGestion", HTTP_POST, [] (AsyncWebServerRequest *request) {	//http:IP/postGestion
		//Ejecutar, Stop, Guardar, Actualizar, Configuraciones
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			bool selectedAtListOne = false;				//Se ha seleccionado al menos 1
			String accion = request->arg("accion");		//Ejecutar, Stop, Configuraciones, Guardar, Actualizar
			String tipoEnvio = request->arg("tipoEnvio");//asyn=0, sin=1
			String d = request->arg("setTime");			//Tiempo en milisegundos

			if(tipoEnvio == "0"){
				tiempo_ms = 0L;
			}else{
				tiempo_ms=(long)StringToInt(d);//Esta funcion me da un int en vez de un long (hecha por mi)
				//Si hay algo que no sea un numero -> 0
				if(tiempo_ms == 0L)Serial.println("WARNING: solo numeros enteros > 0");
			}
			String df = tipoEnvio;						//Ire concatenado los valores del formulario
			if(request->hasArg("ib")){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(request->hasArg("gb")){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(request->hasArg("ip")){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(request->hasArg("fus")){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(tipoEnvio == "0"){
				df+="-0";
			}else{
				df+="-";
				df+=(String)StringToInt(d);//Porque puede ser no valida '100.1' o '12a2' ...
			}
			//Fin de recogida de datos
			
			if(accion == "Configuraciones"){//*********************CONFIGURACIONES
				tipoErrorGestion = NO_ERROR;
				request->redirect("/getConfiguraciones");

			}else if(accion == "Guardar"){	//*********************GUARDAR
				if(idConf > 0){
					Serial.println("ERROR: se ha intentado guardar como nueva conf una cargada");
					request->redirect("/getGestion");
				}else{
					if(selectedAtListOne){//Se ha checkeado al menos un checkbox
						if(addNewConf(df)){	//Añadida
							tipoErrorGestion = NO_ERROR;
							request->redirect("/getConfiguraciones");
						}else{				//No se pudo añadir
							tipoErrorGestion = ERROR_CONF_ADD;
							request->redirect("/getGestion");
						}
					}else{
						tipoErrorGestion = ERROR_NO_SELECT_CHECKBOX;
						request->redirect("/getGestion");
					}
				}
			}else if(accion == "Actualizar"){//*********************ACTUALIZAR
				if(idConf == 0){//No hay configuracion cargada, no se puede actualizar una sin cargarse
					Serial.println("ERROR: se ha intentado actualizar cuando no hay ninguna conf cargada");
					request->redirect("/getGestion");
				}else{
					if(selectedAtListOne){
						if(updateConfig(idConf, df)){
							tipoErrorGestion = NO_ERROR;
							request->redirect("/getConfiguraciones");
						}else{
							tipoErrorGestion = ERROR_NO_UPDATE;
							request->redirect("/getGestion");
						}
					}else{
						tipoErrorGestion = ERROR_NO_SELECT_CHECKBOX;
						request->redirect("/getGestion");
					}
				}
			}else if(accion == "Ejecutar"){//*********************EJECUTAR
				if(idConf == 0){//No hay configuracion cargada, no se puede ejecutar una sin cargarse
					Serial.println("ERROR: se ha intentado ejecutar cuando no hay ninguna conf cargada");
				}else{
					if(selectedAtListOne){
						tipoErrorGestion = NO_ERROR;
						configToBoolArray5(df);
						ejecutando = RUN;
					}else{
						tipoErrorGestion = ERROR_NO_SELECT_CHECKBOX;
					}
				}
				request->redirect("/getGestion");//Pase lo que pase -> gestion
			}else if(accion == "Stop"){
				if(idConf == 0){//No hay configuracion cargada, no se puede parar sino hay cargada
					Serial.println("ERROR: se ha intentado parar cuando no hay ninguna conf cargada");
				}else{
					ejecutando = NO_RUN;
				}
				request->redirect("/getGestion");
			}else{//No existe ese boton, intento de acceso ilegal (por seguridad)
				Serial.println("No existe el boton");
				Serial.println(accion);
				autorizacion=false;
				request->redirect("/getLogin");
			}
		}
	});

	server.on("/getConfiguraciones", HTTP_GET, [] (AsyncWebServerRequest *request) {//http:IP/getConfirmUser
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			response->print(getDatosConfiguraciones());
			request->send(response);
		}
	});

	server.on("/postConfig", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postConfig
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			if(request->arg("et") != ""){
				ejecutando = NO_RUN;					//Borrar todas -> se para, no conf, no ejecuta
				resetArrayBool5();						//No configuraciones -> arraybool vacio
				tiempo_ms = 0L;							//Por si habia alguna cargada
				idConf = 0;								//Por si habia alguna cargada
				resetDatosConfiguraciones();			//Borro todas de la SPIFFS
				request->redirect("/getConfiguraciones");
			}else{
				String idpulsado = request->arg("idpulsado");
				int id = StringToInt(idpulsado);		//Id de la conf pulsada, puede ser boton "Cargar" o "Eliminar"
				String botonpulsado = request->arg("botonpulsado");
				if(botonpulsado == "Cargar"){
					ejecutando = NO_RUN;				//Si le doy a cargar -> se para, se carga pero no ejecuta aun
					String miConf = getConfig(id);		//Obtengo la configuracion de la SPIFFS
					if(!configToBoolArray5(miConf)){	//La paso a array de booleanos
						Serial.println("ERROR: no cargado en arrayBool");
						tiempo_ms = 0L;
					}else{//Si se cargo correctamente, no era vacio, cargo tiempo_ms
						tiempo_ms = StringToInt(miConf.substring(6));//Obtengo el valor de los ms
						idConf = id;					//Set indice de la configuracion cargada
					}
					request->redirect("/getGestion");
				}else if(botonpulsado == "Eliminar"){
					if(!deleteConf(id)){
						Serial.println("Error al borrar la configuracion");
					}else{
						if(id == idConf){		//Si la que se ha borrado es la que estaba cargada
							ejecutando = NO_RUN;//Si se estuviera ejecutando se para
							tiempo_ms = 0L;		//Sea el que sea a 0
							idConf = 0;			//Si ya no esta no esta cargada
							resetArrayBool5();	//Se descarga de arrayBool
						}//Sino es el mismo deja todo como estaba, solo borra datos
					}
					request->redirect("/getConfiguraciones");
				}else{//No existe ese boton, intento de acceso ilegal (por seguridad)
					autorizacion=false;
					request->redirect("/getLogin");
				}
			}
		}
    });

    server.onNotFound(notFound);
	
    server.begin();
	//FUNCIONES DEL MPU
	//Wire.begin();		//Iniciar I2C
	//MPU6050_iniciar();//Configuracion del IMU
	loop_timer = millis();
}

void loop() {
	if(WiFi.softAPgetStationNum() == 0){
		autorizacion=false;			//Si se desconecta se cierra la sesion
		tipoErrorLogin = NO_ERROR;	//Vuelta a empezar, no hay errores
	}

	comandoRecibido = "";
	while(Serial.available() > 0){
		Serial.println("available");
		comandoRecibido += (char)Serial.read();
	}
	parametros = "";
	ins = getInstruccion(comandoRecibido, parametros);
	if( ins != 0)exec(ins, parametros);
	
	if (ejecutando){
		if(actualConf[1]){
		Serial.println("IMU RAW");
		}
		if(actualConf[2]){
			Serial.println("GPS");
		}
		if(actualConf[3]){
			Serial.println("IMU Kalman");
		}
		if(actualConf[4]){
			Serial.println("Fusion");
		}
	}
	
	delay(1000);

	if ((millis() - loop_timer) >= (unsigned long)tiempo_ms){
		Serial.println("Dentro");
		loop_timer = millis();
	}
	/*
	if ((millis() - loop_timer) >= 2000){
		tiempo_ejecucion = (millis() - loop_timer) / 1000;//A segundos
		MPU6050_leer();
		MPU6050_procesado1();
		mostrar();
		Serial.println("--------------------------------------------------------");
		Serial.println();
		loop_timer = millis();
	}
	if (gpsSerial.available())
	{
		char data;
		data = gpsSerial.read();
		Serial.print(data);
	}
	*/
}


