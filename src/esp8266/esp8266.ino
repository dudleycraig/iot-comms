#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

static const char ssid[] = "Silvertree Holdings";
static const char password[] = "***********";

// static const char ssid[] = "iPhone";
// static const char password[] = "4getsme!";

// static const char ssid[] = "Winternet";
// static const char password[] = "Candice1";

char * status = "off"; // status of led

MDNSResponder mdns;

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80); // webserver for http requests 

WebSocketsServer webSocket = WebSocketsServer(81); // socket server for socket requests

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name = "viewport" content = "width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
    <title>ESP8266</title>
    <style>
      body {
        background-color: #808080;
        font-family: Arial, Helvetica, Sans-Serif;
        color: #000000;
      }

      .date { }
      .date .day, .date .month, .date .year { display: none; }
      .date .hour, .date .minute, .date .second { font-size: 16px; }
      .date .millisecond { font-size: 12px }
      .date .hour::after, .date .minute::after { content: ':'; }
      .date .second::after { content: '.'; }
      .date::after { content: '>'; padding-right: 5px; }

      #messages { list-style: none; }
      #messages li { white-space: nowrap; overflow: visible; }
      #messages li.success { color: #00ff00; }
      #messages li.warning { color: #ff8000; }
      #messages li.error { color: #ff0000; }
      #messages li.info { color: #404040; }
      #messages li div { display: inline-block; }
      #messages li div.date *, #messages li div.date::after { color: #303030; }
      #messages li div.text {  }
    </style>
    <script>
      Date.prototype.padzero = function() {
        return {
          day: ('0' + this.getDate()).slice(-2),
          month: ('0' + (this.getMonth() + 1)).slice(-2),
          year: this.getFullYear(),
          hour: ('0' + this.getHours()).slice(-2),
          minute: ('0' + this.getMinutes()).slice(-2),
          second: ('0' + this.getSeconds()).slice(-2),
          millisecond: ('00' + this.getMilliseconds()).slice(-3)
        }
      }
    </script>
  </head>
  <body onload="javascript:start();">
    <h1>ESP8266</h1>
    <div id="status"><b>LED</b></div>
    <button id="on" type="button" onclick="toggle(this)">On</button> 
    <button id="off" type="button" onclick="toggle(this)">Off</button>
    <button id="toggle" type="button" onclick="toggle(this)">Toggle</button>
    <ul id="messages"></ul>
    <script>
      let ws = {}
      let status = 'off'
      let messages = []
      let ul = document.getElementById('messages')

      const clearMessages = () => {
        while (ul.firstChild) { ul.removeChild(ul.firstChild) }
      }

      const renderDate = (date = new Date()) => {
        const padded = date.padzero()
        const div = document.createElement('div')
        div.className = 'date'
        Object.keys({...padded}).forEach((key, index) => {
          const span = document.createElement('span')
          span.className = key
          const text = document.createTextNode(padded[key])
          span.appendChild(text)
          div.appendChild(span)
        })
        return div
      }

      const renderMessages = (newMessages = []) => {
        messages = [...newMessages.map((message, index) => { message.date = new Date(); return message }), ...messages]
        clearMessages()
        const viewable = messages.slice(0, 3)
        for(index in viewable) {
          const message = messages[index]
          const date = renderDate(message.date)
          const li = document.createElement('li')
          const div = document.createElement('div')
          const textNode = document.createTextNode(message.text)
          div.className = 'text'
          div.appendChild(textNode)
          li.className = message.status
          li.appendChild(date)
          li.appendChild(div)
          ul.appendChild(li)
        }
      }

      const wsHandleMessage = (data) => {
        const statusElement = document.getElementById('status')
        const { payload: { status: ledStatus, messages = [] } } = data
        switch(data.status) {
          case ('success'):
            statusElement.style.color = ledStatus === 'on' ? 'red' : 'black' 
            status = ledStatus
            renderMessages(messages)
            break
          case ('warning'):
          case ('error'):
          case ('info'):
            renderMessages(messages)
            break
          default:
            renderMessages([{status: 'error', text: 'Unknown error'}])
            break
        }
      }

      const start = () => {
        ws = new WebSocket('ws://' + window.location.hostname + ':81/');
        ws.onopen = event => {
          console.log('websocket open')
          ws.send('status')
        }
        ws.onclose = event => {
          console.log('websocket close')
        }
        ws.onerror = event => {
          console.log(event)
        }
        ws.onmessage = event => {
          wsHandleMessage(JSON.parse(event.data))
        }
      }

      const toggle = (element) => {
        switch(element.id) {
          case('on'): 
            ws.send('on')
            break
          case('off'):
            ws.send('off')
            break
          case('toggle'):
            ws.send(status === 'off' ? 'on' : 'off')
            break
        }
      }
    </script>
  </body>
</html>
)rawliteral";

void handleRoot() { // serve root web page
  Serial.println("serving root page.");
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound() { // serve 404
  Serial.println("serving 404 page.");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleMessage(uint8_t * message, size_t length, uint8_t client) {
  uint8_t * response;
  if (strcmp("on", (char *)message) == 0) { // cast uint8_t (unsigned pointer) to signed pointer (char *)
    digitalWrite(0, 1); // write pin "D3" on nodemcu
    if (strcmp("on", (char *)status) != 0) {
      status = "on";
      broadcastMessage((uint8_t *)"{\"status\":\"success\",\"payload\":{\"status\":\"on\",\"messages\":[{\"status\":\"success\",\"text\":\"LED is ON\"}]}}"); // send text, state to all connected clients
    }
  }
  else if (strcmp("off", (char *)message) == 0) { // cast uint8_t (unsigned pointer) to signed pointer (char *)
    digitalWrite(0, 0); // write pin "D3" on nodemcu
    if (strcmp("off", (char *)status) != 0) {
      status = "off";
      broadcastMessage((uint8_t *)"{\"status\":\"success\",\"payload\":{\"status\":\"off\",\"messages\":[{\"status\":\"success\",\"text\":\"LED is OFF\"}]}}"); // send text, state to all connected clients
    }
  }
  else if (strcmp("status", (char *)message) == 0) { // cast uint8_t (unsigned pointer) to signed pointer (char *)
    digitalWrite(0, 0); // write pin "D3" on nodemcu
    if (strcmp("on", (char *)status) == 0) {
      sendMessage((uint8_t *)"{\"status\":\"success\",\"payload\":{\"status\":\"on\",\"messages\":[{\"status\":\"success\",\"text\":\"LED is ON\"}]}}", client); // send text, state to client 
    }
    else {
      sendMessage((uint8_t *)"{\"status\":\"success\",\"payload\":{\"status\":\"off\",\"messages\":[{\"status\":\"success\",\"text\":\"LED is OFF\"}]}}", client); // send text, state to client 
    }
  }
  else {
    Serial.println("unknown command");
    sendMessage((uint8_t *)"{\"status\":\"error\",\"payload\":{\"messages\":[{\"status\":\"error\",\"text\":\"Unknown command.\"}]}}", client);
  }
}

// send text to all connected clients
void broadcastMessage(uint8_t * message) {
  Serial.print("broadcasting message: ");
  Serial.print((const char *)message);
  Serial.println();
  webSocket.broadcastTXT(message, strlen((const char *)message));
}

// send text to connected client
void sendMessage(uint8_t * message, uint8_t client) { 
  Serial.print("sending message: ");
  Serial.print((const char *)message);
  Serial.println();
  webSocket.sendTXT(client, message, strlen((const char *)message)); 
}

// display details of client
void dumpClient(uint8_t client, uint8_t * message) {
  IPAddress ip = webSocket.remoteIP(client);
  Serial.printf("[%u] connected from %d.%d.%d.%d url: %s\r\n", client, ip[0], ip[1], ip[2], ip[3], message);
}

void handleEvent(
  uint8_t client, // connected client data, unsigned 8 bit pointer as opposed to a signed pointer (char *)
  WStype_t type, // data type, binary or text
  uint8_t * message, // message received on socket, unsigned 8 bit pointer as opposed to a signed pointer (char *)
  size_t length // length of message
) {
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", client, type);

  switch(type) {

    // disconnected from client
    case WStype_DISCONNECTED: 
      Serial.printf("[%u] Disconnected!\r\n", client);
      break;

    // connected to client
    case WStype_CONNECTED: 
      dumpClient(client, message);
      break;

    // received data type text 
    case WStype_TEXT: 
      handleMessage(message, length, client);
      break;

    // received data type binary 
    case WStype_BIN: 
      Serial.printf("[%u] got binary length: %u\r\n", client, length);
      break;

    // handle error
    case WStype_ERROR: 
      Serial.printf("[%u] error: %s\r\n", client, message);
      break;

    // handle packets
    case WStype_FRAGMENT_TEXT_START: 
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      Serial.printf("[%u] fragment: %s\r\n", client, message);
      break;

    // undefined event triggered by websocket
    default:
      Serial.printf("invalid event [%d]\r\n", type);
      break;
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

  pinMode(0, OUTPUT); // set pin 0 to push output as opposed to receiving input

  digitalWrite(0, 0); // write pin "D3" on nodemcu to off

  WiFi.mode(WIFI_STA); // reset wifi mode for acquiring new ip address in station mode
  // WiFi.mode(WIFI_AP); // reset wifi mode for acquiring new ip address in access point mode
  WiFiMulti.addAP(ssid, password);

  while(int stat = WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(">>");
    Serial.print(stat);
    Serial.println();
    delay(100);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(handleEvent);

  if (mdns.begin("light.esp8266.local", WiFi.localIP())) { // add domain to mdns
    Serial.println("MDNS responder started");
  }
  else {
    Serial.println("MDNS.begin failed");
  }
  Serial.print("Connect to http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();

  mdns.addService("http", "tcp", 80);
  mdns.addService("ws", "tcp", 81);
}

void loop() {
  webSocket.loop();
  server.handleClient();
}
