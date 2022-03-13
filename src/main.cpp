//INSTALACION IDE: https://www.luisllamas.es/como-programar-arduino-con-visual-studio-code-y-plaftormio/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

IPAddress local_IP(192,168,4,22);	//IP para acceder al servidor (el del ESP)
IPAddress gateway(192,168,1,1);		//Como AP suele ser irrelevante
IPAddress subnet(255,255,255,0);	//Máscara de red

AsyncWebServer server(80);			//Puerto 80

const char* ssid = "KalmanShield";	//Nombre baliza AP
const char* password = "KevinSC7";	//8 caracteres sino el móvil (al menos el mío) no deja establecer conexión

void notFound(AsyncWebServerRequest *request) {	//Para URL no encontrada
    request->send(404, "text/plain", "Not found");
}

String getLogin(){	//Buscar alternativa para poner el Frontend
	String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'><head><title>LOGIN</title></head>";
   	html+="<style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}td, th {border: 3px solid #dddddd;text-align: center;}";
   	html+="input[type=text],input[type=password]{border: 2px solid;}</style><body ><header><h1>LOGIN</h1></header>";
   	html+="<form action='/' method='GET' align='right'><input type='submit' value='Volver atras'></form>";
   	html+="<form method='POST' action='/postLogin'>";
   	html+="<table align='center' border='2px' width='100%'>";
   	html+="<tr><th align='center'><label for='user'>Usuario</label><br><input type='text' id='user' name='user'><br><br><label for='psw'>Contrasena</label>";
   	html+="<br><input type='password' id='psw' name='psw'><br><br></th></tr>";
   	html+="<tr><th><input type='submit' value='Login'></th></tr>";
   	html+="</table></form><br>";
   	html+="<footer>Brazo Robotico del Kevin<br>Copyright &copy; Krobotics</footer>";
   	html+="</body></html>";
   	return html;
}

void setup() {

    Serial.begin(115200);							//Baudios del puerto serie
    WiFi.mode(WIFI_AP);								//Establecer modo AP
	WiFi.softAPConfig(local_IP, gateway, subnet);	//Configuración (Poner antes de encender el AP, sino no funciona)
	WiFi.softAP(ssid, password);					//Lanzamiento de la baliza AP
													//Si despues de enceder la baliza se configura no conecta

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){	//Mi http:IP/
        request->send(200, "text/plain", "Hello, world");
    });

    server.on("/getLogin", HTTP_GET, [] (AsyncWebServerRequest *request) {	//http:IP/getLogin
		AsyncResponseStream *response = request->beginResponseStream("text/html");
		response->print(getLogin());	//Aqui el html, poner la funcion antes de la llamada
        request->send(response);
    });

    server.on("/postLogin", HTTP_POST, [](AsyncWebServerRequest *request){	//http:IP/postLogin
		AsyncResponseStream *response = request->beginResponseStream("text/plain");
		String n = request->arg("user");//Backend del Post
		if(n == "root") n="SOY ROOT";
        response->print(n);				//Aqui el html
        request->send(response);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
	
}


