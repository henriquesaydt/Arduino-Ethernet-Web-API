#include <SPI.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <Ultrasonic.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Ethernet.h>


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
#define lm35Pin A8
#define ky038Pin A9

//ENTRADAS
#define rgb_r 9
#define rgb_g 8
#define rgb_b 7
#define led_r 37
#define led_g 36
#define led_b 35
#define servoPin 34
Servo servo;
Adafruit_PCD8544 Nokia = Adafruit_PCD8544(29, 30, 31, 32, 33);

void setup() {
  //INIT
  pinMode(rgb_r, OUTPUT);
  pinMode(rgb_g, OUTPUT);
  pinMode(rgb_b, OUTPUT);
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(lm35Pin, INPUT);
  pinMode(ky038Pin, INPUT);

  led_rgb(0, 0, 0);
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, LOW);
  digitalWrite(led_b, LOW);
  
  servo.attach(servoPin);
  servo.write(0);
  
  Nokia.begin();
  Nokia.setContrast(20);
  Nokia.clearDisplay();
  Nokia.display();

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  
  Serial.println("Iniciando Ethernet Shield");
  Ethernet.begin(mac, ip, gateway, subnet);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield não encontrado");
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Cabo de rede desconectado");
  }

  server.begin();
  Serial.print("Endereço do servidor: ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    boolean emBranco = false;
    boolean isBody = false;
    String body = "";
    DynamicJsonDocument doc(1024);
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
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
        client.println("Connection: close");
        client.println("personalizado: variavelTeste");
        client.println("Access-Control-Allow-Origin: *");
        client.println("Access-Control-Allow-Headers: *");
        client.println();
        
        //Serial.println();
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
          else if(receiveData == false) {
            client.println(
              "{\"status\": \"success\"}"
            );
            
            char* displayText = doc["data"]["display"];
            bool led_1_on = doc["data"]["led_1"];
            bool led_2_on = doc["data"]["led_2"];
            bool led_3_on = doc["data"]["led_3"];
            
            servo.write(doc["data"]["servo"]);
            nokia_display(displayText);
            digitalWrite(led_r, led_1_on);
            digitalWrite(led_g, led_2_on);
            digitalWrite(led_b, led_3_on);
            led_rgb(doc["data"]["led_rgb"]["r"], doc["data"]["led_rgb"]["g"], doc["data"]["led_rgb"]["b"]);
          }
          else {
            client.println(
              "{\"status\": \"success\"}"
            );
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
    delay(1);
    client.stop();
    //Serial.println();
    //Serial.println("client disconnected");
  }
}

int hcsr04() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return (ultrasonic.Ranging(CM));
}

float lm35() {
  float total = 0;
  for (int i = 0; i<100; i++) {
    total += analogRead(lm35Pin);
  }
  float lm35AnalogValue = total / 100;
  float lm35Tensao = (lm35AnalogValue * 5) / 1023;
  return lm35Tensao / 0.010;
}

float ky038() {
  float maior = 0;
  float atual = 0;
  for (int i = 0; i<100; i++) {
    atual = analogRead(ky038Pin);
    if(atual > maior) {
      maior = atual;
    }
  }
  float ky038Voltage = (maior * 5) / 1023;
  return ky038Voltage;
}

void led_rgb(int R, int G, int B) {
  R = abs(R-255);
  G = abs(G-255);
  B = abs(B-255);
  analogWrite(rgb_r, R);
  analogWrite(rgb_g, G);
  analogWrite(rgb_b, B);
}

void nokia_display(String text) {
  Nokia.clearDisplay();
  Nokia.display();
  Nokia.setTextSize(1);
  Nokia.setTextColor(BLACK);
  Nokia.setCursor(0,0);
  Nokia.println(text);
  Nokia.display();
}
