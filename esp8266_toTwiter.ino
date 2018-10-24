#include <ESP8266WiFi.h>

// Wi-Fi 
const char* ssid = ""; //(SSID)
const char* password = ""; //Senha


WiFiClient client;

// ThingSpeak 
const int channelID = 10;
String writeAPIKey = "5CKAXMD6V1I3WLTS"; // API
const char* server = "api.thingspeak.com";
const int postingInterval = 1 * 1000; //Um envio a cada 20 segundos
String tsData;
int i =0;

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;

void setup() {

  pinMode(trigPin, OUTPUT); // Porta trigPin as an Output
  pinMode(echoPin, INPUT); // Porta the echoPin as an Input
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
   
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    
    // Calcula a distancia
    distance= duration*0.034/2;
   
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance<10)
    {      
      if (client.connect(server, 80)) 
      {
        i=i+1;
        
        long rssi = WiFi.RSSI();
        tsData = "api_key="+writeAPIKey+"&status="+"Animal perto  "+distance+" cm.";
        client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(tsData.length());
        client.print("\n\n");
        client.print(tsData);
        Serial.println("tweet enviado");
      }
      client.stop();    
    }
    delay(postingInterval);
}
