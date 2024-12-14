#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "consts.h"
#include "communication.h"
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
char *initModeCS = NULL;
char *deviceMac;
WiFiClient httpClient;
char *baseURL;

void connectToWifiUsingWPA(const char *ssid, const char *pwd) {
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    sendResponseCode(R_PROCESSING);
  }
}

bool isValidArg(char *arg) {
  return  server.hasArg(arg) && server.arg(arg) != NULL;
}

void echoWifiStringToMaster(char *ssid, char *pwd) {
  char *buff = (char*)malloc(strlen(ssid) + strlen(pwd) + 10);
  sprintf(buff, "%s:%s", ssid, pwd);
  writeStringWithMarkers(buff);
  free(buff);
}

u_int8_t connectToNetwork() {
  char *ssid = readStringWithMarkers();
  if (ssid == NULL) {
    return R_FAILED;
  }
  
  sendResponseCode(R_RECEIVED);
  char *pwd = readStringWithMarkers();
  if (pwd == NULL) {
    return R_FAILED;
  }

  sendResponseCode(R_RECEIVED);
  echoWifiStringToMaster(ssid, pwd);
  connectToWifiUsingWPA(ssid, pwd);
  free(ssid);
  free(pwd);
  
  return R_SUCCESS;
}


void handleRoot() {
  server.send(200, "text/plain", "<html><body><h1>welcome to prodstats init server</h1><br><p>steps will be here...</p></body></html>"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}



void handleInit() {                         // If a POST request is made to URI /login
  if (isValidArg(const_cast<char*>("ssid")) && isValidArg(const_cast<char*>("pwd")) && isValidArg(const_cast<char*>("baseURL"))) {
    String cs = server.arg(const_cast<char*>("ssid"));
    cs.concat("\n");
    cs.concat(server.arg(const_cast<char*>("pwd")));
    cs.concat("\n");
    cs.concat(server.arg(const_cast<char*>("baseURL")));
    cs.concat("\n&\n");
    //sprintf(buff, eepromStringFormat,, server.arg("pwd"), server.arg("baseURL"));
    server.send(200, "text/html" , cs);
    sendResponseCode(R_SUCCESS);
    if (initModeCS != NULL) {
      free(initModeCS);
    }
    initModeCS = (char*)malloc(cs.length() + 1);
    cs.toCharArray(initModeCS, cs.length());
    writeStringWithMarkers(initModeCS);
    return;
  }
  sendResponseCode(R_EMPTY);
  server.send(400, "text/plain", "400: Invalid Request");
}



char* getMacAddress() {
  sendResponseCode(R_ACK);
  String mac = WiFi.macAddress();
  if (mac.length() < 1) {
    return NULL;
  }
  deviceMac = (char*)malloc(mac.length() + 1);
  mac.toCharArray(deviceMac, mac.length() + 1);
  return deviceMac;
}


void sendPOST(int vibration) {
  char buff[100];
  sprintf(buff, "{\"id\":\"%s\",\"v\":%d}", deviceMac, vibration);
  Serial.println(buff);
  if (httpClient.connect(baseURL, 8080)) {
    httpClient.println("POST /api/v1/device/record HTTP/1.1");
    httpClient.print("Host:"); httpClient.println(baseURL);
    httpClient.println("Content-Type: application/json");
    httpClient.print("Content-Length:"); httpClient.println(strlen(buff));
    httpClient.println();
    httpClient.println(buff);
  }
}


char* getIPAddress() {
  char *buff = (char*)malloc(16);
  IPAddress ip = WiFi.localIP();
  sprintf(buff, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  if (strlen(buff) <= 0) {
    free(buff);
    return NULL;
  }
  return buff;
}

char* listenAndWait() {
  MDNS.begin("esp8266");
  server.on("/", handleRoot);
  server.on("/init", handleInit);
  server.onNotFound(handleNotFound); 
  server.begin();
  while(1){
    server.handleClient();
  }
}

//char* parsePostRequest(WiFiClient client, char *body) {
//  char c = client.read();
//  if (c == '\n' && currentLineIsBlank) {
//    byte i = 0;
//    while (client.available())
//    {
//      body[i++] = client.read();
//    }
//    body[i] = '\0';
//    client.println("HTTP/1.1 200 OK");
//    client.println("Content-Type: text/html");
//    client.println(body);
//    client.stop();
//    return body;
//  }
//  else if (c == '\n') {
//    currentLineIsBlank = true;
//  }
//  else if (c != '\r') {
//    currentLineIsBlank = false;
//  }
//  return NULL;
//}
