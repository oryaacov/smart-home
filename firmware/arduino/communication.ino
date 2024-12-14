#include "consts.h"

unsigned int parseCommand(char *s) {
  int command = atoi(s);
  if (command <= 0) {
    return 0;
  }
  return hasedToCommand((unsigned int)command);
}

bool isKnownCommand(unsigned int cmd) {
  return cmd >= CONNECT_TO_DEFAULT && cmd <= GET_CS;
}

unsigned int receiveCommandFromMaster() {
  char* commandString = readStringWithMarkers();
  if (commandString == NULL) {
    sendResponseCode(R_TIMEOUT);
    return -1;
  }
  unsigned int cmd = parseCommand(commandString);
  free(commandString);
  if (cmd > 0) {
    if (isKnownCommand(cmd)) {
      sendResponseCode(R_RECEIVED);
      return cmd;
    } else {
      sendResponseCode(R_UNKNOWN_COMMAND);     
    }
  } else {
    sendResponseCode(R_ERROR);
  }
  return -1;
}

unsigned int hasedToCommand(unsigned int data) {
  int mod = data % 10;
  data -= mod;
  unsigned int cmd = ((data % 1000) / 10);
  if (cmd == (data / 1000) && cmd % 10 == mod) {
    return cmd;
  }
  return 0;
}

char *readStringWithMarkers() {
  static boolean openCharFlag = false;
  static int counter = 0;
  static char *buff;
  char c;
  for (int i = 0; i < COMM_TIMEOUT; i++) {
    while (Serial.available() > 0) {
      c = Serial.read();
      if (c == START_CHAR && !openCharFlag) {
        openCharFlag = true;
        buff = (char*)malloc(MAX_MESSAGE_LEN);
        continue;
      }
      if (openCharFlag) {
        if (c == END_CHAR || counter >= MAX_MESSAGE_LEN - 1) {
          *buff = '\0';
          openCharFlag = false;
          buff = buff - counter;
          counter = 0;
          return buff;
        }
        *buff = c;
        buff++;
        counter++;
      }
    }
    delay(1);
  }
  return NULL;
}

void echo(char *c) {
  unsigned int command = 0;
  if (c != NULL) {
    command = parseCommand(c);
  } else {
    writeStringWithMarkers(const_cast<char*>("timeout"));
  }
  switch (command) {
    case CONNECT_TO_DEFAULT:
      writeStringWithMarkers(const_cast<char*>("CONNECT_TO_DEFAULT"));
      break;
    case CONNECT_TO_NETWORK:
      writeStringWithMarkers(const_cast<char*>("CONNECT_TO_NETWORK"));
      break;
    case START_INIT_SERVER:
      writeStringWithMarkers(const_cast<char*>("START_INIT_SERVER"));
      break;
    case GET_MAC_ADDRESS:
      writeStringWithMarkers(const_cast<char*>("GET_MAC_ADDRESS"));
      break;
    case GET_IP_ADDRESS:
      writeStringWithMarkers(const_cast<char*>("GET_IP_ADDRESS"));
      break;
    case SEND_VIB_TO_CLOUD:
      writeStringWithMarkers(const_cast<char*>("SEND_VIB_TO_CLOUD"));
      break;
    case GET_BASEURL:
      writeStringWithMarkers(const_cast<char*>("GET_BASEURL"));
      break;
    default:
      writeStringWithMarkers(const_cast<char*>("unknown command"));
      break;
  }
}

void reply(u_int8_t b, int count) {
  if (b >= 0 && b < 10) {
    char *buff = (char*)malloc(2);
    sprintf(buff, "%d", b);
    writeStringWithMarkers(buff);
    free(buff);
  }
}

void sendResponseCode(u_int8_t rc) {
  char *buff = (char*)malloc(RESP_CODE_FRAME);
  sprintf(buff, "<%d>\n", rc);
  Serial.print(buff);
  free(buff);
}

u_int8_t readResponseCode() {
  char *res = readStringWithMarkers();
  if (res == NULL) {
    return -1;
  }
  
  u_int8_t code = atoi(res);
  free(res);
  return code;
}

void writeStringWithMarkers(char *s) {
  Serial.print(START_CHAR);
  Serial.print(s);
  Serial.print(END_CHAR);
}
