
#ifndef RPCHANDLER_H
#define RPCHANDLER_H

#include <string>
#include <iostream>

class RpcHandler
{
  public:
    void handleRpc(int sock_fd);
};

#endif