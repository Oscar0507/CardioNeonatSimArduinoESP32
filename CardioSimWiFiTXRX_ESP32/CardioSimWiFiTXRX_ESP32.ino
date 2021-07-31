//#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "SonidosResp.h"

 IPAddress local_IP (192 , 168 , 2 , 22);
 IPAddress gateway (192 , 168 , 2 , 1);
 IPAddress subnet (255 , 255 , 255 , 0);

WiFiUDP Udp;
bool TxUDP=false, runn=false; 
int cont=0;
float t=0;

unsigned long t_ini=0,t_fin=0,t0=0,ts=0;
int Fr=0;

 //Nombre de la red y la contraseña
const char *ssid = "CardioNeonatSim";
const char *password = "12345678";
unsigned int localPort = 8888;

char packetBuffer[7]; //char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
char msgArray[65];
char SendArray[65];
char buffer_in[64]; 
int N_data=0,test;
int N_dataUDPin=0;
String msgrUDPin=String(0);
char Dato;
size_t a1;

void setup() {
  delay(1000);
  Serial.begin(57600);
  Serial2.begin(57600);
  //delay(100);

  pinMode(2, OUTPUT); //Salida de led placa

  Serial.println("Configurando...cómo access point AP...");
  WiFi.softAP(ssid,password) ? "Ready":"Failed";  //Nombre y PASS del AP
  delay(100);
  WiFi.softAPConfig(local_IP, gateway,subnet);
  delay(100);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Conectado a red WIFI");

  WiFi.printDiag(Serial);
  Serial.println(WiFi.softAPmacAddress());
  Udp.begin(localPort);      //Inicio de protocolo UDP el puerto especificado}
  t0=micros();
}

void loop() {

  if (runn){
    ts=micros()-t0;
    //Serial.println(ts);  
    if (ts>=100000){
      t0=micros();
      dacWrite(25,SonResp[cont]);
      cont++;
      if (cont>=100){
        Serial.println(cont);
        cont=0;}
    }
    }
  
  digitalWrite(2, HIGH);
  //Leyendo posibles mensajes
  LeerUDP();
  a1=Serial2.readBytesUntil(255,buffer_in,64);
  if (a1>0){ 
            //Serial.println(buffer_in);
              //t_ini=micros();
              TxUDP=true;
            }
 if(TxUDP){
        TxUDP=false;
        TransmitirUDP();
        //t_fin=micros();
        //Serial.println(t_fin-t_ini);
        }
              
        }
       

void LeerUDP(){
  //Si hay datos disponibles lee un paquete
  int packetSize = Udp.parsePacket(); 
  //Serial.println("running");
  if(packetSize) {
    // read the packet into packetBufffer
    N_dataUDPin = Udp.read(packetBuffer, 7);   //N_dataUDPin = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[N_dataUDPin] = 0;
    msgrUDPin=String(packetBuffer);
    Serial.println("tx"+msgrUDPin);       //Diag: Imprime mensaje recibido vía WiFi
    if(msgrUDPin[0]=='i'){
      runn=true;
      t0=micros();
      }
      else if(msgrUDPin[0]=='b'){
          Fr=msgrUDPin.substring(1,N_dataUDPin-1).toInt();
          Serial.println(Fr);
          }
          else if(msgrUDPin[0]=='f'){
            runn=false;
//        timerAlarmDisable(timer);
//        Serial.println("FinInt");
        } 
    Serial2.print(msgrUDPin);             //Envía dato al Arduino vía serial 
  }
}

void TransmitirUDP(){Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                    Udp.print(buffer_in);
                    Udp.endPacket();}    
                    
