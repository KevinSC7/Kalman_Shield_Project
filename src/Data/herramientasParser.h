
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
	if(numeroBloque < 1 || cadena.isEmpty())return "";   
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
	if(numeroBloque < 1 || cadena.isEmpty())return "";   
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