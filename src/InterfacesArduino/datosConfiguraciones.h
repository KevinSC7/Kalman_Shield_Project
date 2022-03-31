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
    html+="<body><header><h1>MIS CONFIGURACIONES</h1></header><table><tr>";
    html+="<th colspan='3' style='border: 0px;'><form method='post' action='/logout'>";
    html+="<input type='submit' value='Cerrar sesion' style='float: right;' class='confirm'>";
    html+="<input type='submit' value='Volver a gestion de datos' style='float: left;' class='confirm'></form></th></tr>";

    html+="<form method='POST' action='/postDataConfig'>";
    html+="<tr><th><p>Configuracion 1:</p><ul align='left'>";
    html+="<li>Datos en IMU en bruto</li><li>Datos IMU procesados Kalman</li><li>Datos fusionados IMU+GPS</li></ul>";
    html+="</th><th class='ajustar'><input type='submit' class='cargar' value='Cargar'>";
    html+="</th><th class='ajustar'><input type='submit' class='cancel' value='Eliminar'></th></tr>";
    
    html+="</form></table><hr><footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";
    
    return html;
}