#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF 
};
IPAddress ip(192,168,0,23);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

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
            client.println(
              "{\"status\":\"success\", \"dados\":\"75\"}"
            );
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
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println();
    Serial.println("client disconnected");
  }
}
