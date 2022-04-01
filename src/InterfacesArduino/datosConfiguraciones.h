String getDatosConfiguraciones(){
    String html="<html><meta name='viewport' content='width=320, initial-scale=0.7'><head><title>MIS CONFIGURACIONES</title></head>";
    html+="<style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}input[type=text]{border: 2px solid;}table {width: 100%; border-collapse: separate; border-spacing: 10px;}th {border: 3px solid gray;}";
    html+=".protegido {background-color: #080808; font-size: 10px;}.protegido:hover{background-color: gray;}";
    html+="input[type=submit] {color: white;border: 3px solid #080808; border-radius: 12px;font-size: 15px;}";
    html+=".cancel {background-color: #a50a0a; }.cancel:hover {background-color: #fc0f0f; }";
    html+=".cargar{background-color: #d4bb2a; }.cargar:hover{background-color: #fceb05;}";
    html+=".confirm{background-color: #094709; margin-bottom: 5px; margin-left: 25px; margin-right: 25px;}.confirm:hover{background-color: #229422;}";
    html+=".ajustar {width: 10%;}";
    html+="@media only screen and (orientation:portrait){body {margin-left: 6em;}html { transform: rotate(90deg); transform-origin: right top;width: 100vh; overflow-x: hidden; position: absolute; top: 100%; right: 0;}}";
    html+="</style>";
    html+="<script>function envio(e){let id = e.id;let valor = e.value;";
    html+="document.getElementById('idpulsado').value=id;document.getElementById('botonpulsado').value=valor;}";
    html+="</script>";

    html+="<body><header><h1>MIS CONFIGURACIONES</h1></header><table><tr>";
    html+="<th colspan='3' style='border: 0px;'><form method='post' action='/logout'>";
    html+="<input type='submit' value='Cerrar sesion' style='float: right;' class='confirm'>";
    html+="</form><form action='/getGestion' method='get'><input type='submit' value='Volver a gestion de datos' style='float: left;' class='confirm'></form></th></tr>";

    html+="<form method='post' action='/postDataConfig'>";
    html+="<input type='hidden' name='idpulsado' id='idpulsado' value='' />";
    html+="<input type='hidden' name='botonpulsado' id='botonpulsado' value='' />";
    for(int i=1; i<11; i++){
        String c = getConfig(i);
        Serial.println(c);
        if(c=="")break;
        html+="<tr><th><p>Configuracion ";html.concat(i);html+=":</p><ul align='left'>";
        if(c[0]=='1'){
            html+="<li>Envio sincronizado: ";html+=c.substring(6); html+="ms</li>";
        }else{
            html+="<li>Envio asicr";html.concat((char)243);html+="nico</li>";
        }
        if(c[1]=='1')html+="<li>Datos del IMU en bruto</li>";
        if(c[2]=='1')html+="<li>Datos del GPS en bruto</li>";
        if(c[3]=='1')html+="<li>Datos IMU procesados Kalman</li>";
        if(c[4]=='1')html+="<li>Datos fusionados IMU+GPS</li></ul>";
        html+="</th><th class='ajustar'><input type='submit' class='cargar' id='";html.concat(i); html+="' onclick='envio(this)' value='Cargar'>";
        html+="</th><th class='ajustar'><input type='submit' class='cancel' id='";html.concat(i); html+="' onclick='envio(this)' value='Eliminar'></th></tr>";
    }
    html+="<tr><th class='ajustar' colspan='3'><input type='submit' name='et' class='cancel' value='Eliminar todas'></th></tr>";
    html+="</form></table><hr><footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";
    
    return html;
}