String getGestion(){
    String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'>";
    html+="<head><title>GESTION DE DATOS</title></head>";
    html+="<style>";
    html+="header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}";
    html+="table {width: 20%; border: 2px; margin: auto;}";
    html+=".borde {border: 3px solid #dddddd;}";
    html+="label, input[type=radio], input[type=checkbox] { float: left; text-align: left;}";
    html+="p {font-size: 20px;}";
    html+="input[type=number] { float: left;border: 2px solid;margin-left: 20px;}";
    html+=".protegido {background-color: #080808; font-size: 10px;}";
    html+=".protegido:hover{background-color: gray;}";
    html+=".botonRojo {background-color: #a50a0a; color: white;border: 3px solid #080808;border-radius: 12px; height: 30px; font-size: 15px;}";
    html+=".botonRojo:hover {background-color: #fc0f0f;}";
    html+=".botonVerde {background-color: #094709; color: white;border: 3px solid #080808;border-radius: 12px; height: 30px; font-size: 15px;}";
    html+=".botonVerde:hover {background-color: #229422}";
    html+="</style>";
    html+="<script>function showsText(object){if(object.value == 1){";
    html+="document.getElementById('setTime').hidden = false;}else{";
    html+="document.getElementById('setTime').hidden = true;}}";
    html+="</script>";

    html+="<body><header><h1>Gesti"; html.concat((char)243); html+="n de datos</h1></header>";
    html+="<table><tr>";

    html+="<th><form method='get' action='/getUserGestion'>";
    html+="<input type='submit' class='botonVerde' value='Gesti"; html.concat((char)243); html+="n Usuario' style='width: 150px;'>";
    html+="</form></th>";

    html+="<th><form method='get' action='/getApGestion'>";
    html+="<input type='submit' class='botonVerde' value='Gesti"; html.concat((char)243); html+="n AP' style='width: 150px;'>";
    html+="</form></th>";

    html+="<th><form method='post' action='/logout'>";
    html+="<input type='submit' class='botonVerde' value='Cerrar sesi"; html.concat((char)243); html+="n' style='width: 150px;'>";
    html+="</form></th>";

    html+="</tr>";
    html+="<tr style='vertical-align: top;''>";
    html+="<th colspan='3' class='borde'><hr>";
    html+="<p align='center'><b>Configuraci"; html.concat((char)243); html+="n herramientas</b></p><hr>";

    html+="<div style='width: 200px; margin: auto;'>";
    html+="<p style='font-size: 15px; text-align: left;'>ID configuraci"; html.concat((char)243); html+="n:";
    html+=(String)idConf;
    html+="</p>";
    html+="<form method='post' action='/postQuitarCargada'>";
    html+="<input type='submit' class='botonVerde' style='width: 200px;' value='Nueva'></form>";
    html+="</div><hr>";

    html+="<div style='margin: auto; margin-left: 30%;'>";
    html+="<form method='post' action='/postGestion'>";
   
    html+="<label><input type='radio' onchange='showsText(this);' name='tipoEnvio' id='asin' value='0'";
    if(!actualConf[0])html+="checked";
    html+=">";
    html+="Env"; html.concat((char)237); html+="o asincr"; html.concat((char)243); html+="nico de datos</label>";
    html+="<br></br>";
    
    html+="<label><input type='radio' onchange='showsText(this);' name='tipoEnvio' id='sin' value='1'";
    if(actualConf[0])html+="checked";
    html+=">";
    html+="Env"; html.concat((char)237); html+="o s"; html.concat((char)237); html+="ncrono de datos</label>";
    html+="<br></br>";
    if(actualConf[0]){
        html+="<input type='number' name='setTime' id='setTime' placeholder='Tiempo en milisegundos (min:100ms=0.1s)' value='";
        html.concat(tiempo_ms);
        html += "' >";
    }else{
        html+="<input type='number' name='setTime' id='setTime' placeholder='Tiempo en milisegundos (min:100ms=0.1s)' hidden >";
    }
    html+="<br></br>";
    
    html+="<label><input type='checkbox' name='ib' id='ib' ";
    if(actualConf[1])html+="checked";
    html+=">";
    html+="Datos IMU en bruto</label>";
    html+="<br></br>";
    
    html+="<label><input type='checkbox' name='gb' id='gb' ";
    if(actualConf[2])html+="checked";
    html+=">";
    html+="Datos GPS en bruto</label>";
    html+="<br></br>";
    
    html+="<label><input type='checkbox' name='ip' id='ip' ";
    if(actualConf[3])html+="checked";
    html+=">";
    html+="Datos del IMU procesados con Kalman</label>";
    html+="<br></br>";
    
    html+="<label><input type='checkbox' name='fus' id='fus' ";
    if(actualConf[4])html+="checked";
    html+=">";
    html+="Fusion datos IMU+GPS con Kalman</label>";
    html+="<br></br></div><hr>";
    
    if(ejecutando){
        html+="<input type='submit' class='botonRojo' name='accion' value='Stop' style='width: 200px; float: left;'>";
    }else{
        html+="<input type='submit' class='botonVerde' name='accion' value='Ejecutar' style='width: 200px; float: left;'>";
    }
    
    if(idConf > 0){
        html+="<input type='submit' class='botonVerde' style='width: 200px; float: right;' name='accion' value='Actualizar'><br></br>";
    }else{
        html+="<input type='submit' class='botonVerde' style='width: 200px; float: right;' name='accion' value='Guardar'><br></br>";
    }
    
    html+="<input type='submit' class='botonVerde' style='width: 100%; margin-top: 4px;' name='accion' value='Configuraciones'><br></br>";
    html+="<hr><div style='color: red; width: 100%; white-space: normal; margin-top: 1em;'>";
    if(tipoErrorGestion == ERROR_CONF_ADD){
        html+="No se pudo guardar. Máximo de configuraciones: 10";
    }
    else if(tipoErrorGestion == ERROR_NO_SELECT_CHECKBOX){
        html+="Escoja min"; html.concat((char)237); html+="mo una herramienta(checkbox).";
    }
    else if(tipoErrorGestion == ERROR_NO_UPDATE){
        html+="No se actualiz";html.concat((char)243);html+=". Problema interno / No permitido.";
    }
    html+="</div>";
    html+="</form></th></tr></table><hr>";
    
    html+="<footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";

    return html;
}