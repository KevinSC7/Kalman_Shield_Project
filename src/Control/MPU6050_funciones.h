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

bool calibrate(){
    Serial.println("Iniciando rutina de calibracion");
    /*
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);
    */
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setZAccelOffset(0);
    if(devStatus == 0){
        mpu.CalibrateAccel(10);
        mpu.CalibrateGyro(10);
        Serial.println("Calibrate readings: 1000 loops");
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);
    }
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
    Serial.println("Calibrar finalizado");
    return true;
}
//https://www.i2cdevlib.com/forums/topic/336-dmp-without-interrupts/
//https://github.com/ElectronicCats/mpu6050/blob/master/examples/MPU6050_DMP6_ESPWiFi/MPU6050_DMP6_ESPWiFi.ino
bool getDataMPU(){
    if(!dmpReady)return false;              //Si no hay dmp inicializado no puedo obtener los datos
    mpu.resetFIFO();                        //AQUI PONER RESETFIFO (siempre que entre aqui que adquiera datos si o si)
    while (fifoCount < packetSize){         //Hasta que no obtenga los paquetes necesarios
        fifoCount = mpu.getFIFOCount();
    }
    if (fifoCount == 1024){                 //Overflow buffer
        Serial.println("ERROR: Overflow");  //Hay que hacer resetFIFO
        return false;                       //Al volver a llamar a getDataMPU hara reset
    }else if(fifoCount == 0){               //No data
        return false;                       //Repetir captura de paquetes
    }else{
        if(fifoCount % packetSize != 0){    //No asegurada la integridad de los datos (fifo multiplo de packetsize)
            Serial.println("ERROR: Paquete corrupto");
            //SOLUCIÓN para recuperacion del paquete corrupto: https://github.com/jrowberg/i2cdevlib/issues/479  (El de ZHomeSlice)
            uint8_t fifoTrash[packetSize];  //Trash bin
            mpu.getFIFOBytes(fifoTrash, (fifoCount % packetSize) + packetSize );//Eliminar el paquete parcial y salvar el otro
            fifoCount -= ((fifoCount % packetSize) + packetSize);               //Actualizar fifo
            return false;                   //Volver a adquirir datos con la fifo corregida
            //Sino se hubiese corregido daria siempre corrupto (no multiplo) por el desfase.
            //Mientras he corregido puede haber cargado datos en fifo, no corruptos pero tampoco validos, repetir
        }
        fifoCount = mpu.getFIFOCount();
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

void mostrarDatosAccelWithG(){
    Serial.print("xyz\t");
    Serial.print(aa.x * 9.81 / 8192);//Lo paso a 1g=8192, luego 1g=9.81m/s2
    Serial.print("\t");
    Serial.print(aa.y * 9.81 / 8192);
    Serial.print("\t");
    Serial.print(aa.z * 9.81 / 8192);
    Serial.println();
}

void mostrarDatosAccelLineal(){
    Serial.print("xyz\t");
    Serial.print(aaReal.x * 9.81 / 8192);
    Serial.print("\t");
    Serial.print(aaReal.y * 9.81 / 8192);
    Serial.print("\t");
    Serial.print(aaReal.z * 9.81 / 8192);
    Serial.println();
}

void mostrarDatosWorldAccelLineal(){//
    Serial.print("xyz\t");
    Serial.print(aaWorld.x * 9.81 / 8192);
    Serial.print("\t");
    Serial.print(aaWorld.y * 9.81 / 8192);
    Serial.print("\t");
    Serial.print(aaWorld.z * 9.81 / 8192);
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