#include <Servo.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Ultrasonic.h>

byte mac[] = {
  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF 
};
IPAddress ip(192,168,0,23);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255, 255, 255, 0);
EthernetServer server(80);

//SENSORES
#define echoPin 23
#define trigPin 22
Ultrasonic ultrasonic(trigPin,echoPin);
#define lm35Pin A0
#define ky038Pin A1

//ENTRADAS
#define servoPin 24
Servo servo;

void setup() {
  //INIT
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(lm35Pin, INPUT);
  pinMode(ky038, INPUT);
  servo.attach(servoPin);
  servo.write(0);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean emBranco = false;
    boolean isBody = false;
    String body = "";
    DynamicJsonDocument doc(1024);
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (isBody) {
          body.concat(c);
          continue;
        }
        if (c == '{') {
          body.concat(c);
          isBody = true;
        }
      }
      else {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json; charset=UTF-8");
        client.println("Connection: close");  // the connection will be closed after completion of the response
        client.println("personalizado: variavelTeste");
        client.println("Access-Control-Allow-Origin: *");
        client.println("Access-Control-Allow-Headers: *");
        client.println();
        
        Serial.println();
        deserializeJson(doc, body);
        String token = doc["token"];
        bool receiveData = doc["receiveData"];
        
        if (token == "0147") {
          if (receiveData) {

            //JSON com dados dos sensores
            String dados;
            DynamicJsonDocument docSend(1024);
            docSend["hcsr04"] = hcsr04();
            docSend["lm35"] = lm35();
            docSend["ky038"] = ky038();
            serializeJson(docSend, dados);
            
            client.print(
              "{\"status\":\"success\", \"dados\":"
            );
            client.print(dados);
            client.print('}');
          }
          else {
            client.println(
              "{\"status\": \"success\"}"
            );
            servo.write(doc["data"]["servo"]);
          }
        }
        else {
          client.println(
            "{\"status\":\"error\", \"message\":\"token inválido\"}"
          );
        }
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println();
    Serial.println("client disconnected");
  }
}

//============================== SENSORES =================================

int hcsr04() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return (ultrasonic.Ranging(CM));
}

float lm35() {
  float lm35AnalogValue = float(analogRead(lm35Pin));
  float lm35Tensao = (lm35AnalogValue * 5) / 1023;
  return lm35Tensao / 0.010;
}

int ky038() {
  int total = 0;
  for (int i = 0; i<100; i++) {
    total += analogRead(ky038Pin);
  }
  return total / 100;
}
