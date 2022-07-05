//https://www.delftstack.com/es/howto/cpp/matrix-multiplication-in-cpp/#:~:text=La%20funci%C3%B3n%20multiplyMatrix%20implementa%20un,las%20segundas%20columnas%20del%20array.
class operacionesMatrices{
    public:
        double **matrizA;
        double **matrizB;
        int filasA, filasB, colsA, colsB;

        operacionesMatrices(double **_matrizA, double **_matrizB, int _filasA, int _colsA, int _filasB, int _colsB){
            matrizA = _matrizA;
            matrizB = _matrizB;
            filasA = _filasA;
            filasB = _filasB;
            colsA = _colsA;
            colsB = _colsB;
        }
        void setNuevas(double **_matrizA, double **_matrizB, int _filasA, int _colsA, int _filasB, int _colsB){
            matrizA = _matrizA;
            matrizB = _matrizB;
            filasA = _filasA;
            filasB = _filasB;
            colsA = _colsA;
            colsB = _colsB;
        }

        void mostrarMatrizA(){
            for(int i = 0; i < filasA; i++){
                for(int j = 0; j < colsA; j++){
                    Serial.print(matrizA[i][j]);Serial.print(" ");
                }
                Serial.println();
            }
        }
        void mostrarMatrizB(){
            for(int i = 0; i < filasB; i++){
                for(int j = 0; j < colsB; j++){
                    Serial.print(matrizB[i][j]);Serial.print(" ");
                }
                Serial.println();
            }
        }

        double **calcularProducto(){
            if (colsA != filasB)return nullptr;
            double **producto = matrizProductoCreate(filasA, colsB);

            for(int i = 0; i < filasA; i++) {
                for(int j = 0; j < colsB; j++) {
                    for(int k = 0; k < colsA; k++) {
                        producto[i][j] += matrizA[i][k] * matrizB[k][j];
                    }
                }
            }
            return producto;
        }
        double **calcularSuma(){
            double **suma = matrizProductoCreate(filasA, colsA);//Suma matrices mismo tamaño
            for(int i=0;i<filasA;i++){
                for(int j=0;j<colsA;j++){
                        suma[i][j]=matrizA[i][j]+matrizB[i][j];//la operacion 
                }
            }
            return suma;
        }
        double **calcularResta(){
            double **resta = matrizProductoCreate(filasA, colsA);//Suma matrices mismo tamaño
            for(int i=0;i<filasA;i++){
                for(int j=0;j<colsA;j++){
                        resta[i][j]=matrizA[i][j]-matrizB[i][j];//la operacion 
                }
            }
            return resta;
        }
        double **traspuesta(double **matriz, int f, int c){
            double **t = matrizProductoCreate(c, f);//filas x col -> col x filas
            for(int i = 0; i < f; i++){
                for(int j = 0; j < c; j++){
                    t[j][i]=matriz[i][j];
                }
            }
            return t;
        }
        void actualizarMatriz(double **A, double **B, int filas, int cols){
            for(int i = 0; i < filas; i++){
                for(int j = 0; j < cols; j++){
                    A[i][j]=B[i][j];
                }
            }
        }
        double **matrizProductoCreate(int row, int col){
            double **matrix = new double*[row];
            for(int i = 0; i < row; ++i) {
                matrix[i] = new double[col]{0.0};
            }
            return matrix;
        }
        bool deallocateMatrix(double **matrix, int row){//Para borrar matriz
            for(int i = 0; i < row; ++i) {
                delete matrix[i];
            }
            delete [] matrix;
            return true;
        }
        double **crearZeroMatriz(int cols, int filas){
            double **matrix = new double*[filas];
            for(int i = 0; i < filas; ++i) {
                matrix[i] = new double[cols]{0};
            }
            return matrix;
        }
        double **convertirIdentidadMatriz(double **M, int cols, int filas){
            for(int i = 0; i < filas; ++i) {
                for(int j=0; j < cols; j++){
                    if(i == j)M[i][j]=1;
                }
            }
            return M;
        }
        int getFilasProd(){
            return filasA;
        }
        int getColsProd(){
            return colsB;
        }
};

double *F[6]=
{
(double[]){1, 0, 0, 1, 0, 0},
(double[]){0, 1, 0, 0, 1, 0},
(double[]){0, 0, 1, 0, 0, 1},
(double[]){0, 0, 0, 1, 0, 0},
(double[]){0, 0, 0, 0, 1, 0},
(double[]){0, 0, 0, 0, 0, 1},
};

double *G[6]=
{
(double[]){1, 0, 0},
(double[]){0, 1, 0},
(double[]){0, 0, 1},
(double[]){1, 0, 0},
(double[]){0, 1, 0},
(double[]){0, 0, 1},
};

double *Q[6]=
{
(double[]){1, 0, 0, 1, 0, 0},
(double[]){0, 1, 0, 0, 1, 0},
(double[]){0, 0, 1, 0, 0, 1},
(double[]){1, 0, 0, 1, 0, 0},
(double[]){0, 1, 0, 0, 1, 0},
(double[]){0, 0, 1, 0, 0, 1},
};
double *inputF[6]=//Es X0,0 (vector posicion xyz y velocidad xyz)
{
    (double[]){0},
    (double[]){0},
    (double[]){0},
    (double[]){0},
    (double[]){0},
    (double[]){0}
};
double *inputA[3]=//Es U0,0 (input)
{
    (double[]){0},
    (double[]){0},
    (double[]){0}
};

double *P[6]=
{
(double[]){500, 0, 0, 0, 0, 0},
(double[]){0, 500, 0, 0, 0, 0},
(double[]){0, 0, 500, 0, 0, 0},
(double[]){0, 0, 0, 500, 0, 0},
(double[]){0, 0, 0, 0, 500, 0},
(double[]){0, 0, 0, 0, 0, 500},
};

double *medidas[6]=//Es Z (solo medidas de pos y vel, si quiero tambien accel -> tamaño 9)
{
    (double[]){0},
    (double[]){0},
    (double[]){0},
    (double[]){0},
    (double[]){0},
    (double[]){0}
};

double *R[6]=
{
    (double[]){0, 0, 0, 0, 0, 0},
    (double[]){0, 0, 0, 0, 0, 0},
    (double[]){0, 0, 0, 0, 0, 0},
    (double[]){0, 0, 0, 0, 0, 0},
    (double[]){0, 0, 0, 0, 0, 0},
    (double[]){0, 0, 0, 0, 0, 0}
};

void XnNext(double tiempo, double **Xn, double **Un, double **F, double **G, bool mostrar = false){//Eq. Extrapolacion
    F[0][3]=tiempo;
    F[1][4]=tiempo;
    F[2][5]=tiempo;
    G[0][0]=0.5*tiempo*tiempo;
    G[1][1]=0.5*tiempo*tiempo;
    G[2][2]=0.5*tiempo*tiempo;
    G[3][0]=tiempo;
    G[4][1]=tiempo;
    G[5][2]=tiempo;

    operacionesMatrices pmf = operacionesMatrices(F, Xn, 6, 6, 6, 1);
    double ** resultado1 = pmf.calcularProducto();

    operacionesMatrices pmg = operacionesMatrices(G, Un, 6, 3, 3, 1);
    double **resultado2 = pmg.calcularProducto();

    operacionesMatrices suma = operacionesMatrices(resultado1, resultado2, 6, 1, 6, 1);
    double **resultado3=suma.calcularSuma();

    suma.actualizarMatriz(Xn, resultado3, 6, 1);

    if(mostrar){
        for(int i=0; i<6; i++){
            for(int j=0; j < 1; j++){
                Serial.print(Xn[i][j]);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
    suma.deallocateMatrix(resultado1, 6);
    suma.deallocateMatrix(resultado2, 6);
    suma.deallocateMatrix(resultado3, 6);
}

void PnNext(double tiempo, double varAccel, double **F, double **P, double **Q, bool mostrar = false){//Eq. cov. Extrapolacion
    Q[0][0]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[1][1]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[2][2]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[3][3]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[4][4]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[5][5]=(tiempo*tiempo*tiempo*tiempo)/4;

    Q[0][3]=(tiempo*tiempo*tiempo)/2;
    Q[1][4]=(tiempo*tiempo*tiempo)/2;
    Q[2][5]=(tiempo*tiempo*tiempo)/2;

    Q[3][0]=(tiempo*tiempo*tiempo)/2;
    Q[4][1]=(tiempo*tiempo*tiempo)/2;
    Q[5][2]=(tiempo*tiempo*tiempo)/2;
    for(int i=0; i < 6; i++){
        for(int j=0; j < 6; j++){
            Q[i][j]*=varAccel*varAccel;
        }
    }

    operacionesMatrices pFP = operacionesMatrices(F, P, 6, 6, 6, 6);
    double **FP = pFP.calcularProducto();//F·P
    double **Ft = pFP.traspuesta(F, 6, 6);//Ft: F traspuesta

    operacionesMatrices pFPFt = operacionesMatrices(FP, Ft, 6, 6, 6, 6);
    double **FPFt = pFPFt.calcularProducto();

    operacionesMatrices suma = operacionesMatrices(FPFt, Q, 6, 6, 6, 6);
    double **resultado = suma.calcularSuma();

    suma.actualizarMatriz(P, resultado, 6, 6);

    if(mostrar){
        for(int i=0; i<6; i++){
            for(int j=0; j < 6; j++){
                Serial.print(P[i][j]);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
    suma.deallocateMatrix(FP, 6);
    suma.deallocateMatrix(Ft, 6);
    suma.deallocateMatrix(FPFt, 6);
    suma.deallocateMatrix(resultado, 6);
}

double **setZ(//Eq. medida
    double **Z,
    double velx, double vely, double velz,
    double posx, double posy, double posz,//distancia gps + distancia imu respecto al punto anterior
    bool mostrar = false){
    Z[0][0]=posx;Z[0][1]=posy;Z[0][2]=posz;
    Z[0][3]=velx;Z[0][4]=vely;Z[0][5]=velz;
    if(mostrar){
        for(int i=0; i < 6; i++){
            Serial.println(Z[0][i]);
            Serial.print(" ");
        }
        Serial.println();
    }
    return Z;
}

double **KnNext(//Eq. Kalman ganancia y R de acuerdo a Z
    double **P, double **Z, double **Xn, double **R,
    double varposx, double varposy, double varposz,//varianza de las posiciones por eje
    double varvelx, double varvely, double varvelz,//varianza de las velocidades por eje
    double tiempo, bool mostrar = false){
    R[0][0]=varposx*varposx*tiempo*tiempo;
    R[1][1]=varposy*varposy*tiempo*tiempo;
    R[2][2]=varposz*varposz*tiempo*tiempo;
    R[3][3]=varvelx*varvelx;
    R[4][4]=varvely*varvely;
    R[5][5]=varvelz*varvelz;
    R[0][3]=R[3][0]=varposx*varvelx*tiempo;
    R[1][4]=R[4][1]=varposx*varvelx*tiempo;
    R[2][5]=R[5][2]=varposx*varvelx*tiempo;
    
    operacionesMatrices op = operacionesMatrices(P, Z, 6, 6, 6, 1);
    double **H=op.crearZeroMatriz(6, 6);
    H = op.convertirIdentidadMatriz(H, 6, 6);
    double **K=op.crearZeroMatriz(6, 6);

    operacionesMatrices pHP = operacionesMatrices(H, P, 6, 6, 6, 6);
    double **HP = pHP.calcularProducto();
    double **Ht = pHP.traspuesta(H, 6, 6);

    operacionesMatrices pHPHt = operacionesMatrices(HP, Ht, 6, 6, 6, 6);
    double **HPHt = pHPHt.calcularProducto();

    operacionesMatrices suma = operacionesMatrices(HPHt, R, 6, 6, 6, 6);
    double **r1 = suma.calcularSuma();
    //Codigo ejemplo matrixmath: https://codebender.cc/example/MatrixMath/MatrixMathExample#MatrixMathExample.ino
    //Inversa:
    Matrix.Invert((double*)r1, 6);

    operacionesMatrices pPHt = operacionesMatrices(P, Ht, 6, 6, 6, 6);
    double **PHt = pPHt.calcularProducto();

    operacionesMatrices final = operacionesMatrices(PHt, r1, 6, 6, 6, 6);
    double ** rfinal = final.calcularSuma();
    final.actualizarMatriz(K, rfinal, 6, 6);

    if(mostrar){
        for(int i=0; i<6; i++){
            for(int j=0; j < 6; j++){
                Serial.print(K[i][j]);
                Serial.print(" ");
            }
            Serial.println();
        }
    }

    return K;
}

double **actualizacionEstado(double **K, double **Z, double **H, double **Xn){//Eq. actualizacion de estado
    operacionesMatrices op = operacionesMatrices(H, Xn, 6, 6, 6, 1);
    double **resultado1 = op.calcularProducto();
    operacionesMatrices resta = operacionesMatrices(Z, resultado1, 6, 1, 6, 1);
    double **innovacion = resta.calcularResta();
    operacionesMatrices K_innov = operacionesMatrices(K, innovacion, 6, 6, 6, 1);
    double **resultado2 = K_innov.calcularProducto();
    operacionesMatrices resta2 = operacionesMatrices(Xn, resultado2, 6, 1, 6, 1);
    double **final = resta2.calcularResta();
    resta2.actualizarMatriz(Xn, final, 6, 1);
    resta2.deallocateMatrix(resultado1, 6);
    resta2.deallocateMatrix(resultado2, 6);
    resta2.deallocateMatrix(innovacion, 6);
    resta2.deallocateMatrix(final, 6);
    return Xn;
}

double **actualizacionCovarianza(double **K, double **R, double **H, double **P){//Eq. actualizacion de estado
    double *I[6]=
    {
    (double[]){1, 0, 0, 0, 0, 0},
    (double[]){0, 1, 0, 0, 0, 0},
    (double[]){0, 0, 1, 0, 0, 0},
    (double[]){0, 0, 0, 1, 0, 0},
    (double[]){0, 0, 0, 0, 1, 0},
    (double[]){0, 0, 0, 0, 0, 1},
    };
    operacionesMatrices op1 = operacionesMatrices(K, R, 6, 6, 6, 6);
    double **Ktraspuesta = op1.traspuesta(K, 6, 6);
    double **r1 = op1.calcularProducto();
    operacionesMatrices op2 = operacionesMatrices(r1, Ktraspuesta, 6, 6, 6, 6);
    double **parte1 = op2.calcularProducto();
    operacionesMatrices op3 = operacionesMatrices(K, H, 6, 6, 6, 6);
    double **r2 = op3.calcularProducto();
    operacionesMatrices resta1 = operacionesMatrices(I, r2, 6, 6, 6, 6);
    double **parentesis1 = resta1.calcularResta();
    double **parentesis2 = resta1.traspuesta(parentesis1, 6, 6);
    operacionesMatrices op4 = operacionesMatrices(parentesis1, P, 6, 6, 6, 6);
    double **parte2 = op4.calcularProducto();
    operacionesMatrices op5 = operacionesMatrices(parte2, parentesis2, 6, 6, 6, 6);
    parte2 = op5.calcularProducto();
    operacionesMatrices final = operacionesMatrices(parte2, parte1, 6, 6, 6, 6);
    double **Pactualizada = final.calcularSuma();
    final.actualizarMatriz(P, Pactualizada, 6, 6);
    final.deallocateMatrix(Ktraspuesta, 6);
    final.deallocateMatrix(r1, 6);
    final.deallocateMatrix(parte1, 6);
    final.deallocateMatrix(r2, 6);
    final.deallocateMatrix(parte2, 6);
    final.deallocateMatrix(parentesis1, 6);
    final.deallocateMatrix(parentesis2, 6);
    final.deallocateMatrix(Pactualizada, 6);
    return P;
}

void fusion(double tiempo){
    
}

void actualizarKalman(
    double tiempo, double varAccel,
    double _ax, double _ay, double _az,
    double velx, double vely, double velz,
    double posx, double posy, double posz,//distancia gps + distancia imu respecto al punto anterior
    double varposx, double varposy, double varposz,//varianza de las posiciones por eje
    double varvelx, double varvely, double varvelz//varianza de las velocidades por eje
    ){
    F[0][3]=tiempo;
    F[1][4]=tiempo;
    F[2][5]=tiempo;
    //F hecha
    G[0][0]=0.5*tiempo*tiempo;
    G[1][1]=0.5*tiempo*tiempo;
    G[2][2]=0.5*tiempo*tiempo;

    G[3][0]=tiempo;
    G[4][1]=tiempo;
    G[5][2]=tiempo;
    //G hecha
    //Resultado de inputF*F+inputA*G = posterior (ecuaciones extrapolacion de estado Xn+1)
    double **resultado1;
    operacionesMatrices pmf = operacionesMatrices(F, inputF, 6, 6, 6, 1);
    pmf.mostrarMatrizA();
    Serial.println("x");
    pmf.mostrarMatrizB();
    Serial.println("=");
    resultado1 = pmf.calcularProducto();
    for(int i=0; i < pmf.getFilasProd(); i++){
        for(int j=0; j < pmf.getColsProd(); j++){
            Serial.print(resultado1[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println();

    double **resultado2;
    operacionesMatrices pmg = operacionesMatrices(G, inputA, 6, 3, 3, 1);
    pmg.mostrarMatrizA();
    Serial.println("x");
    pmg.mostrarMatrizB();
    Serial.println("=");
    resultado2 = pmg.calcularProducto();
    for(int i=0; i<pmg.getFilasProd(); i++){
        for(int j=0; j < pmg.getColsProd(); j++){
            Serial.print(resultado2[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }

    Serial.println();
    Serial.println("Resultado: F·Xn + G·u = Xn+1");
    operacionesMatrices suma = operacionesMatrices(resultado1, resultado2, 6, 1, 6, 1);
    double **resultado3=suma.calcularSuma();
    suma.actualizarMatriz(inputF, resultado3, 6, 1);//Copiar resultado3 en inputF
    suma.mostrarMatrizA(); Serial.println("+"); suma.mostrarMatrizB(); Serial.println("=");
    for(int i=0; i<6; i++){
        for(int j=0; j < 1; j++){
            Serial.print(inputF[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    //Fin calculo Xn+1
    Q[0][0]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[1][1]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[2][2]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[3][3]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[4][4]=(tiempo*tiempo*tiempo*tiempo)/4;
    Q[5][5]=(tiempo*tiempo*tiempo*tiempo)/4;

    Q[0][3]=(tiempo*tiempo*tiempo)/2;
    Q[1][4]=(tiempo*tiempo*tiempo)/2;
    Q[2][5]=(tiempo*tiempo*tiempo)/2;

    Q[3][0]=(tiempo*tiempo*tiempo)/2;
    Q[4][1]=(tiempo*tiempo*tiempo)/2;
    Q[5][2]=(tiempo*tiempo*tiempo)/2;
    for(int i=0; i < 6; i++){
        for(int j=0; j < 6; j++){
            Q[i][j]*=varAccel*varAccel;
        }
    }
    //Ya se actualizao inputFn+1 es decir Xn+1, puedo liberar las matrices
    pmf.deallocateMatrix(resultado1, 6);
    pmg.deallocateMatrix(resultado2, 6);
    pmg.deallocateMatrix(resultado3, 6);

    pmf.setNuevas(F, P, 6, 6, 6, 6);
    resultado1 = pmf.calcularProducto();//F·P
    resultado2 = pmf.traspuesta(F, 6, 6);//Ft: F traspuesta
    pmg.setNuevas(resultado1, resultado2, 6, 6, 6, 6 );
    Serial.println("F·P:");
    pmg.mostrarMatrizA();Serial.println();
    Serial.println("F traspuesta:");
    pmg.mostrarMatrizB();Serial.println();
    resultado3 = pmg.calcularProducto();//(F·P)·Ft
    suma.setNuevas(resultado3, Q, 6, 6, 6, 6);
    Serial.println("F·P·Ft:");
    suma.mostrarMatrizA();Serial.println();
    resultado3 = suma.calcularSuma();
    suma.actualizarMatriz(P, resultado3, 6, 6);
    suma.setNuevas(P, nullptr, 6, 6, 6, 6);//Solo para mostrar, sobra
    Serial.println("Pn+1:");
    suma.mostrarMatrizA();Serial.println();

    pmf.deallocateMatrix(resultado1, 6);
    pmg.deallocateMatrix(resultado2, 6);
    pmg.deallocateMatrix(resultado3, 6);
    //Fin calculo Pn+1
    //Hago z, meto las medidas en la matriz
    medidas[0][0]=posx;medidas[0][1]=posy;medidas[0][2]=posz;
    medidas[0][3]=velx;medidas[0][4]=vely;medidas[0][5]=velz;
    //medidas[0][6]=_ax;medidas[0][7]=_ay;medidas[0][8]=_az;
    //Hago R, las varianzas y COV de las posiciones, vel y accel
    R[0][0]=varposx*varposx;
    R[1][1]=varposy*varposy;
    R[2][2]=varposz*varposz;
    R[3][3]=varvelx*varvelx;
    R[4][4]=varvely*varvely;
    R[5][5]=varvelz*varvelz;
    R[0][3]=R[3][0]=varposx*varvelx;
    R[1][4]=R[4][1]=varposx*varvelx;
    R[2][5]=R[5][2]=varposx*varvelx;
    //El resto como no hay correlacion (distinto eje no cov) -> 0
    double **H=suma.crearZeroMatriz(6, 6);//Z son 6 y Xn(inputF) son 6
    H=suma.convertirIdentidadMatriz(H, 6, 6);
    double **K=suma.crearZeroMatriz(6, 6);
}