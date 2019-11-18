#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Twinbot";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

const int numLeds=3;//número de leds
const String ref="handle_led";
const int pines[]={2,15,4} ;// Los pines que están usando
bool stats[]={false,false,false}; // pongan la misma cantidad de false que pines que están usando
const char* nombres[]={"LED1","LED2","LED3"};

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

/* Los arreglos de arriba sustituyen estos valores.
 * uint8_t LED1pin = 2;
bool LED1status = LOW;

uint8_t LED2pin = 15;
bool LED2status = LOW;

uint8_t LED3pin = 4;
bool LED3status = LOW;*/

void setup() {
  Serial.begin(115200);
  /*pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT); 
  pinMode(LED3pin, OUTPUT);*/ 


  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  // Tienen que agregar una linea para cada led que usen y la posi
  server.on("/led1", handle_led1);
  server.on("/led2", handle_led2);
  server.on("/led3", handle_led3);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  /*{digitalWrite(LED1pin, LED1status);}
 
  {digitalWrite(LED2pin, LED2status);}
  
  {digitalWrite(LED3pin, LED3status);}*/ // estos son sustituidos por el for siguiente
  for (int i=0;i<numLeds;i++){
    digitalWrite(pines[i],stats[i]);
  }

}

void handle_OnConnect() {
  for (int i=0;i<numLeds;i++){
    digitalWrite(pines[i],0);
  }
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led1() {
  stats[0] = !stats[0];
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led2() {
  stats[1] = !stats[1];
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led3() {
  stats[2] = !stats[2];
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  for(int i=0;i<numLeds;i++){
    if(stats[i])//cambiar "LED"+String(i+1)+"Status" por el nombre que quieran que aparezca en la pagina web.
    {ptr +="<p>"+String(nombres[i])+"Status: ON</p><a class=\"button button-off\" href=\"/led"+String(i+1)+"\">OFF</a>\n";}
    else
    {ptr +="<p>LED"+String(nombres[1])+"Status: OFF</p><a class=\"button button-on\" href=\"/led"+String(i+1)+"\">ON</a>\n";}
  }

  

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
