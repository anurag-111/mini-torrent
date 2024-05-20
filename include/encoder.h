#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <iostream>
#include <vector>

class Encoder
{
  public:
    std::vector<char> encode(std::string type,std::vector<char> b);
};

#endif