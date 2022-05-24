//https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf

MPU6050 mpu;
#define MPU6050_DMP_FIFO_RATE_DIVISOR 0x02 //Redefinido para quitar carga al mcu (menos paquetes corruptos)

bool dmpReady = false;  //true si el DMP se inicio correctamente
uint8_t devStatus;      //Despues de cada operacion para retorno, 0 si es correcto, sino !=0
uint16_t packetSize;    // DMP packet size esperado (por defecto es 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor medidas
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector

int16_t ax, ay, az;     //RAW
int16_t gx, gy, gz;     //RAW

float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

bool mpuSetup(){
    Wire.begin();
    Wire.setClock(400000);// TWBR = 24
    Serial.println("Intentando conexion I2C con mpu...");
    mpu.initialize();
    Serial.println("Probando si se ha conectado...");
    if(!mpu.testConnection()){
        Serial.println("MPU6050 connection failed");
        return false;
    }
    Serial.println("MPU6050 connection successful");
    Serial.println("Inicializando DMP...");
    devStatus = mpu.dmpInitialize();
    if(devStatus != 0){
        Serial.println("DMP failed");
        return false;
    }
    return true;
}

bool calibrate(bool rutina = true){
    Serial.println("Iniciando rutina de calibracion");
    if(!rutina){
        mpu.setXGyroOffset(220);
        mpu.setYGyroOffset(76);
        mpu.setZGyroOffset(-85);
        mpu.setZAccelOffset(1788);
    }else{

    }
    if(devStatus == 0)mpu.setDMPEnabled(true);
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
    Serial.println("Calibrar finalizado");
    return true;
}
//https://www.i2cdevlib.com/forums/topic/336-dmp-without-interrupts/
//https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_DMP6_ESPWiFi/MPU6050_DMP6_ESPWiFi.ino
bool getDataMPU(){
    if(!dmpReady)return false;//Si no hay dmp inicializado no puedo obtener los datos
    Serial.println("OK dmpReady");
    //AQUI PONER RESETFIFO (siempre que entre aqui que adquiera datos si o si)
    mpu.resetFIFO();
    while (fifoCount < packetSize){
        fifoCount = mpu.getFIFOCount();
    }
    if (fifoCount == 1024){
        Serial.println("resetFIFO Overflow");
        mpu.resetFIFO();//Overflow
    }else{
        if(fifoCount % packetSize != 0){//No asegurada la integridad de los datos (fifo multiplo de packetsize)
            Serial.println("Paquete corrupto");
            //SOLUCIÓN para recuperacion del paquete corrupto: https://github.com/jrowberg/i2cdevlib/issues/479  (El de ZHomeSlice)
            uint8_t fifoTrash[packetSize]; // Trash bin
            mpu.getFIFOBytes(fifoTrash, (fifoCount % packetSize) + packetSize );//Eliminar el paquete parcial y salvar el otro
            fifoCount -= ((fifoCount % packetSize) + packetSize);//Actualizar fifo
            fifoCount = mpu.getFIFOCount();//Corroborar si se ha recuperado
            if ((fifoCount % packetSize) == 0) { 
                Serial.println("Recuperacion conseguida");
            }else{
                Serial.println("Error en recuperacion");
                return false;//El paquete sigue corrupto->no recuperado
            }
        }else{
            fifoCount = mpu.getFIFOCount();
        }
        mpu.getFIFOBytes(fifoBuffer, packetSize); 
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        mpu.getAcceleration(&ax, &ay, &az);
        mpu.getRotation(&gx, &gy, &gz);
    }
    return true;
}

void mostrarDatosYPR(){
    Serial.print("ypr\t");
    Serial.print(ypr[0]*180/PI);
    Serial.print("\t");
    Serial.print(ypr[1]*180/PI);
    Serial.print("\t");
    Serial.print(ypr[2]*180/PI);
    Serial.println();
}

void mostrarDatosEuler(){
    Serial.print("euler\t");
    Serial.print((euler[0] * 180/PI));
    Serial.print("\t");
    Serial.print(euler[1] * 180/PI);
    Serial.print("\t");
    Serial.println(euler[2] * 180/PI);
}

void printRAW()
{
   Serial.print("a[x y z] g[x y z]:\t");
   Serial.print(ax); Serial.print("\t");
   Serial.print(ay); Serial.print("\t");
   Serial.print(az); Serial.print("\t");
   Serial.print(gx); Serial.print("\t");
   Serial.print(gy); Serial.print("\t");
   Serial.println(gz);
}