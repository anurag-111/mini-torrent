#include "fileHandler.h"
#include "clientDatabase.h"
#include <fstream>
#include <syslog.h>
#include <math.h>
#include "mtorrent.h"
#include "seeder.h"
#include "openssl/sha.h"
#include "errorMsg.h"
#include <errno.h>
#include <string.h>

using std::cout;
using std::endl;

//generating mtorrent file
void FileHandler::createMTorrent(mTorrent_Sptr torr)
{
    std::ofstream myfile;
    myfile.open(torr->getMTorrPath().c_str());
    //myfile.exceptions(std::ios::badbit);
    if (myfile.bad())
    {
        throw ErrorMsg("Invalid torrent path");
    }
    if (myfile.is_open())
    {
        // auto filename = torr->getPath().substr(torr->getPath().find_last_of("/")+1);
        // if (filename.size() == 0)
        // {
        //     throw ErrorMsg("Invalid Path");
        // }
        syslog(0, "Writing file to: [%s]", torr->getPath().c_str());
        myfile << torr->getHash() << "\n";
       // cout << "In mtorr constructor, filename: " << torr->getfileName() << endl;
        myfile << torr->getfileName() << "\n";
        myfile << torr->getFileSize() << "\n";
        myfile << ClientDatabase::getInstance().getTracker1().getIp() << "\n";
        myfile << ClientDatabase::getInstance().getTracker1().getPort() << "\n";
        myfile << ClientDatabase::getInstance().getTracker2().getIp() << "\n";
        myfile << ClientDatabase::getInstance().getTracker2().getPort() << "\n";
    }
    myfile.close();
}

mTorrent_Sptr FileHandler::readMTorrent(std::string file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in | std::ios::binary);
    //myfile.exceptions(std::ios::badbit);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    std::string str;
    mTorrent_Sptr mFile = std::make_shared<mTorrent>();

    //path
    mFile->setPath(file_name);

    //hash
    std::getline(file, str);
    mFile->setHash(str);

    //filename
    std::getline(file, str);
    mFile->setFileName(str);

    //path
    std::getline(file, str);
    mFile->setFileSize(std::stol(str));

    mFile->setBitChunks(std::vector<u_int32_t>(ceil((std::stol(str) * 1.0000) / CHUNK_SIZE), 1));

    return mFile;
}

//getting the hash of the file
std::string FileHandler::getFileHash(std::string file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in | std::ios::binary);
    //myfile.exceptions(std::ios::badbit);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    std::string hash = "";
    int chunk_count = ceil((file_size * 1.00) / CHUNK_SIZE);
    for (int i = 0; i < chunk_count; i++)
    {
        char data[CHUNK_SIZE + 1];
        file.seekg(i * CHUNK_SIZE);
        size_t length;
        if (i == chunk_count - 1)
        {
            file.read(data, file_size % CHUNK_SIZE);
            data[file_size % CHUNK_SIZE] = '\0';
            length = file_size % CHUNK_SIZE;
        }
        else
        {
            file.read(data, CHUNK_SIZE);
            data[CHUNK_SIZE] = '\0';
            length = CHUNK_SIZE;
        }

        unsigned char hash_buff[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char *>(data), length, hash_buff);

        char mdString[SHA_DIGEST_LENGTH * 2];

        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        {
            sprintf(&mdString[i * 2], "%02x", (unsigned int)hash_buff[i]);
        }
        std::string chunk_hash = std::string((char *)mdString);
        hash += chunk_hash.substr(0, 20);
        //syslog(0, "Chunk Hash: [%s]", chunk_hash.c_str());
    }
    return hash;
}

long FileHandler::fileSize(std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize file_size = file.gcount();
    file.close();
    return file_size;
}

void FileHandler::readFileChunk(int chunk_index, std::string path, std::vector<char> &buffer)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.good())
    {
        throw ErrorMsg("File does not exists");
    }
    auto fsize = this->fileSize(path);
    auto chunksize = (fsize - (CHUNK_SIZE * chunk_index)) > CHUNK_SIZE ? CHUNK_SIZE : (fsize - (CHUNK_SIZE * chunk_index));
    file.seekg(CHUNK_SIZE * chunk_index);
    buffer.resize(chunksize);
    //cut it off to available bytes
    // create a buffer
    file.read(&buffer[0], buffer.size()); // read to buffer
    file.close();
}

void FileHandler::createEmptyFile(std::string path, long size)
{
    //create empty file
    std::fstream outfile;
    outfile.open(path, std::ios::trunc | std::ios::out | std::ios::binary);
    //cout << "In Filehandler, Creating empty file, file path: " << path << endl;
    if (outfile.is_open())
    {
        //std::cout << "In Filehandler, Creating empty file of size: " << size << std::endl;
        std::vector<char> buf(size);
        //std::cout << "In Filehandler, Creating buffer of size: " << size << std::endl;
        outfile.write(buf.data(), size);
        //std::cout << "In Filehandler, Created empty file of size: " << size << std::endl;
    }
    else
    {
        throw ErrorMsg("Could not create file: " + std::string(strerror(errno)));
        //std::cout << "In Filehandler, unable to create empty file of size: " << size << std::endl;
    }
    outfile.close();
}