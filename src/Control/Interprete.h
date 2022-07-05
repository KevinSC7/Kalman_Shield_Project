int getInstruccion(String comando, String & parametros){
    int nb = numBloques(comando, '/');						//Tokenizacion 1
	if(nb < 2)return 0;										//Solo un bloque o nada-> ignora
	if(comando[comando.length()-1] == ((char)10)){			//Existe fin de linea (\n) al final (por consola al enviar lo tiene)
		comando = comando.substring(0, comando.length()-1);
	}
	
	String numeracion = "";
	for(int i = 1; i <= nb; i++){							//Sintactico
		String bloque = buscarBloque(comando, '/', i);	//Tokenizacion 2
		if(i == 1 && bloque != "ESPKALMAN")return -1;
		if(i == 2){
			if(bloque == "RESET")numeracion += "0001";
			else if(bloque == "REBOOT")numeracion += "0010";
			else if(bloque == "SET")numeracion += "0011";
			else if(bloque == "GET")numeracion += "0100";
			else if(bloque == "CAL")numeracion += "0101";
			else if(bloque == "ON")numeracion += "0110";
			else if(bloque == "OFF")numeracion += "0111";
			else if(bloque == "EXEC")numeracion += "1000";
			else if(bloque == "STOP")numeracion += "1001";
			else if(bloque == "PIN_READY")numeracion += "1010";
			else if(bloque == "")return -2;
			else return -2;
		}
		if(i == 3){
			if(bloque == "ALL")numeracion += "0001";
			else if(bloque == "SSID")numeracion += "0010";
			else if(bloque == "LOGIN")numeracion += "0011";
			else if(bloque == "MAX_VALUES")numeracion += "0100";
			else if(bloque == "CONF")numeracion += "0101";
			else if(bloque == "ON")numeracion += "0110";
			else if(bloque == "OFF")numeracion += "0111";
			else if(bloque == "")numeracion += "1000";//No llega, pero si llega ej: /REBOOT// -> error
			else {
				numeracion += "1001";
				parametros += bloque+"/";
			}
		}
		if(i == 4){
			if(bloque == "ALL")numeracion += "0001";
			else if(bloque == "")numeracion += "0010";
			else{
				numeracion += "0011";
				parametros += bloque+"/";
			}
		}
		if(i == 5)return -4;
	}
	
	return StringBinToInt(numeracion);
}

bool exec(int num, String parametros){//Sintacticamente correcto, aqui ver la gramatica (parametros)
	Serial.print("instruccion numero: ");
	Serial.println(num);
	Serial.print("parametros: ");
	Serial.println(parametros);
	
	if(num == 17){												//RESET ALL
		if(!resetDatosConfiguraciones())Serial.println("ERROR");
		if(!resetDatosUsuario())Serial.println("ERROR");
		if(!resetDatosAP())Serial.println("ERROR");

	}else if(num == 18){										//RESET SSID
		if(!resetDatosAP())Serial.println("ERROR");

	}else if(num == 19){										//RESET LOGIN
		if(!resetDatosUsuario())Serial.println("ERROR");

	}else if(num == 2){											//REBOOT
		Serial.println("END");									//Se pone antes porque este no lo mostraria
		ESP.restart();

	}else if(num == 114){//OFF SSID
		WiFi.softAP(currentSSID, currentPSW, 1, 1, 1);//Hidden pero no apagado
	}else if(num == 98){//ON SSID
		WiFi.softAP(currentSSID, currentPSW, 1, 0, 1);
	}else if(num == 1105){//GET CONF ALL
		String sendConf = "";
		int i = 1;
		while(getConfig(i) != ""){
			sendConf += getConfig(i)+" ";
			i++;
		}
		Serial.println(sendConf);
	}else if(num == 1107){//GET CONF nº conf
		int nconf = StringToInt(buscarBloque(parametros, '/', 1));
		if(nconf == 0)Serial.println("ERROR");
		else Serial.println(getConfig(nconf));
	}else if(num == 133){//EXEC CONF
		if(idConf != 0){//Hay una configuración caragada
			if(!configToBoolArray5(getConfig(idConf)))Serial.println("ERROR");	//Cambia el array de ejecucion
			else{
				if(actualConf[0])tiempo_ms = 0;
				else tiempo_ms = StringToInt(getConfig(idConf).substring(6));
				ejecutando = RUN;
			}
		}else{//No hay configuración caragada
			Serial.println("ERROR: SET ANY CONF ");
		}
	}else if(num == 851){//SET CONF nº conf
		int nconf = StringToInt(buscarBloque(parametros, '/', 1));
		if(nconf == 0)Serial.println("ERROR SET CONF");
		else{
			idConf = nconf;
		}
	}else if(num == 149){//STOP CONF
		if(!resetArrayBool5())Serial.println("ERROR");
		else{
			tiempo_ms = 0;
			ejecutando = NO_RUN;
		}
	}else if(num == 89){//CAL nº ciclos
		int ciclos = StringToInt(buscarBloque(parametros, '/', 1));
		if(ciclos == 0) Serial.println("ERROR: MUST BE INT TYPE AND NON ZERO");
		else{
			calibrate(ciclos);
		}
	}else if(num == 915){//SET Lat Lon
		double lat = buscarBloque(parametros, '/', 1).toDouble();
		double lon = buscarBloque(parametros, '/', 2).toDouble();
		latOrigin = lat;
		lonOrigin = lon;
	}else if(num == 166){//PIN_READY ON							
		Serial.println("PIN_READY ON");
	}else if(num == 167){//PIN_READY OFF
		Serial.println("PIN_READY OFF");
	}else if(num == 835){//SET MAX_VALUES 'nº max de valores, luego STOP'
		Serial.println("NO IMPLEMENTADO POR EL MOMENTO");
	}else if(num == -1){//Falta ESPKALMAN
		Serial.println("ERROR: 1 token desconocido");
		return false;

	}else if(num == -2){//Segundo comando no existe
		Serial.println("ERROR: 2 token desconocido");
		return false;

	}else if(num == -3){//Solo ESPKALMAN/ y nada mas
		Serial.println("ERROR: falta instruccion");
		return false;

	}else if(num == -4){//Exceso de bloques/parametros
		Serial.println("ERROR: Exceso de parametros");
		return false;

	}else{
		Serial.println("ERROR: comando desconocido");
		return false;
		
	}
	
	Serial.println("END");
	return true;
}