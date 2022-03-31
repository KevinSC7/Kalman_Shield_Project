String getGestionDatos(String _ssid, String _user, String _NyA, String _pswUser, byte tipoError){
    String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'><head>";
    html+="<title>GESTION DE DATOS</title></head>";

    html+="<style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;} input[type=text],input[type=password]{border: 2px solid; overflow: hidden; margin-left: 20px; width: 65%;} input[type=submit]{background-color: #094709; color: white;border: 3px solid #080808;border-radius: 12px;font-size: 15px; height: 30px;} input[type=submit]:hover{background-color: #229422;} table {width: 100%; border: 2px} td, th {border: 3px solid #dddddd;} div {float: left;  margin-top: 10px; margin-bottom: 10px; white-space: nowrap;} p {font-size: 20px;}";
    html+=" .protegido {background-color: #080808; font-size: 10px;} .protegido:hover{background-color: gray;} .boton1 {width: 31%;}";
    
    html+="@media only screen and (orientation:portrait){body {margin-left: 6em;}html { transform: rotate(90deg); transform-origin: right top;width: 100vh; overflow-x: hidden; position: absolute; top: 100%; right: 0;}}";
    
    html+="</style><script>function showsText(object){if(object.value == 1){document.getElementById('setTime').hidden = false;}else{document.getElementById('setTime').hidden = true;}}";
    html+="</script><body><header><h1>Gestion de datos</h1></header>";

    html+="<table><tr><th colspan='3' style='border: 0px;'><form method='post' action='/logout'><input type='submit' value='Cerrar sesion' style='float: right; margin-bottom: 5px;'></form></th></tr>";
    
    html+="<tr style='vertical-align: top;'><th><hr><p><b>Configuraci";html.concat((char)243);html+="n AP</b></p>";
    html+="<form method='post' action='/postSSID'><div style='width: 40%;' align='right'><label for='SSID'>Nueva SSID*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' maxlength='20' required name='SSID' value='";
    html+=_ssid;
    html+="'></div>";
    html+="<div style='width: 40%;' align='right'><label for='psw'>Nueva contrase";html.concat((char)241);html+="a*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' minlength='8' required name='psw'></div>";
    html+="<div style='width: 40%;' align='right'><label for='retype'>Retype*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' minlength='8' name='retype' required></div>";
    html+="<div style='width: 100%;'><input type='submit' value='Establecer';></div>";
    html+="<div style='color: red; width: 100%;'>";
    if(tipoError == 1){
        html+="Retype incorrecto";
    }
    html+="</div>";

    html+="</form></th><th style='width: 25%; text-align: left;'><hr><p><b>Configuraci";html.concat((char)241);html+="n herramientas</b></p><hr>";
    html+="<form method='post' action='/postConfig'>";
    html+="<input type='radio' onchange='showsText(this);' name='tipoEnvio' id='asin' value='0' checked>";
    html+="<label for='asin'>Env";html.concat((char)237);html+="o asincr";html.concat((char)241);html+="nico de datos</label><br>";
    html+="<input type='radio' onchange='showsText(this);' name='tipoEnvio' id='sin' value='1'>";
    html+="<label for='sin'>Env";html.concat((char)237);html+="o s";html.concat((char)237);html+="ncrono de datos</label><br>";
    html+="<input type='text' id='setTime' placeholder='Tiempo en milisegundos (min:100ms=0.1s)' hidden>";
    html+="<hr><input type='checkbox' name='envio' id='ib' value='0'>";
    html+="<label for='ib'>Datos IMU en bruto</label><br>";
    html+="<input type='checkbox' name='envio' id='gb' value='1'><label for='gb'>Datos GPS en bruto</label>";
    html+=" <br><input type='checkbox' name='envio' id='ip' value='2'>";
    html+="<label for='ip'>Datos del IMU procesados con Kalman</label><br>";
    html+="<input type='checkbox' name='envio' id='fus' value='2'>";
    html+="<label for='fus'>Fusion datos IMU+GPS con Kalman</label>";
    html+="<hr><input type='submit' class='boton1' name='accion' value='Ejecutar'>";
    html+="<input type='submit' class='boton1' name='accion' value='Guardar'>";
    html+="<input type='submit' class='boton1' name='accion' value='Configuraciones'></form>";

    html+="<hr><p style='color: rgb(82, 24, 216); width: 100%; font-size: 15px;'>Formato de envio por serial:</p>";
    html+="<div id='rawData' style='color: #094709; width: 100%;'>/rawData/...</div></th>";

    html+="<th><hr><p><b>Configuraci";html.concat((char)243);html+="n Usuario</b></p>";
    html+=" <form method='post' action='/postChangeUser'>";
    html+="<div style='width: 40%;' align='right'><label for='Usuario'>Usuario*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' maxlength='20' required name='Usuario' value='";
    html+=_user;
    html+="'></div>";
    html+="<div style='width: 40%;' align='right'><label for='NombreApellidos'>Nombre y apellidos:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' maxlength='20' name='NombreApellidos' value='";
    html+=_NyA;
    html+="'></div>";
    html+="<div style='width: 40%;' align='right'><label for='pswActual'>Contrase";html.concat((char)241);html+="a actual:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' readonly maxlength='20' class='protegido' value='";
    html+=_pswUser;
    html+="' name='pswActual'></div>";
    html+="<hr style='width: 100%; margin-bottom: 10px;'><div style='width: 40%;' align='right'><label for='psw'>Nueva contrase";html.concat((char)241);html+="a*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' required name='psw'></div>";
    html+="<div style='width: 40%;' align='right'><label for='retype'>Retype*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' required name='retype'></div>";
    html+="<div style='width: 100%;'><input type='submit' value='Establecer'></div>";
    html+="<div style='color: red; width: 100%;'>";
    if(tipoError == 2){
        html+="Retype incorrecto";
    }
    html+="</div></form>";
    html+="</th></tr></table><hr>";
    html+="<footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";

    return html;
}