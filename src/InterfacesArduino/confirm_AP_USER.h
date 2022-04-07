
String getConfirm(String datos){
    String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'> <head><title>CONFIRMAR</title>";
    html+="</head><style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}input[type=text],input[type=password]{border: 2px solid;}";
    html+="table {width: 100%; border-collapse: separate; border-spacing: 10px;}.protegido {background-color: #080808; font-size: 10px;}";
    html+=".protegido:hover{background-color: gray;}input[type=submit] {color: white;border: 3px solid #080808;border-radius: 12px;";
    html+="font-size: 15px; height: 30px; margin-top: 20px;}.cancel {background-color: #a50a0a; }";
    html+=".cancel:hover {background-color: #fc0f0f; }.confirm{background-color: #094709; }";
    html+=".confirm:hover{background-color: #229422;}";
    html+="@media only screen and (orientation:portrait){body {margin-left: 6em;}html { transform: rotate(90deg); transform-origin: right top;width: 100vh; overflow-x: hidden; position: absolute; top: 100%; right: 0;}}";
    html+="</style>";
    
    if(numBloques(datos, '/') == 4){       //Son los datos del usuario
        html+="<body><header><h1>CONFIRMAR</h1></header><form method='POST' action='/postConfirmUser'>";

        html+="<table><tr><th colspan='3'>";
        html+=buscarBloque(datos, '/', 1);//Comentario (t)
        html+="</th></tr>";
        html+="<tr><th style='text-align: right; width: 50%;'><label for='nuser'>Nuevo nombre de usuario: </label></th>";
        html+="<th style='text-align: left; line-height: 0%;'><input type='text' name='nuser' value='";html+=buscarBloque(datos, '/', 2);//nombre user (d1)
        html+="' readonly></th></tr>";

        html+="<tr><th style='text-align: right; width: 50%;'><label for='nya'>Nuevos datos personales del usuario: </label></th>";
        html+="<th style='text-align: left; line-height: 0%;'><input type='text' name='nya' value='";html+=buscarBloque(datos, '/', 3);//NyA (d2)
        html+="' readonly></th></tr>";

        html+="<tr><th style='text-align: right;'><label for='psw'>Nueva contrase";html.concat((char)241);html+="a: </label></th>";
        html+="<th style='text-align: left; line-height: 0%;'><input type='text' name='psw' class='protegido' value='";
        html+=buscarBloque(datos, '/', 4);//psw (d3)
        html+="' readonly><br><br></th></tr>";
    }else if(numBloques(datos, '/') == 3){ //Son los datos del AP
        html+="<body><header><h1>CONFIRMAR</h1></header><form method='POST' action='/postConfirmSSID'>";

        html+="<table><tr><th colspan='3'>";
        html+=buscarBloque(datos, '/', 1);//Comentario (t)
        html+="</th></tr>";
        html+="<tr><th style='text-align: right; width: 50%;'><label for='ssid'>Nueva SSID: </label></th>";
        html+="<th style='text-align: left; line-height: 0%;'><input type='text' name='ssid' value='";html+=buscarBloque(datos, '/', 2);//SSID (d1)
        html+="' readonly></th></tr>";
        html+="<tr><th style='text-align: right;'><label for='psw'>Nueva contrase";html.concat((char)241);html+="a: </label></th>";
        html+="<th style='text-align: left; line-height: 0%;'><input type='text' name='psw' class='protegido' value='";
        html+=buscarBloque(datos, '/', 3);//psw (d2)
        html+="' readonly><br><br></th></tr>";

    }else{//Los datos enviados no coinciden con los que deberian, seguridad comprometida (ir a login)
        autorizacion = false;
        html+="<body><header><h1>ERROR CONFIRMAR DATOS</h1></header><form method='POST' action='/getLogin'>";
    }
    
    html+="<tr><th colspan='3'><input type='submit' name='env' value='Confirmar' class='confirm'><input type='submit' name='env' value='Cancelar' class='cancel'></th></tr></table>";

    html+="</form><br><footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";
    
    return html;
    return html;
}