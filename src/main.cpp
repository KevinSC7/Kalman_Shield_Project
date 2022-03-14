//INSTALACION IDE: https://www.luisllamas.es/como-programar-arduino-con-visual-studio-code-y-plaftormio/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "Data/config.h"
#include "Data/usuarios.h"

#include "InterfacesArduino/login.h"

AsyncWebServer server(80);			//Puerto 80

const char* ssid = "KalmanShield";	//Nombre baliza AP
const char* password = "KevinSC7";	//8 caracteres sino el móvil (al menos el mío) no deja establecer conexión
bool usarUsuarios = true;			//Si puedo acceder al sistema de usuarios o hay algun problema

void notFound(AsyncWebServerRequest *request) {	//Para URL no encontrada
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);							//Baudios del puerto serie
    WiFi.mode(WIFI_AP);								//Establecer modo AP
	WiFi.softAPConfig(local_IP, gateway, subnet);	//Configuración (Poner antes de encender el AP, sino no funciona)
	WiFi.softAP(ssid, password);					//Lanzamiento de la baliza AP
													//Si despues de enceder la baliza se configura no conecta
	Serial.println("AP configurado y lanzado");
	//PREPARAR DATOS
	
	if(!usuariosBegin()){
		usarUsuarios=false; Serial.println("Error en usuarios");
	} else{
		usarUsuarios = true;
	}
	
	//FIN PREPARAR DATOS
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){	//Mi http:IP/
        request->redirect("/getLogin");
    });

    server.on("/getLogin", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getLogin
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		response->print(getLogin());	//Aqui el html, poner la funcion antes de la llamada
        request->send(response);
    });

    server.on("/postLogin", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postLogin
		AsyncResponseStream *response = request->beginResponseStream("text/plain");
		String n = request->arg("user");//Backend del Post
		usuario *u=new usuario();
		if(u){
			Serial.println(u->user);
			Serial.println(u->nombreApellidos);
			Serial.println(u->psw);
		}
		n = "EXISTE";
        response->print(n);				//Aqui el html
        request->send(response);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
	
}


