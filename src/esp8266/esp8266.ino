#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// static const char ssid[] = "Silvertree Holdings";
// static const char password[] = "5mart5hopper";

static const char ssid[] = "iPhone";
static const char password[] = "4getsme!";

static void writeLED(bool);
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>ESP8266 WebSocket Demo</title>
    <style>
      body {
        background-color: #808080;
        font-family: Arial, Helvetica, Sans-Serif;
        Color: #000000;
      }
    </style>
    <script>
      var websock;
      var start = function() {
        websock = new WebSocket('ws://' + window.location.hostname + ':81/');

        websock.onopen = event => {
          console.log('websock open')
        }

        websock.onclose = event => {
          console.log('websock close')
        }

        websock.onerror = event => {
          console.log(evt)
        }

        websock.onmessage = event => {
          const statusElement = document.getElementById('ledstatus')

          if (event.data === 'ledon') {
            statusElement.style.color = 'red'
          }

          else if (event.data === 'ledoff') {
            statusElement.style.color = 'black'
          }
        }
      }

      var lightSwitch = element => {
        websock.send(element.id)
      }
    </script>
  </head>

  <body onload="javascript:start();">
    <h1>ESP8266 WebSocket Demo</h1>

    <div id="ledstatus">
      <b>LED</b>
    </div>

    <button id="ledon" type="button" onclick="lightSwitch(this)">On</button> 
    <button id="ledoff" type="button" onclick="lightSwitch(this)">Off</button>

  </body>
</html>
)rawliteral";

const int LEDPIN = 0; // Pin "D3" on nodemcu.
bool LEDStatus; // LED status
const char LEDON[] = "ledon"; // request text
const char LEDOFF[] = "ledoff"; // request text

MDNSResponder mdns;

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80); // webserver to connect to socket

WebSocketsServer webSocket = WebSocketsServer(81); // socket server

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);

  switch(type) {

    // disconnected from client
    case WStype_DISCONNECTED: 
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;

    // connected to client 
    case WStype_CONNECTED: 
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, LEDON, strlen(LEDStatus ? LEDON : LEDOFF));
      }
      break;

    // got text
    case WStype_TEXT:  
      Serial.printf("[%u] got text: %s\r\n", num, payload);
      if (strcmp(LEDON, (const char *)payload) == 0) {
        writeLED(true); // switch led on
      }
      else if (strcmp(LEDOFF, (const char *)payload) == 0) {
        writeLED(false); // switch led off
      }
      else {
        Serial.println("unknown command");
      }
      webSocket.broadcastTXT(payload, length); // send text, state to all connected clients
      break;

    // got binary
    case WStype_BIN:  
      Serial.printf("[%u] got binary length: %u\r\n", num, length);
      hexdump(payload, length);
      webSocket.sendBIN(num, payload, length); // send binary
      break;

    // handle error
    case WStype_ERROR: 
      Serial.printf("[%u] error: %s\r\n", num, payload);
      break;

    // handle packets
    case WStype_FRAGMENT_TEXT_START: 
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      Serial.printf("[%u] fragment: %s\r\n", num, payload);
      break;

    default:
      Serial.printf("invalid event [%d]\r\n", type);
      break;
  }
}

void handleRoot() { // serve root web page
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound() { // serve 404
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

static void writeLED(bool LEDon) {
  LEDStatus = LEDon;
  if (LEDon) {
    digitalWrite(LEDPIN, 1);
  }
  else {
    digitalWrite(LEDPIN, 0);
  }
}

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  writeLED(false);

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

  // WiFi.mode(WIFI_STA); // reset wifi mode for acquiring new ip address in station mode
  WiFi.mode(WIFI_AP); // reset wifi mode for acquiring new ip address in access point mode
  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("espWebSock", WiFi.localIP())) {
    Serial.println("MDNS responder started");
    mdns.addService("http", "tcp", 80);
    mdns.addService("ws", "tcp", 81);
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("Connect to http://espWebSock.local or http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
}
