#include "TrackerServiceServer.h"
#include "encoder.h"
#include "networkInterfacer.h"
#include "utils.h"
#include "decoder.h"
#include <syslog.h>
#include <arpa/inet.h>
#include <string.h>
#include "utils.h"
#include "unistd.h"
#include "errorMsg.h"
using std::cout;
using std::endl;

TrackerServiceServer::TrackerServiceServer(Seeder tracker1, Seeder tracker2)
{
    try
    {
        //cout << "TrackerServiceServer() tracker1: " << tracker1.getIp() << ", " << tracker1.getPort() << endl;
        this->tracker_fd = createTCPClient(tracker1);
    }
    catch (ErrorMsg e)
    {
        //cout << "TrackerServiceServer() tracker2: " << tracker2.getIp() << ", " << tracker2.getPort() << endl;
        try
        {
            this->tracker_fd = createTCPClient(tracker2);
        }
        catch (std::exception e)
        {
            throw ErrorMsg("Unable to establish connection");
        }
    }
    // cout << "### Created TrackerServiceServer with fd: " << this->tracker_fd << endl;
}

TrackerServiceServer::TrackerServiceServer(Seeder tracker1)
{
    try
    {
        //cout << "TrackerServiceServer() tracker1: " << tracker1.getIp() << ", " << tracker1.getPort() << endl;
        this->tracker_fd = createTCPClient(tracker1);
    }
    catch (std::exception e)
    {
        throw ErrorMsg("Unable to establish connection");
    }
    //cout << "### Created TrackerServiceServer with fd: " << this->tracker_fd << endl;
}

TrackerServiceServer::~TrackerServiceServer()
{
    syslog(LOG_INFO, "### Closing TrackerServiceServer() with fd: %d", this->tracker_fd);
    close(this->tracker_fd);
}

Response TrackerServiceServer::shareFile(Share msg)
{
    //cout << "in shareFile() with msg: " <<  msg.getFileName() << endl;
    Encoder encoder;
    auto b = encoder.encode("SHARE", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    // cout << "in shareFile() witing rpc & request of size: " <<  b.size() << endl;
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    //cout << "in shareFile() about to read response from fd: " <<  this->tracker_fd << endl;
    auto response_b = reader.readFromNetwork();
    //cout << "in shareFile() received response of size: " <<  response_b.size() << endl;
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    //cout << "in shareFile() decoded msg type: " <<  msg_pair.first << endl;
    return Response(msg_pair.second);
}
Response TrackerServiceServer::addSeederRequest(AddSeeder msg)
{
    Encoder encoder;
    auto b = encoder.encode("ADDSEEDER", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}

Response TrackerServiceServer::removeSeederRequest(RemoveSeeder msg)
{
    Encoder encoder;
    auto b = encoder.encode("REMOVESEEDER", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}

//-----------------------------------------------Sync messages-------------------------------------

Response TrackerServiceServer::syncshareFile(SyncShare msg)
{
    //cout << "in shareFile() with msg: " <<  msg.getFileName() << endl;
    Encoder encoder;
    auto b = encoder.encode("SYNCSHARE", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    // cout << "in shareFile() witing rpc & request of size: " <<  b.size() << endl;
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    //cout << "in shareFile() about to read response from fd: " <<  this->tracker_fd << endl;
    auto response_b = reader.readFromNetwork();
    //cout << "in shareFile() received response of size: " <<  response_b.size() << endl;
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    //cout << "in shareFile() decoded msg type: " <<  msg_pair.first << endl;
    return Response(msg_pair.second);
}
Response TrackerServiceServer::syncaddSeederRequest(SyncAddSeeder msg)
{
    Encoder encoder;
    auto b = encoder.encode("SYNCADDSEEDER", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}

Response TrackerServiceServer::syncremoveSeederRequest(SyncRemoveSeeder msg)
{
    Encoder encoder;
    auto b = encoder.encode("SYNCREMOVESEEDER", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    return Response(msg_pair.second);
}

SeederInfoResponse TrackerServiceServer::getSeederInfo(SeederInfoRequest msg)
{
    Encoder encoder;
    auto b = encoder.encode("SEEDERINFOREQUEST", msg.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    //std::cout << "getSeederInfo Check" << std::endl;
    NetworkReader reader(this->tracker_fd);
    //std::cout << "getSeederInfo Check2" << std::endl;
    auto response_b = reader.readFromNetwork();
    //std::cout << "getSeederInfo received response size: " << response_b.size() <<std::endl;
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    //std::cout << "getSeederInfo response msg type after decoding: " << msg_pair.first <<std::endl;
    //std::cout << "getSeederInfo response msg size after decoding: " << msg_pair.second.size() <<std::endl;
    return SeederInfoResponse(msg_pair.second);
}

ChunkInfoResponse TrackerServiceServer::getChunkInfo(ChunkInfoRequest req)
{
    Encoder encoder;
    auto b = encoder.encode("CHUNKINFOREQUEST", req.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    // std::cout << "getSeederInfo response msg type after decoding: " << msg_pair.first <<std::endl;
    //std::cout << "getSeederInfo response msg size after decoding: " << msg_pair.second.size() <<std::endl;
    return ChunkInfoResponse(msg_pair.second);
}

SendChunkResponse TrackerServiceServer::getChunk(SendChunkRequest req)
{
    Encoder encoder;
    auto b = encoder.encode("SENDCHUNKREQUEST", req.getBytes());
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    // std::cout << "getChunk response msg type after decoding: " << msg_pair.first <<std::endl;
    //std::cout << "getChunk response msg size after decoding: " << msg_pair.second.size() <<std::endl;
    return SendChunkResponse(msg_pair.second);
}

SyncSeederListResponse TrackerServiceServer::syncSeederFile(SyncSeederListRequest msg)
{
    Encoder encoder;
    auto b = encoder.encode("SYNCSEEDERLISTREQUEST", msg.getBytes());
    //std::cout << "sending " << b.size() << " bytes" << std::endl;
    NetworkWriter writer(this->tracker_fd);
    writer.writeToNetwork(b);

    NetworkReader reader(this->tracker_fd);
    auto response_b = reader.readFromNetwork();
   // response_b = reader.readFromNetwork();
    //std::cout << "In Tracker Respone for sync, resp size: " << response_b.size() << std::endl;
    Decoder decoder;
    auto msg_pair = decoder.decodeMsgType(response_b);
    //std::cout << "In Tracker Respone for sync, actual msg size: " << msg_pair.second.size() << std::endl;
    return SyncSeederListResponse(msg_pair.second);
}