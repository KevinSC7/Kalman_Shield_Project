//https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf

void MPU6050_iniciar() {
  Wire.beginTransmission(MPU6050_adress);
  Wire.write(0x6B);                        //Registro 6B hex para configuración
  Wire.write(0x00);                        //00000000 para activar giroscopio (Internal 8Mhz osc) sin PLL
  Wire.endTransmission();
  Wire.beginTransmission(MPU6050_adress);
  Wire.write(0x1B);                        //Register 1B hex (Configuracion giroscopio)
  Wire.write(0x08);                        //Giroscopio a 500dps (full scale)(000 01 000 -> 0x08)
  Wire.endTransmission();
  Wire.beginTransmission(MPU6050_adress);
  Wire.write(0x1C);                        //Registro de activacion del acelerometro
  Wire.write(0x10);                        //Acelerómetro a  +/- 8g (full scale range) (000 10 000 -> 0x10)
  Wire.endTransmission();

  Wire.beginTransmission(MPU6050_adress);
  Wire.write(0x1B);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_adress, 1);
  while (Wire.available() < 1);//No avanza hasta cumplir lo anterior

  // Activar y configurar filtro pasa bajos LPF que incorpora el sensor
 /* Wire.beginTransmission(MPU6050_adress);
  Wire.write(0x1A);//Esto es para el filtro pasa bajo -> no quiero
  Wire.write(0x04);//Esta es con filtro la conf 4, si quiero establecer sin DLPF es la 0x00
  Wire.endTransmission();*/
}

void MPU6050_leer() {
  // Los datos del giroscopio y el acelerómetro se encuentran de la dirección 3B a la 14
  Wire.beginTransmission(MPU6050_adress);       // Empezamos comunicación
  Wire.write(0x3B);                             // Pedir el registro 0x3B (AcX)
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_adress, 14);         // Solicitar un total de 14 registros
  while (Wire.available() < 14);                // Esperamos hasta recibir los 14 bytes

  ax = (int16_t)(Wire.read() << 8 | Wire.read());          // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  ay = (int16_t)(Wire.read() << 8 | Wire.read());          // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  az = (int16_t)(Wire.read() << 8 | Wire.read());          // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  temperature = (int16_t)(Wire.read() << 8 | Wire.read()); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  gx = (int16_t)(Wire.read() << 8 | Wire.read());          // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gy = (int16_t)(Wire.read() << 8 | Wire.read());          // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gz = (int16_t)(Wire.read() << 8 | Wire.read());          // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  //Tengo que castear de uint16_t (es unsigned) a int16_t (valores negativos)
}

void MPU6050_procesado1(){
  //Conf +-8g -> 1g=4096
  accel_z = az/4096;//az tendra el valor de la gravedad 9.8m/s2 = 1g = 4096
  accel_x = ax/4096;//Valores cercanos a 0
  accel_y = ay/4096;//Valores cercanos a 0
  giro_x=gx/65.5;//º/s
  giro_y=gy/65.5;//º/s
  giro_z=gz/65.5;//º/s
  angulo_pitch += giro_x * tiempo_ejecucion / 1000;
  angulo_roll  += giro_y * tiempo_ejecucion / 1000;
  angulo_pitch += angulo_roll  * sin((gz) * tiempo_ejecucion  * 0.000000266);
  angulo_roll  -= angulo_pitch * sin((gz) * tiempo_ejecucion  * 0.000000266);
}

void mostrar(){
  Serial.println("Valores raw:");
  Serial.println("raw_ax\t\traw_ay\t\traw_az\t\traw_gx\t\traw_gy\t\traw_gz");
  Serial.print(ax);
	Serial.print("\t\t");
	Serial.print(ay, BIN);
	Serial.print("\t\t");
	Serial.print(az);
	Serial.print("\t\t");
	Serial.print(gx);
	Serial.print("\t\t");
	Serial.print(gy);
	Serial.print("\t\t");
	Serial.println(gz);
  Serial.println();

  Serial.println("Aceleraciones:");
  Serial.println("accel_x\taccel_y\taccel_z");
  Serial.print(accel_x);
	Serial.print("\t");
	Serial.print(accel_y);
	Serial.print("\t");
	Serial.println(accel_z);
  Serial.println();

  Serial.println("Velocidades º/s giroscopio");
  Serial.println("giro_x\tgiro_y\tgiro_z");
  Serial.print(giro_x);
	Serial.print("\t");
	Serial.print(giro_y);
	Serial.print("\t");
	Serial.println(giro_z);
  Serial.println();
}