//Programa para subir datos de la temperatura en la atmosfera

#include <OneWire.h>
#include <DallasTemperature.h>
#include "floatToString.h"

#define CTRL_Z 26

#define DALLAS              0
#define DALLAS1             1
#define DALLAS2             2
#define DALLAS3             3
#define DALLAS4             4
#define DALLAS5             5
#define DALLAS6             6
#define DALLAS7             7

String temp1;
String temp2;
String temp3;
String temp4;
String temp5;
String temp6;
String temp7;
String temp8;

#define DALLASPIN 4 //Se declara el pin donde se conectará la DATA
#define PinOn     7

OneWire ourWire(DALLASPIN); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

unsigned long myChannelNumber = 285354;
const char * myWriteAPIKey = "JAIC2WLODHZPYPRV";


void setup() 
{
  uint32_t baud = 9600;
  Serial.begin(baud);
  while (!Serial) { }
  Serial1.begin(baud);
  //while (!Serial1) { }
  //Serial.print("SETUP!! @");
  //Serial.println(baud);
  pinMode(pwrPin, OUTPUT);
  digitalWrite(pwrPin, HIGH);

  sensors.begin(); //Se inician los sensores DS18B20
}

void loop() 
{
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
  delay(1000);

  Serial.write("Midiendo Sensores...\n");
  float dallas = sensors.getTempCByIndex(0);
  float dallas1 = sensors.getTempCByIndex(1);
  float dallas2 = sensors.getTempCByIndex(2);
  float dallas3 = sensors.getTempCByIndex(3);
  float dallas4 = sensors.getTempCByIndex(4);
  float dallas5 = sensors.getTempCByIndex(5);
  float dallas6 = sensors.getTempCByIndex(6);
  float dallas7 = sensors.getTempCByIndex(7);


  temp1 = floatToString(buffer, dallas, 2);
  temp2 = floatToString(buffer, dallas1, 2);
  temp3 = floatToString(buffer, dallas2, 2);
  temp4 = floatToString(buffer, dallas3, 2);
  temp5 = floatToString(buffer, dallas4, 2);
  temp6 = floatToString(buffer, dallas5, 2);
  temp7 = floatToString(buffer, dallas6, 2);
  temp8 = floatToString(buffer, dallas7, 2);

    Serial.println("Valores de Sensores");
    Serial.print("Temp 1: ");
    Serial.println(temp1);
    Serial.print("Temp 2: ");
    Serial.println(temp2);
    Serial.print("Temp 3: ");
    Serial.println(temp3);
    Serial.print("Temp 4: ");
    Serial.println(temp4);
    Serial.print("Temp 5: ");
    Serial.println(temp5);
    Serial.print("Temp 6: ");
    Serial.println(temp6);
    Serial.print("Temp 7: ");
    Serial.println(temp7);
    Serial.print("Temp 8: ");
    Serial.println(temp8);
    Serial.println("");

  Serial.write("Encendiendo GPRSBee...\n");
  OnOFFGPRSbee();
  iniciarGPRSbeee();
  Serial.write("Estableciendo Conexion TCP...\n");
  openTCP();
  sendData();
  OnOFFGPRSbee();    
}

void OnOFFGPRSbee() {
  //delay(4000);
  digitalWrite(PinOn, LOW),
               delay(1000);
  digitalWrite(PinOn, HIGH);
  esperarRespuesta();
  while(Serial1.find("Call")==0){esperarRespuesta();
  //if(Serial.find("Call Ready") != 0){ esperarRespuesta(); break;}
  }
  delay(5000);  
  Serial.print("tamo_redy");    
}

void iniciarGPRSbee() {
Serial1.println("AT");
  esperarRespuesta();
  Serial1.println("AT+CREG?");
  esperarRespuesta();
  Serial1.println("AT+QIFGCNT=0");
  esperarRespuesta();
  Serial1.println("AT+QICSGP=1,\"imovil.entelpcs.cl\"");
  esperarRespuesta();
  Serial1.println("AT+QIDNSIP=1");
  esperarRespuesta();
  Serial1.println("AT+QINDI=1");
  esperarRespuesta();
  Serial1.println("AT+QIMUX=0");
  esperarRespuesta();
  Serial1.println("AT+QIMODE=0");
  delay(1000);
  esperarRespuesta();
}


void esperarRespuesta()
{
    while(Serial1.available() > 0) 
    {
      char a = Serial1.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      Serial.print(a);
    }
    
    while(Serial.available() > 0)
    {
      char a = Serial.read();
      //Serial.write(a);
      Serial1.write(a);
    }
    delay(100);
}

void openTCP()
{
  Serial1.println("AT+QIOPEN=\"TCP\",\"184.106.153.149\",\"80\"");
  //while(Serial1.find("CONNECT") == 0){continue;}
  //esperarRespuesta();
  while(true){
  esperarRespuesta();
  if(Serial1.find("CONNECT OK")!=0){break;}}
  Serial.println("CONNECT OK");
}

void sendData(){
Serial1.println("AT+QISEND");
//while(Serial1.find("OK")==0){continue;}
//Serial.println("OK");
  
while(Serial1.find(">") == 0){continue;}
//esperarRespuesta();
Serial1.print("GET /update?api_key=JAIC2WLODHZPYPRV&
Serial1.print("field1=" + temp1);
Serial1.print("&field2=" + temp2);
Serial1.print("&field3=" + temp3);
Serial1.print("&field4=" + temp4);
Serial1.print("&field5=" + temp5);
Serial1.print("&field6=" + temp6);
Serial1.print("&field7=" + temp7);
Serial1.print("&field8=" + temp8);
Serial1.print(" HTTP/1.1");
//Serial1.println("POST /update HTTP/1.1");
Serial1.print("\r\nHost: 184.106.153.149");
Serial1.print("\r\nUser-Agent: curl/7.45.0");
Serial1.print("\r\nConnection: close\r\n\r\n");  
//Serial1.println(" HTTP/1.1Connection: close");
Serial1.write(CTRL_Z);

esperarRespuesta();

  //while(Serial1.find("OK")==0){
    //continue;}

//while(Serial1.find("Send OK") == 0){continue;}

//esperarRespuesta();
//delay(5000);
}
