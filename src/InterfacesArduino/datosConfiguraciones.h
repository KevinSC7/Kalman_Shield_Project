
String principio(){
    String html="<html><meta name='viewport' content='width=320, initial-scale=0.7, user-scalable=0'><head><title>MIS CONFIGURACIONES</title></head>";
    html+="<style>header, footer { padding: 0em;color: white;background-color: Black;text-align: center;}input[type=text]{border: 2px solid;}table {width: 100%; border-collapse: separate; border-spacing: 10px;}th {border: 3px solid gray;}";
    html+=".protegido {background-color: #080808; font-size: 10px;}.protegido:hover{background-color: gray;}";
    html+="input[type=submit] {color: white;border: 3px solid #080808; border-radius: 12px;font-size: 15px;}";
    html+=".cancel {background-color: #a50a0a; }.cancel:hover {background-color: #fc0f0f; }";
    html+=".cargar{background-color: #d4bb2a; }.cargar:hover{background-color: #fceb05;}";
    html+=".confirm{background-color: #094709; margin-bottom: 5px; margin-left: 25px; margin-right: 25px;}.confirm:hover{background-color: #229422;}";
    html+=".ajustar {width: 10%;}";
    
    html+="@media only screen and (orientation:portrait){";
    html+="body {margin-left: 6em;}";
    html+="html { transform: rotate(90deg); transform-origin: right top;width: 100vh;overflow-x: hidden; position: absolute; top: 100%; right: 0;}}";
    
    html+="</style>";
    html+="<script>function envio(e){let id = e.id;let valor = e.value;";
    html+="document.getElementById('idpulsado').value=id;document.getElementById('botonpulsado').value=valor;}";
    html+="</script>";

    html+="<body><header><h1>MIS CONFIGURACIONES</h1></header><table><tr>";
    html+="<th colspan='3' style='border: 0px;'><form method='post' action='/logout'>";
    html+="<input type='submit' value='Cerrar sesion' style='float: right; width: 150px;' class='confirm'>";
    html+="</form><form method='get' action='/getGestion'><input type='submit' value='Volver' style='float: left; width: 150px;' class='confirm'></form></th></tr></table>";

    html+="<form method='post' action='/postConfig'>";
    html+="<div style='overflow-y: auto; max-height: 30vh; white-space: nowrap;'>";
    html+="<table>";
    html+="<input type='hidden' name='idpulsado' id='idpulsado' value='' />";
    html+="<input type='hidden' name='botonpulsado' id='botonpulsado' value='' />";
    
    return html;
}

String losFor(String c, int i){   
    String html="";
    if(c=="")return "";
        html+="<tr><th><p>Configuracion ";html+=(String)i;html+=":</p><ul align='left'>";
        if(c[0]=='1'){
            html+="<li>Envio sincronizado: ";html+=c.substring(6); html+="ms</li>";
        }else{
            html+="<li>Envio asicr";html.concat((char)243);html+="nico</li>";
        }
        if(c[1]=='1')html+="<li>Datos del IMU en bruto</li>";
        if(c[2]=='1')html+="<li>Datos del GPS en bruto</li>";
        if(c[3]=='1')html+="<li>Datos IMU procesados Kalman</li>";
        if(c[4]=='1')html+="<li>Datos fusionados IMU+GPS</li>";
        html+="</ul></th><th class='ajustar'><input type='submit' class='cargar' id='";html+=(String)i; html+="' onclick='envio(this)' value='Cargar' />";
        html+="</th><th class='ajustar'><input type='submit' class='cancel' id='";html+=(String)i; html+="' onclick='envio(this)' value='Eliminar' /></th></tr>";
    
    return html;
}

String final(){
    String html="</table>";
    html+="</div>";
    html+="<table><tr><th class='ajustar' colspan='3'><input type='submit' name='et' class='cancel' value='Eliminar todas'></th></tr></table>";
    html+="</form>";
    
    html+="<hr><footer>Shield Kalman ESP8266-12e de KevinSC<br>Copyright &copy; Krobotics</footer></body></html>";
    
    return html;
}