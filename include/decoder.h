
#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "message.h"

class Decoder {
    public:
    // std::shared_ptr<Message> decode(std::vector<char>);
    std::pair<std::string, std::vector<char>> decodeMsgType(std::vector<char> b);
};

#endif