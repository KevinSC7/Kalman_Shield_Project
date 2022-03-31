//INSTALACION IDE: https://www.luisllamas.es/como-programar-arduino-con-visual-studio-code-y-plaftormio/

#include <Arduino.h>
#include <ESP8266WiFi.h>		//Para configuración SSID, AP y uso de otras funciones
#include <ESPAsyncTCP.h>		//Necesario para realizar TCP asicronico
#include <ESPAsyncWebServer.h>	//Permite operar con peticiones web asincronicas
#include <LittleFS.h>			//Para operar con la memoria SPIFFS del ESP826612

AsyncWebServer server(80);				//Puerto 80

const char* ssid = "KalmanShield";		//Nombre baliza AP por defecto (RF1)
const char* password = "KalmanShield";	//8 caracteres (Req. seguridad) y por defecto esa contraseña (RF1)
String currentSSID = ssid;
String currentPSW = password;
bool autorizacion = false;

//ESTO ES CODIGO SEPARADO, TIENE QUE ESTAR ANTES DE LAS VARIABLES GLOBALES
#include "Data/config.h"		//Contiene la configuración del AP (IPAddress)
#include "Data/herramientasParser.h" //Herramientas propias que ayudan a parsear Strings
#include "Data/usuarios.h"		//Control de datos del usuario sobre SPIFFS
#include "Data/SSID.h"			//Control de datos del AP sobre SPIFFS

#include "InterfacesArduino/login.h"
#include "InterfacesArduino/gestionDatos.h"
#include "InterfacesArduino/confirmConfig.h"
#include "InterfacesArduino/datosConfiguraciones.h"

usuario *miUsuario;						//Ver struct en usuarios.h

void notFound(AsyncWebServerRequest *request) {	//Para URL no encontrada
    request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);							//Baudios del puerto serie
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

	//FIN PREPARAR DATOS
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){	//Mi http:IP/
        request->redirect("/getLogin");
    });

    server.on("/getLogin", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getLogin
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		response->print(getLogin(0));	//Llamada a la intefaz login sin errores tipoError = 0
        request->send(response);
    });

    server.on("/postLogin", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postLogin
		AsyncResponseStream *response = request->beginResponseStream("text/html");//Backend del Post
		String nombreUsuario = request->arg("user");
		String pswUsuario = request->arg("psw");
		if(nombreUsuario == miUsuario->user){
			if(pswUsuario == miUsuario->psw){
				autorizacion = true;
				request->redirect("/getGestion");
			}else{
				response->print(getLogin(2));//Llamada a la intefaz login con error en psw tipoError = 2
			}
		}else{
			response->print(getLogin(1));//Llamada a la intefaz login con error en user tipoError = 1
		}
		
        request->send(response);
    });

	server.on("/logout", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/logout
		autorizacion=false;
		request->redirect("/getLogin");
    });

	server.on("/getGestion", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getGestion
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(autorizacion){
			response->print(getGestionDatos(currentSSID, miUsuario->user, miUsuario->nombreApellidos, miUsuario->psw, 0));	//Llamada a la intefaz gestion datos sin errores tipoError = 0
			request->send(response);
		}else{
			request->redirect("/getLogin");
		}
    });

	server.on("/postSSID", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postSSID
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(!autorizacion)request->redirect("/getLogin");
		String d1 = request->arg("SSID");
		String d2 = request->arg("psw");
		String d3 = request->arg("retype");
		String t ="";
		if(d2 != d3){
			response->print(getGestionDatos(currentSSID, miUsuario->user, miUsuario->nombreApellidos, miUsuario->psw, 1));
			request->send(response);
		} 
		t.concat((char)191);t+="Estas seguro de que quieres cambiar la configuraci";t.concat((char)243);t+="n del AP?";
		
		response->print(getConfirm(t, d1, d2));
		request->send(response);
		
    });

	server.on("/postSSIDConfirm", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postSSIDConfirm
		if(!autorizacion)request->redirect("/getLogin");
		String b = request->arg("env");
		if(b == "Confirmar"){
			String nssid = request->arg("ssid");
			String npsw = request->arg("psw");
			Serial.println("nssid: "+nssid+" npsw: "+npsw);
		}
		
		request->redirect("/getGestion");
    });

	server.on("/postChangeUser", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postChangeUser
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(!autorizacion)request->redirect("/getLogin");
		String u = request->arg("Usuario");
		String n = request->arg("NombreApellidos");
		String p = request->arg("psw");
		String r = request->arg("retype");
		String t = "";
		t.concat((char)191);t+="Estas seguro de que quieres cambiar el nombre del usuario a: "+u+
		", tu nombre y apellidos a:"+n+" y la contrase";t.concat((char)241);t+="a del login?";
		if(p != r){
			response->print(getGestionDatos(currentSSID, miUsuario->user, miUsuario->nombreApellidos, miUsuario->psw, 2));	//Llamada a la intefaz gestion datos con error mal retypr tipoError = 0
			request->send(response);
		}
		String datos[]={t, u, n, p, r};
		response->print(getConfirm(datos));
		request->send(response);
    });

	server.on("/postChangeUserConfirm", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postChangeUserConfirm
		if(!autorizacion)request->redirect("/getLogin");
		String b = request->arg("env");
		if(b == "Confirmar"){
			String nuser = request->arg("nuser");
			String nya = request->arg("nya");
			String psw = request->arg("psw");
			Serial.println("nuser: "+nuser+" nya: "+nya+" psw: "+psw);
		}
		
		request->redirect("/getGestion");
    });

	server.on("/postConfig", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postConfig
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		if(!autorizacion)request->redirect("/getLogin");
		String accion = request->arg("accion");
		if(accion == "Configuraciones"){
			response->print(getDatosConfiguraciones());
		}else if(accion == "Ejecutar"){
			response->print("");
		}else if(accion == "Guardar"){
			response->print("");
		}else{
			request->send(401);
		}
		
		request->send(response);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
	if(WiFi.softAPgetStationNum() == 0)autorizacion=false;//Si se desconecta se cierra la sesion
}


