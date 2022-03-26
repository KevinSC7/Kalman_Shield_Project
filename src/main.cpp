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

//ESTO ES CODIGO SEPARADO, TIENE QUE ESTAR ANTES DE LAS VARIABLES GLOBALES
#include "Data/config.h"		//Contiene la configuración del AP (IPAddress)
#include "Data/herramientasParser.h" //Herramientas propias que ayudan a parsear Strings
#include "Data/usuarios.h"		//Control de datos del usuario sobre SPIFFS
#include "Data/SSID.h"			//Control de datos del AP sobre SPIFFS

#include "InterfacesArduino/login.h"

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
	WiFi.softAP(ssid, password);					//Lanzamiento de la baliza AP
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
				response->print("<p>gestionDatos()</p>");
			}else{
				response->print(getLogin(2));//Llamada a la intefaz login con error en psw tipoError = 2
			}
		}else{
			response->print(getLogin(1));//Llamada a la intefaz login con error en user tipoError = 1
		}
		
        request->send(response);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
	
}


