String getGestionDatos(byte tipoError, String _config, bool ejecutando){
    bool sin=false, ib=false, gb=false, ip=false, fus=false;
    String t="";
    if(!_config.isEmpty()){
        if(_config[0]=='1')sin = true;
        if(_config[1]=='1')ib = true;
        if(_config[2]=='1')gb = true;
        if(_config[3]=='1')ip = true;
        if(_config[4]=='1')fus =true;
        t = _config.substring(6);
    }
    
    
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
    html+="<div style='width: 60%;' align='left'><input type='text' maxlength='20' minlength='1' required name='SSID' value='";
    html+=currentSSID;
    html+="'></div>";
    html+="<div style='width: 40%;' align='right'><label for='psw'>Nueva contrase";html.concat((char)241);html+="a*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' minlength='8' required name='psw'></div>";
    html+="<div style='width: 40%;' align='right'><label for='retype'>Retype*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' minlength='8' name='retype' required></div>";
    html+="<div style='width: 100%;'><input type='submit' value='Establecer';></div>";
    html+="<div style='color: red; width: 100%;'>";
    if(tipoError == ERROR_RETYPE_SSID){
        html+="Retype incorrecto";
    }
    if(tipoError == ERROR_DATA_AP){
        html+="Fallo al establecer los datos";
    }
    html+="</div>";

    html+="</form></th><th style='width: 25%; text-align: left;'><hr><p><b>Configuraci";html.concat((char)243);html+="n herramientas</b></p><hr>";
    html+="<form method='post' action='/postGestion'>";
    html+="<input type='radio' onchange='showsText(this);' name='tipoEnvio' id='asin' value='0'"; if(!sin)html+=" checked";html+=">";
    html+="<label for='asin'>Env";html.concat((char)237);html+="o asincr";html.concat((char)243);html+="nico de datos</label><br>";
    html+="<input type='radio' onchange='showsText(this);' name='tipoEnvio' id='sin' value='1'";if(sin)html+=" checked";html+=">";
    html+="<label for='sin'>Env";html.concat((char)237);html+="o s";html.concat((char)237);html+="ncrono de datos</label><br>";
    html+="<input type='number' name='setTime' id='setTime' value='";html+=t;html+="' placeholder='Tiempo en milisegundos (min:100ms=0.1s)' ";if(!sin)html+=" hidden";html+=">";
    html+="<hr><input type='checkbox' name='ib' id='ib' value='ok'";if(ib)html+=" checked";html+=">";
    html+="<label for='ib'>Datos IMU en bruto</label><br>";
    html+="<input type='checkbox' name='gb' id='gb' value='ok'";if(gb)html+=" checked";html+="><label for='gb'>Datos GPS en bruto</label>";
    html+=" <br><input type='checkbox' name='ip' id='ip' value='ok'"; if(ip)html+=" checked";html+=">";
    html+="<label for='ip'>Datos del IMU procesados con Kalman</label><br>";
    html+="<input type='checkbox' name='fus' id='fus' value='ok'";if(fus)html+=" checked";html+=">";
    html+="<label for='fus'>Fusion datos IMU+GPS con Kalman</label>";
    html+="<hr><input type='submit' class='boton1' name='accion' value='Ejecutar'>";
    html+="<input type='submit' class='boton1' name='accion' value='Guardar'>";
    html+="<input type='submit' class='boton1' name='accion' value='Configuraciones'></form>";
    html+="<div style='color: red; width: 100%;'>";
    if(tipoError == ERROR_CONF_ADD){
        html+="No se pudo guardar. Máximo de configuraciones: 10";
    }
    if(tipoError == ERROR_NO_SELECT_CHECKBOX){
        html+="No se guard";html.concat((char)243);html+=". Escoja minimo una herramienta(checkbox).";
    }
    html+="</div>";
    html+="<hr><p style='color: rgb(82, 24, 216); width: 100%; font-size: 15px;'>Formato de envio por serial:</p>";
    html+="<div id='rawData' style='color: #094709; width: 100%;'>/rawData/...</div></th>";

    html+="<th><hr><p><b>Configuraci";html.concat((char)243);html+="n Usuario</b></p>";
    html+=" <form method='post' action='/postUser'>";
    html+="<div style='width: 40%;' align='right'><label for='Usuario'>Usuario*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' maxlength='20' minlength='1' required name='Usuario' value='";
    html+=miUsuario->user;
    html+="'></div>";
    html+="<div style='width: 40%;' align='right'><label for='NombreApellidos'>Nombre y apellidos:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' maxlength='20' name='NombreApellidos' value='";
    html+=miUsuario->nombreApellidos;
    html+="'></div>";
    html+="<div style='width: 40%;' align='right'><label for='pswActual'>Contrase";html.concat((char)241);html+="a actual:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='text' readonly class='protegido' value='";
    html+=miUsuario->psw;
    html+="' name='pswActual'></div>";
    html+="<hr style='width: 100%; margin-bottom: 10px;'><div style='width: 40%;' align='right'><label for='psw'>Nueva contrase";html.concat((char)241);html+="a*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' minlength='8' required name='psw'></div>";
    html+="<div style='width: 40%;' align='right'><label for='retype'>Retype*:</label></div>";
    html+="<div style='width: 60%;' align='left'><input type='password' maxlength='20' minlength='8' required name='retype'></div>";
    html+="<div style='width: 100%;'><input type='submit' value='Establecer'></div>";
    html+="<div style='color: red; width: 100%;'>";
    if(tipoError == ERROR_RETYPE_USER){
        html+="Retype incorrecto";
    }
    if(tipoError == ERROR_DATA_USER){
        html+="Fallo al establecer los datos";
    }
    html+="</div></form>";
    html+="</th></tr></table><hr>";
    html+="<footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";

    return html;
}