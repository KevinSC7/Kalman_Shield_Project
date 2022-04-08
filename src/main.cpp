//INSTALACION IDE: https://www.luisllamas.es/como-programar-arduino-con-visual-studio-code-y-plaftormio/

#include <Arduino.h>
#include <ESP8266WiFi.h>		//Para configuración SSID, AP y uso de otras funciones
#include <ESPAsyncTCP.h>		//Necesario para realizar TCP asicronico
#include <ESPAsyncWebServer.h>	//Permite operar con peticiones web asincronicas
#include <LittleFS.h>			//Para operar con la memoria SPIFFS del ESP826612

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
#define NO_CONFIG ""
#define NO_RUN 0
#define RUN 1

AsyncWebServer server(80);				//Puerto 80

const char* ssid = "KalmanShield";		//Nombre baliza AP por defecto (RF1)
const char* password = "KalmanShield";	//8 caracteres (Req. seguridad) y por defecto esa contraseña (RF1)
String currentSSID = ssid;
String currentPSW = password;
bool autorizacion = false;
//bool actualConf[5]={0,0,0,0,0};			//5 datos: async/sinc, ib, gb, ip, fus
bool ejecutando;
String miConf;							//Configuracion cargada
double tiempo_ms;						//Ultimo dato de la configuracion actual, tasa de envio
int idConf;								//Indice de la actual configuracion
byte tipoErrorGestion;					//Tipo de error al cargar gestion
byte tipoErrorLogin;					//Tipo de error al cargar login
String datos;							//Usos varios, datos globales

//ESTO ES CODIGO SEPARADO, TIENE QUE ESTAR ANTES DE LAS VARIABLES GLOBALES
#include "Data/herramientasParser.h" 	//Herramientas propias que ayudan a parsear Strings
#include "Data/usuarios.h"				//Control de datos del usuario sobre SPIFFS
#include "Data/SSID.h"					//Control de datos del AP sobre SPIFFS, contiene tambien IPAddress
#include "Data/config.h"				//Control de datos de las configuraciones

usuario *miUsuario;						//Ver struct en usuarios.h

#include "InterfacesArduino/login.h"
#include "InterfacesArduino/gestionDatos.h"
#include "InterfacesArduino/confirm_AP_USER.h"
#include "InterfacesArduino/datosConfiguraciones.h"

void notFound(AsyncWebServerRequest *request) {	//Para URL no encontrada
    request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);							//Baudios del puerto serie
	idConf = 0;										//No hay configuracion cargada
	tipoErrorGestion = NO_ERROR;					//Acceso a gestion sin errores
	tipoErrorLogin = NO_ERROR;						//Acceso a login sin errores
	miConf = NO_CONFIG;								//No hay configuracion cargada
	ejecutando = NO_RUN;							//No esta ejecutandose la configuracion
	tiempo_ms=(double)0;							//Tasa de envio de datos 0ms

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
		idConf = 0;									//Siempre que haya logout->no hay configuracion
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
			response->print(getGestionDatos(tipoErrorGestion, miConf, ejecutando));
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
			
			if(d2 != d3){
				tipoErrorGestion = ERROR_RETYPE_SSID;
				request->redirect("/getGestion");
			}else{
				datos = d1+"/"+d2;
				tipoErrorGestion = NO_ERROR;
				request->redirect("/getConfirmSSID");
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
				}else{
					Serial.println("No se pudo establecer nuevos datos de configuracion AP");
					tipoErrorGestion = ERROR_DATA_AP;
				}
			}
			request->redirect("/getGestion");
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
			if(p != r){
				tipoErrorGestion = ERROR_RETYPE_USER;
				request->redirect("/getGestion");
			}else{
				datos = u+"/"+n+"/"+p;
				tipoErrorGestion = NO_ERROR;
				request->redirect("/getConfirmUser");
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
				}else{
					Serial.println("No se pudo establecer nuevos datos de configuracion del usuario");
					tipoErrorGestion = ERROR_DATA_USER;
				}
			}//else 'Cancelar'
			request->redirect("/getGestion");
		}
    });

	server.on("/postGestion", HTTP_POST, [] (AsyncWebServerRequest *request) {	//http:IP/postGestion
		//Ejecutar, Stop, Guardar, Actualizar, Configuraciones
		if(!autorizacion){
			request->redirect("/getLogin");
		}else{
			bool selectedAtListOne = false;
			String accion = request->arg("accion");
			String tipoEnvio = request->arg("tipoEnvio");
			String d = request->arg("setTime");
			Serial.println("miConf: "+request->arg("miConf"));
			if(request->arg("miConf") == 0){
				idConf = 0;
				miConf = NO_CONFIG;
			}
			if(tipoEnvio == "0"){
				tiempo_ms=0;
			}else{
				if(d.indexOf(',') != -1 || d.indexOf('.') != -1){
					Serial.println("Error solo numeros enteros");
					tiempo_ms=0;
				}else{
					tiempo_ms=d.toDouble();
				}
			}
			String df = tipoEnvio;
			if(request->arg("ib")=="ok"){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(request->arg("gb")=="ok"){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(request->arg("ip")=="ok"){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(request->arg("fus")=="ok"){
				df+=1; selectedAtListOne = true;
			}else{
				df+=0;
			}
			if(tipoEnvio == "0"){
				df+="-0";
			}else{
				df+="-";
				df+=d;
			}
			//Fin de recogida de datos
			
			if(accion == "Configuraciones"){
				tipoErrorGestion = NO_ERROR;
				request->redirect("/getConfiguraciones");
			}else if(accion == "Guardar"){
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
			}else if(accion == "Actualizar"){
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
			}else if(accion == "Ejecutar"){
				if(selectedAtListOne){
					tipoErrorGestion = NO_ERROR;
					ejecutando = RUN;
				}else{
					tipoErrorGestion = ERROR_NO_SELECT_CHECKBOX;
				}
				request->redirect("/getGestion");
			}else if(accion == "Stop"){
				ejecutando = NO_RUN;
				request->redirect("/getGestion");
			}else{//No existe ese boton, intento de acceso ilegal (por seguridad)
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
				resetDatosConfiguraciones();
				request->redirect("/getConfiguraciones");
			}else{
				String idpulsado = request->arg("idpulsado");
				idConf = idpulsado.toInt();
				String botonpulsado = request->arg("botonpulsado");
				if(botonpulsado == "Cargar"){
					miConf = getConfig(idConf);
					request->redirect("/getGestion");
				}else if(botonpulsado == "Eliminar"){
					if(!deleteConf(idConf)){
						Serial.println("Error al borrar la configuracion");
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
}

void loop() {
	if(WiFi.softAPgetStationNum() == 0){
		autorizacion=false;//Si se desconecta se cierra la sesion
		tipoErrorLogin = 0;//Vuelta a empezar, no hay errores
	}
}


