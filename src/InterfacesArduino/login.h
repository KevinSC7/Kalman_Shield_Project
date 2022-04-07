String getLogin(byte tipoError){
	String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'><head><title>LOGIN</title></head>";
   	html+="<style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}td, th {border: 3px solid #dddddd;text-align: center;}";
   	html+="input[type=text],input[type=password]{border: 2px solid;}";
	html+="input[type=submit] {color: white;border: 3px solid #080808; border-radius: 12px;font-size: 15px; width: 10%; margin-top: 5px;}";
	html+=".confirm{background-color: #095092; margin-bottom: 5px; margin-left: 25px; margin-right: 25px;}";
	html+=".confirm:hover{background-color: #1c93e2;}";
	html+="</style><body ><header><h1>LOGIN</h1></header>";
   	html+="<form method='POST' action='/postLogin'>";
   	html+="<table align='center' border='2px' width='100%'>";
   	html+="<tr><th align='center'><label for='user'>Usuario</label><br><input type='text' id='user' name='user'>";
	html+="<div id='errorUser' style='color: red;'>";
	if(tipoError == ERROR_USER)html+="Usuario incorrecto";
	html+="</div><br><br><label for='psw'>Contrase"; html.concat((char)241); html+="a</label>";
   	html+="<br><input type='password' id='psw' name='psw'><div id='errorPsw' style='color: red;'>";
	if(tipoError == ERROR_PSW){
		html+="Contrase";
		html.concat((char)241);//La ñ
		html+="a incorrecta";
	}
	html+="</div><br><br></th></tr>";
   	html+="<tr><th><input type='submit' class='confirm' value='Login'></th></tr>";
   	html+="</table></form><br>";
   	html+="<footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer>";
   	html+="</body></html>";
   	return html;
}