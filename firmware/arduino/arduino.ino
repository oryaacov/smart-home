#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "consts.h"
#include "wifi.h"

//vars
int communicationBuffer;
const char* eepromStringFormat = "%s\n%s\n%s\n&\n";
int vibration;
u_int8_t run_mode = PROD_MODE;
char *ip;
char *_baseURL;
char *vibrationString;
bool currentLineIsBlank = true;
char *_deviceMac;

//functions
void replay(byte code);


void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("prodstats_esp8266 started");
}

void loop() {
  unsigned int command = receiveCommandFromMaster();
  if (command>0){
    handleCommand(command);
  }
}

void handleCommand(unsigned int cmd) {
  switch (cmd) {
    case CONNECT_TO_DEFAULT:
      connectToWifiUsingWPA(SECRET_SSID, SECRET_PASS);
      reply(R_SUCCESS, 0);
      break;

    case HANDSHAKE:
      reply(HANDSHAKE, 0);
      break;

    case CONNECT_TO_NETWORK:
      reply(connectToNetwork(), 0);
      break;


    case GET_CS:
      listenAndWait();
      //if (initModeCS==NULL){ todo: complete
        sendResponseCode(R_EMPTY);
      //}
      break;

    case GET_MAC_ADDRESS:
      _deviceMac = getMacAddress();
      if (_deviceMac != NULL) {
        writeStringWithMarkers(_deviceMac);
      } else {
        writeStringWithMarkers(const_cast<char*>("failed to get mac"));
      }
      break;

    case GET_BASEURL:
      _baseURL = readStringWithMarkers();
      break;

    case GET_IP_ADDRESS:
      ip = getIPAddress();
      writeStringWithMarkers(ip);
      free(ip);
      break;


    case START_INIT_SERVER:
      listenAndWait();
      break;

    case SEND_VIB_TO_CLOUD:
      vibrationString = readStringWithMarkers();
      vibration = atoi(vibrationString);
      free(vibrationString);
      sendPOST(vibration);
      break;

    default:
      reply(R_UNKNOWN_COMMAND, 0);
      break;

  }
}
