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
			else if(bloque == "CAL_V")numeracion += "0101";
			else if(bloque == "MPU")numeracion += "0110";
			else if(bloque == "EXEC")numeracion += "0111";
			else if(bloque == "STOP")numeracion += "1000";
			else if(bloque == "PIN_READY")numeracion += "1001";
			else if(bloque == "TOOL")numeracion += "1010";
			else if(bloque == "")return -2;
			else return -2;
		}
		if(i == 3){
			if(bloque == "ALL")numeracion += "0001";
			else if(bloque == "SSID")numeracion += "0010";
			else if(bloque == "LOGIN")numeracion += "0011";
			else if(bloque == "V")numeracion += "0100";
			else if(bloque == "M")numeracion += "0101";
			else if(bloque == "MAX_VALUES")numeracion += "0110";
			else if(bloque == "W")numeracion += "0111";
			else if(bloque == "END_T")numeracion += "1000";
			else if(bloque == "CONF")numeracion += "1001";
			else if(bloque == "ON")numeracion += "1010";
			else if(bloque == "OFF")numeracion += "1011";
			else if(bloque == "")numeracion += "1100";//No llega, pero si llega ej: /REBOOT// -> error
			else {
				numeracion += "1101";
				parametros += bloque+"/";
			}
		}
		if(i == 4){
			if(bloque == "ALL")numeracion += "0001";
			else if(bloque == "ON")numeracion += "1010";
			else if(bloque == "")numeracion += "1100";
			else{
				numeracion += "1101";
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

	}else if(num == 845){//SET V 'valor numerico'

	}else if(num == 68){//GET V

	}else if(num == 861){//SET M 'valor numerico'

	}else if(num == 69){//GET M 

	}else if(num == 1501){//CAL_V 'nº de tomas(int)' 'guardar=1, no guardar=0'/

	}else if(num == 1661){//MPU W 'direccion en HEX'

	}else if(num == 104){//MPU END_T

	}else if(num == 1169){										//GET CONF ALL
		String sendConf = "";
		int i = 1;
		while(getConfig(i) != ""){
			sendConf += getConfig(i)+" ";
			i++;
		}
		Serial.println(sendConf);

	}else if(num == 1181){										//GET CONF 'nº de configuracion (int)'
		int nconf = StringToInt(buscarBloque(parametros, '/', 1));
		if(nconf == 0)Serial.println("ERROR");
		else Serial.println(getConfig(nconf));

	}else if(num == 1949){										//EXEC CONF 'nº de configuracion (int)'
		int nconf = StringToInt(buscarBloque(parametros, '/', 1));
		if(nconf == 0)Serial.println("ERROR");
		else{				//Comienza a cargarla
			String pconf = getConfig(nconf);
			if(pconf != ""){//Si existe la carga
				miConf = pconf;
				idConf = nconf;
			}
			if(!configToBoolArray5(miConf))Serial.println("ERROR");	//Cambia el array de ejecucion
			else{
				if(actualConf[0])tiempo_ms = 0;
				else tiempo_ms = StringToInt(getConfig(nconf).substring(6));
				ejecutando = RUN;									//Pasa a ejecutar
			}		
		}

	}else if(num == 137){										//STOP CONF
		if(!resetArrayBool5())Serial.println("ERROR");
		else{
			tiempo_ms = 0;
			ejecutando = NO_RUN;
		}

	}else if(num == 154){//PIN_READY ON

	}else if(num == 155){//PIN_READY OFF

	}else if(num == 877){//SET MAX_VALUES 'nº max de valores, luego STOP'

	}else if(num == 2778){//TOOL 'KALMAN_1D|SIMPSON' ON

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