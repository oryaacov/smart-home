#include <cstdint>

#ifndef WIFI_H
#define WIFI_H

void connectToWifiUsingWPA(const char *ssid, const char *pwd);
uint8_t connectToNetwork();
void echoWifiStringToMaster(char *ssid, char *pwd);
char* listenAndWait();
void handleRoot();
void handleNotFound();
void handleInit();
bool isValidArg(char *arg);
char* getMacAddress();
void sendPOST(int vibration);
char* getIPAddress();

#endif