
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