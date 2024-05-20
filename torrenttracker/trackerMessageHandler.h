
#ifndef TRACKERMESSAGEHANDLER_H
#define TRACKERMESSAGEHANDLER_H

#include <iostream>
#include <string>
#include "message.h"
class TrackerMessageHandler
{
  public:
    Response handleShareRequest(std::vector<char> b);
    Response handleAddSeederRequest(std::vector<char> b);
    Response handleRemoveSeederRequest(std::vector<char> b);
    SeederInfoResponse handleGetSeedsRequest(std::vector<char> b);
    SyncSeederListResponse handleSyncSeederRequest();
};

#endif