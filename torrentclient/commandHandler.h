#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <iostream>
#include <string>
#include "message.h"

class CommandHandler
{
  public:
    void handleCommand(std::string);
    void printResponse(Response);
    void printResponse(std::string msg_type, Response res);
    void printError(std::string e);
};

#endif
