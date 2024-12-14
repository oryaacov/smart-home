#include <cstdint>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

unsigned int parseCommand(char *s);
bool isKnownCommand(unsigned int cmd);
unsigned int receiveCommandFromMaster();
unsigned int hasedToCommand(unsigned int data);
char *readStringWithMarkers();
void echo(char *c);
void reply(byte b, int count);
void sendResponseCode(byte rc);
u_int8_t readResponseCode();
void writeStringWithMarkers(char *s);

#endif
