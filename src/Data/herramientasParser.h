
String buscarBloque(String cadena, char delimitador, int numeroBloque){
  int i = 0, s, n = 0;                                  //Indice(i), siguiente(s) y bloque obtenido(n)
  String r = "";                                        //Valor a retornar
  if(numeroBloque < 1 || cadena.isEmpty())return "";    
  while(true){
    n++;
    s = cadena.indexOf(delimitador, i);                 //Desde i busca el delimitador
    if(s < 0){                                          //No hay mas bloques por buscar, return
      if(n == numeroBloque) return cadena.substring(i); //El bloque ultimo coincide con el pedido, return bloque (con \n del String si lo tuviese)
      return "";                                        //El numero de bloque que pidio no existe (ej: hay 3 bloques y pides el 4), return vacio
    }
    r = cadena.substring(i, s);                         //Valor del bloque en el que estoy
    if(n == numeroBloque)return r;                      //Si coincide con el bloque devuelve el valor
    i = s+1;
  }
}

int numBloques(String cadena, char delimitador){
  if(cadena.isEmpty()) return 0;
  int i = 0, s, n = 0;
  while(true){
    n++;
    s = cadena.indexOf(delimitador, i);
    if(s < 0) break;
    i = s+1;
  }
  return n;
}

String borrarBloque(String cadena, char delimitador, int numeroBloque){
  int i = 0, s, n = 0;                                  //Indice(i), siguiente(s) y bloque obtenido(n)
	String parte1 = "", parte2 = "";
	boolean principio = true;
	if(numeroBloque < 1 || cadena.isEmpty())return cadena;
  if(cadena.indexOf(delimitador, i) < 0)return cadena;//Sino existe delimitador la deja intacta
	while(true){
		n++;
		 if(n == numeroBloque){
		  principio = false; //Por si es el primero, n=numbloque=1
		  s = cadena.indexOf(delimitador, i);//Me salto el bloque
		  if(s < 0)break;
		  i = s+1;
		}
		s = cadena.indexOf(delimitador, i);
		if(s < 0){
		  break;                                        
	  }
		if(principio)parte1 += cadena.substring(i, s)+"/";           //Si no he llegado al numBloque, añade al principio
		if(!principio)parte2 += cadena.substring(i, s)+"/";
		i = s+1;
	}
  return parte1+parte2;
}

String actualizarBloque(String cadena, char delimitador, int numeroBloque, String nuevo){
  int i = 0, s, n = 0;
	String parte1 = "", parte2 = "";
	boolean principio = true;
	if(numeroBloque < 1 || cadena.isEmpty())return cadena;
  if(cadena.indexOf(delimitador, i) < 0)return cadena;//Sino existe delimitador la deja intacta
	while(true){
		n++;
		if(n == numeroBloque){
		  principio = false;
		  s = cadena.indexOf(delimitador, i);
		  if(s < 0)break;
		  parte2 += nuevo+"/";
		  i = s+1;
		}
		s = cadena.indexOf(delimitador, i);
    if(s < 0){
    	break;                                        
  	}
	  if(principio)parte1 += cadena.substring(i, s)+"/";
	  if(!principio)parte2 += cadena.substring(i, s)+"/";
	   i = s+1;
	  }
  return parte1+parte2;
}

int StringToInt(String cadena){
  int l = cadena.length();
  int i = 0, ceros = l-1;//125 -> 1*100 + 2*10 + 5*1 (l=3, l-1=numero de ceros)
  int suma = 0;
  while(i < l){
    if(cadena[i] > 47 && cadena[i] < 58){ //Si es un numero (ascii table)
      suma += (cadena[i]-'0')*(pow(10, ceros)); //125 -> 1*(10^2) + 2*(10^1) + 5*(10^0)
      ceros--;
    }else{
      return 0;
    }
    i++;
  }
  return suma;
}

int StringBinToInt(String cadena){
  int l = cadena.length()-1;
  int resultado = 0;
  for(int i = l; i > -1; i--){
    if(cadena[i] == '1'){
      resultado += pow(2, (l-i));
    }else if(cadena[i] == '0');
    else return 0;                          //Caracter que no es ni 1 ni 0 -> return 0
  }
  return resultado;
}

bool configToBoolArray5(String c){// 00000-0/10000-100/01000-0...
  if(c.isEmpty())return false;
  for(int i = 0; i < 5; i++){
    if(c[i]=='1')actualConf[i] = true;
    else actualConf[i] = false;
  }
  return true;
}

bool resetArrayBool5(){
  return configToBoolArray5("00000");
}