// Author : Matheus Simões Peixeiro
// Inspired By: Fábio Souza's course in Udemy: www.udemy.com/course/controle-dispositivos-remotamente-com-esp8266

// Acionamento de uma saída digital do ESP8266 por um servidor local WEB (Hospedado no próprio ESP)
// OBS.: É necessário ter instalado na IDE Arduino a biblioteca compatível com o ESP8266 

// Triggering a digital output of ESP8266 by a local WEB server (Hosted on ESP itself.
// OBS.: It is necessary to have the ESP8266 compatible library installed in the Arduino IDE.

#include <ESP8266WiFi.h>

// port 2 => An ESP8266 LED output
const byte LED = 2;

// LED Status begin off
boolean statusLED = LOW;

// Enter your WiFi details
char ssid [] = "WIFI-SSID";
char senha[] = "WIFI-PASSWORD";

// You can instantiate fixed IPs as follows
WiFiServer server(80);
IPAddress ip(192,168,0,27);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

void setup() {
  pinMode(LED,OUTPUT);

  // Station mode
  WiFi.mode(WIFI_STA);
  
  Serial.begin(115200);
  delay(10);

  connectWiFi(ssid,senha);

  server.begin();
  Serial.println("Server started");
  
}

void loop() {

  WiFiClient client = server.available();
  if(!client){
    return;
  }

  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if(req.indexOf("ledOn") != -1){
    digitalWrite(LED,HIGH);
    statusLED = HIGH;
  }
  else if(req.indexOf("ledOff")!=-1){
          digitalWrite(LED,LOW);
          statusLED = LOW;
  }
   
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv='refresh' content='2'>");
  client.println("<h1>Esp8266 led controller web server</h1>");
  client.println("<h2>Hello!</h2>");

  if(!statusLED)
  client.println("<p>LED <a href=\"ledOn\"><button>Desligar</button></a></p>");
  else
  client.println("<p>LED <a href=\"ledOff\"><button>Ligar</button></a></p>");

  client.println("</html>");
   
}


void connectWiFi(char SSID[],char SENHA[]){

  Serial.print("Connecting to network");
  Serial.println(SSID);

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(SSID,SENHA);

  // Prints "." while it does not connect
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println(" ");
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}
