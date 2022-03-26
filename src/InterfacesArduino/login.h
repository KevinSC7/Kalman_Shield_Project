String getLogin(byte tipoError){
	String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'><head><title>LOGIN</title></head>";
   	html+="<style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}td, th {border: 3px solid #dddddd;text-align: center;}";
   	html+="input[type=text],input[type=password]{border: 2px solid;}</style><body ><header><h1>LOGIN</h1></header>";
   	html+="<form action='/' method='GET' align='right'><input type='submit' value='Volver atras'></form>";
   	html+="<form method='POST' action='/postLogin'>";
   	html+="<table align='center' border='2px' width='100%'>";
   	html+="<tr><th align='center'><label for='user'>Usuario</label><br><input type='text' id='user' name='user'>";
	html+="<div id='errorUser' style='color: red;'>";
	if(tipoError == 1)html+="Usuario incorrecto";
	html+="</div><br><br><label for='psw'>Contrase"; html.concat((char)241); html+="a</label>";
   	html+="<br><input type='password' id='psw' name='psw'><div id='errorPsw' style='color: red;'>";
	if(tipoError == 2){
		html+="Contrase";
		html.concat((char)241);//La ñ
		html+="a incorrecta";
	}
	html+="</div><br><br></th></tr>";
   	html+="<tr><th><input type='submit' value='Login'></th></tr>";
   	html+="</table></form><br>";
   	html+="<footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer>";
   	html+="</body></html>";
   	return html;
}