#include<Wire.h> //Carrega a biblioteca Wire necessária para o protocolo I2C
    const int MPU=0x68; //Endereço do sensor MPU-6050
    int Ax,Ay,Az,T,Gx,Gy,Gz; //Variáveis para armazenar os valores do acelerômetro, giroscópio e sensor de temperatura
    const int button = 7; // Botão na porta digital 7
    const int saida5 = 5; //Saída para o codificador na porta digital 5
    int buttonState = 0; // Estado inicial do botão
    double ACC; //constante do módulo da aceleração
    double angleChange=0; //constante da Diferença angular
    const int bx=0;
    const int by=0;
    const int bz=2048;
    int BM=pow(pow(bx,2)+pow(by,2)+pow  (bz,2),0.5); //módulo do vetor de aceleração da referência
    //eixos do vetor da aceleração de referência
    boolean trigger1= false; //Sinaliza realização da condição1
    boolean trigger2= false; //Sinaliza realização da condição2
    boolean trigger3= false; //Sinaliza realização da condição3
    byte trigger1count=0; //temporizador1
    byte trigger2count=0; //temporizador2
    byte trigger3count=0; //temporizador3
    void setup()
    {
        Serial.begin(9600); //Início da comunicação serial
        Wire.begin(); //Inicialização da biblioteca Wire
        Wire.beginTransmission(MPU); //Início da transmissão
        Wire.write(0x6B); //Inicializa o sensor MPU-6050
        Wire.write(0);
        Wire.endTransmission(true);
        Wire.beginTransmission(MPU);
        Wire.write(0x1C);
        Wire.write(B00011000); //seleciona a sensibilidade do
        acelerômetro
        Wire.endTransmission(true); //Finaliza a transmissão
        pinMode(saida5, OUTPUT); //Porta Digital 5 como
        saída
        pinMode(button, INPUT); //Porta Digital 7 como
        entrada
    }
    void loop()
    {
        Wire.beginTransmission(MPU); //Início da transmissão
        Wire.write(0x3B); //Especifica o registrador de início
        Wire.endTransmission(false);
        Wire.requestFrom(MPU,14,true); //Solicita os dados do sensor
        //Armazena o valor dos sensores nas váriaveis
        Ax=Wire.read()<<8|Wire.read(); //Valor do acelerômetro no eixo x (registradores 0x3B e 0x3C)
        Ay=Wire.read()<<8|Wire.read(); //Valor do acelerômetro no eixo y (registradores 0x3D e 0x3E)
        Az=Wire.read()<<8|Wire.read(); //Valor do acelerômetro no eixo y (registradores 0x3F e 0x40)
        T=Wire.read()<<8|Wire.read(); //Valor da temperatura(registradores 0x41 e 0x42)
        Gx=Wire.read()<<8|Wire.read(); //Valor do giroscópio no eixo x (registradores 0x43 e 0x44)
        Gy=Wire.read()<<8|Wire.read(); //Valor do giroscópio no eixo y (registradores 0x45 e 0x46)
        Gz=Wire.read()<<8|Wire.read(); //Valor do giroscópio no eixo z (registradores 0x47 e 0x48)
        //Escrever os valores na serial
        Serial.print("Acelerômetro: X = ");
        Serial.print(AM);
        Serial.print(" | Y = ");
        Serial.print(Ay);
        Serial.print(" | Z = ");
        Serial.print(Az);
        Serial.println();
        Serial.print("Giroscópio: X = ");
        Serial.print(Gx);
        Serial.print(" | Y = ");
        Serial.print(Gy);
        Serial.print(" | Z = ");
        Serial.print(Gz);
        Serial.println();
        Serial.print("Temperatura: ");
        Serial.println(T/340.00+36.53); //Temperatura em grau Celsius
        Serial.println();
        buttonState = digitalRead(button); //Configuração do Botão
        ACC=pow((pow(Ax,2)+pow(Ay,2)+pow(Az,2)),0.5); //cálculo do modulo da aceleração
        angleChange=acos(((double)Ax*(double)bx+(double)Ay*(double)
        by+(double)Az*(double)bz)/(double)AM/(double)BM); //cálculo da diferença angular
        if (buttonState == HIGH)//Se o botão for pressionado
        {
            digitalWrite(saida5, LOW); //Dispare o alarme
        }
        else //se o botão não for pressionado
        {
            if(ACC<=1700 && angleChange<0.5)
            //verificar primeira condição
            {
            trigger1=true; //realização da primeira condição
            }
            if (trigger1==true)//se primeira condição acontecer
            {
                trigger1count++;//incrementa temporizador1
                if (ACC>=3684) //verificar segunda condição
                {
                    trigger2=true;//realização da segunda condição
                    trigger1=false;
                    trigger1count=0; //zera temporizador1
                }
            }
            if (trigger2==true) //se segunda condição acontecer
            {
                trigger2count++; //incrementa temporizador2
                if (angleChange>1) //verificar terceira condição
                {
                    trigger3=true; //realização da terceira condição
                    trigger2=false;
                    trigger2count=0; //zera temporizador2
                }
            }
            if(trigger3==true) //se terceira condição acontecer
            {
                trigger3count++; //incrementa temporizador3
                digitalWrite(saida5, LOW); //Dispara o alarme
            }
            else //se as três condições não acontecer
            {
                digitalWrite(saida5, HIGH); //não dispara o alarme
            }
        }
        if (trigger1count>=7) //segunda condição tem 140 ms para ser executada
        {
            trigger1=false;
            trigger1count=0; //zera temporizador1
        }
        if (trigger2count>=7) //terceira condição tem 140 ms para ser executada
        {
            trigger2=false;
            trigger2count=0; //zera temporizador2
        }
        if (trigger3count>=20) //alarme ficara disparado por apenas 400 ms
        {
            trigger3=false;
            trigger3count=0; //zera temporizador3
            digitalWrite(saida5, HIGH); //Desliga o alarme
        }
        delay(20); // mede os valores a cada 20 ms
    }