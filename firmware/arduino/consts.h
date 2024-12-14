#ifndef CONSTANTS_H
#define CONSTANTS_H

// WiFi credentials
const char SECRET_SSID[] = "a";
const char SECRET_PASS[] = "a";

// Command codes
const int HANDSHAKE = 255;
const int CONNECT_TO_DEFAULT = 20;
const int CONNECT_TO_NETWORK = 21;
const int START_INIT_SERVER = 22;
const int GET_MAC_ADDRESS = 23;
const int GET_IP_ADDRESS = 24;
const int SEND_VIB_TO_CLOUD = 25;
const int GET_BASEURL = 26;
const int GET_CS = 27;

// Buffer size
const int INIT_BUFF_SIZE = 200;

// Response codes
const int R_UNKNOWN_COMMAND = 0;
const int R_SUCCESS = 1;
const int R_ERROR = 2;
const int R_RECEIVED = 3;
const int R_EMPTY = 4;
const int R_FAILED = 5;
const int R_ACK = 6;
const int R_PROCESSING = 7;
const int R_TIMEOUT = 8;

// Message constants
const int MAX_MESSAGE_LEN = 100;
const char END_TAG[] = "<psend>";
const char START_TAG[] = "<psstart>";

// Communication constants
const int COMM_TIMEOUT = 10000;

// Modes
const int PROD_MODE = 0;
const int ECHO_MODE = 1;

// Frame codes
const int COMMAND_FRAME = 13;
const int RESP_CODE_FRAME = 10;

// Delimiters
const char END_CHAR = '>';
const char START_CHAR = '<';

#endif // CONSTANTS_H
