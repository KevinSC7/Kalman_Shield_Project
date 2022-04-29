String getUserGestion(){
    String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'>";
    html+="<head><title>GESTION DEL USUARIO</title></head>";
    html+="<style>";
    html+="header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}";
    html+="table {width: 30%; border: 2px; margin: auto;}";
    html+=".borde {border: 3px solid #dddddd;}";
    html+="input[type=text],input[type=password]{border: 2px solid; overflow: hidden; margin-right: 10px; max-width: 40%; float: right;}";
    html+="label{float: left;}";
    html+="p {font-size: 20px;}";
    html+=".protegido {background-color: #080808; font-size: 10px;}";
    html+=".protegido:hover{background-color: gray;}",
    html+=".botonVerde {background-color: #094709; color: white;border: 3px solid #080808;border-radius: 12px; height: 30px; font-size: 15px;}";
    html+=".botonVerde:hover {background-color: #229422}";
    //Quito @media
    html+="</style>";

    html+="<body><header><h1>Gesti"; html.concat((char)243); html+="n del Usuario</h1></header>";
    html+="<table><tr>";
    html+="<th><form method='get' action='/getGestion'>";
    html+="<input type='submit' class='botonVerde' value='Volver' style='width: 150px;float: left;'>";
    html+="</form></th>";
    html+="<th><form method='post' action='/logout'>";
    html+="<input type='submit' class='botonVerde' value='Cerrar sesi"; html.concat((char)243); html+="n' style='width: 150px;float: right;'>";
    html+="</form></th>";
    
    html+="</tr><tr style='vertical-align: top;'>";
    html+="<th colspan='2' class='borde'><hr><p><b>Configuraci"; html.concat((char)243); html+="n Usuario</b></p>";
    html+="<form method='post' action='/postUser'>";
    html+="<label style='float: left;' for='Usuario'>Usuario*:</label>";
    html+="<input style='float: right;' type='text' maxlength='20' required name='Usuario' value='";
    html+=miUsuario->user;
    html+="'>";
    html+="<br></br>";
    html+="<label style='float: left;' for='NombreApellidos'>Propietario:</label>";
    html+="<input style='float: right;' type='text' maxlength='20' name='NombreApellidos' value='";
    html+=miUsuario->nombreApellidos;
    html+="'>";
    html+="<br></br>";
    html+="<label style='float: left;' for='actualpsw'>Contrase"; html.concat((char)241); html+="a actual:</label>";
    html+="<input style='float: right;' class='protegido' type='text' name='actualpsw' readonly value='";
    html+=miUsuario->psw;
    html+="'>";
    html+="<br></br>";
    html+="<hr>";
    html+="<label style='float: left;' for='psw'>Nueva contrase"; html.concat((char)241); html+="a*:</label>";
    html+="<input style='float: right;' type='password' maxlength='20' minlength='8' required name='psw'>";
    html+="<br></br>";
    html+="<label for='retype'>Retype*:</label>";
    html+="<input type='password' maxlength='20' minlength='8' name='retype' required>";
    html+="<br></br>";
    html+="<input type='submit' class='botonVerde' value='Establecer'>";
    html+="<hr><div style='color: red; width: 100%; margin-top: 1em;'>";
    if(tipoErrorGestion == ERROR_RETYPE_USER){
        html+="Retype incorrecto";
    }
    else if(tipoErrorGestion == ERROR_DATA_USER){
        html+="Fallo al establecer los datos";
    }
    html+="</div>";
    html+="</form>";
    html+="</th></table><hr>";

    html+="<footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";

    return html;
}