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

  Serial.begin(9600);
  Nokia.begin(); //INICIALIZA O DISPLAY
  Nokia.setContrast(20); //DEFINE O CONTRASTE DO DISPLAY
  Nokia.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
  Nokia.display(); //EFETIVA A ESCRITA NO DISPLAY
  
  Nokia.setTextSize(1); //DEFINE O TAMANHO DA FONTE DO TEXTO
  Nokia.setTextColor(BLACK); //DEFINE A COR DO TEXTO
  Nokia.setCursor(0,0); //POSIÇÃO EM QUE O CURSOR IRÁ FAZER A ESCRITA (COLUNA,LINHA)
  Nokia.println("Sistemas Embarcados 2 - Luiz Henrique Saydt"); //ESCREVE O TEXTO NO DISPLAY
  Nokia.display(); //EFETIVA A ESCRITA NO DISPLAY
}

void loop() {
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
  //float lm35AnalogValue = float(analogRead(lm35Pin));
  float lm35Tensao = (lm35AnalogValue * 5) / 1023;
  return lm35Tensao / 0.010;
}

int ky038() {
  int maior = 0;
  int atual = 0;
  for (int i = 0; i<100; i++) {
    atual = analogRead(ky038Pin);
    if(atual > maior) {
      maior = atual;
    }
  }
  return maior;
}

void led_rgb(int R, int G, int B) {
  R = abs(R-255);
  G = abs(G-255);
  B = abs(B-255);
  analogWrite(rgb_r, R);
  analogWrite(rgb_g, G);
  analogWrite(rgb_b, B);
}
