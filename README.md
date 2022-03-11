# Proyecto KALMAN con IMU y GPS

## DESCRIPCIÓN DEL PROYECTO

### OBJETIVO
-Desarrollo de un dispositivo configurable por interfaz web compuesto por un sensor 
inercial, GPS y un microcontrolador con ciertas herramientas matemáticas integradas
que funciona como un shield para otros proyectos.

-Principalmente orientado a el posicionamiento preciso de vehículos (tracking) y 
proyectos de estabilización.

-También se podría aplicar a otros proyectos según necesidades de acuerdo a su 
versatilidad y escalabilidad

### RAZON DE SER

-Este dispositivo, basado en el ESP8266-12, funciona como un shield para otros 
microcontroladores como Arduino, FPGAs...entre otros, el cual se adapta a, y si es 
necesario abstrae al usuario, ciertas necesidades para los diversos proyectos que se 
pudieran acaecer relacionados con el tracking o que tengan como necesidad aplicar 
filtros de Kalman o que necesitaran hacer uso de otras herramientas matemáticas 
(integración por Simpson 1/3, entre otros)

-Permite implementar estos componentes ahorrando tiempo de desarrollo al no tener 
la necesidad de comprender su funcionamiento interno.

### EJEMPLOS DE APLICACION DEL SHIELD

-Tracking de vehiculos

-Proyectos de estabilización (self-balancing robot, estabilización de una cámara con servomotores...).

-Obtención del estado oculto en sensores poco precisos (medición de voltaje, señal digital...).

-Datos externos que requieran alguna herramienta integrada (Simpson 1/3, Kalman,...) del shield, por ejemplo tratamiento de datos poblacionales que requieran de una regresión lineal.

## MANUAL DE USUARIO

### PRIMEROS PASOS
### MANDAR DATOS POR SERIAL
### CARGAR Y DESCARGAR POR INTERFAZ
### ETC 