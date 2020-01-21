#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <cstddef>
#include <cstdint>
#include "../utility/json.hpp"

typedef nlohmann::json json;

class Packet {
protected:
    std::unique_ptr<uint8_t[]> message;
    size_t processed;
    // default constructor and uint8_t * constructor 
    // used in receiving packages and should not
    // be used not in PacketQueue
    Packet();
    Packet(const uint8_t *);
    size_t remained();
    void process(size_t numberOfProcessed);
    size_t getProcessed();
    uint8_t * getPtr();
    uint32_t getJsonSize() const;
    friend class PacketQueue;
public:
    Packet(int code, json j);
    Packet(int code);
    // you don't realy want to copy packets
    // so you should move them
    Packet(const Packet &);
    Packet(Packet &&);
    Packet & operator=(const Packet &);
    Packet & operator=(Packet &&);
    json getJson() const;
    // return Action or result of server depending
    // on sending or reseiving package it is
    int32_t getFlag() const;
};

enum Action
{
    LOGIN = 1,
    LOGOUT = 2,
    MOVE = 3,
    UPGRADE = 4,
    TURN = 5,
    PLAYER = 6,
    GAMES = 7,
    MAP = 10
};

enum Result
{
    OKEY = 0,
    BAD_COMMAND = 1,
    RESOURCE_NOT_FOUND = 2,
    ACCESS_DENIED = 3,
    INAPPROPRIATE_GAME_STATE = 4,
    TIMEOUT = 5,
    INTERNAL_SERVER_ERROR = 500
};

enum GameState
{
    MENU = 0,
    INIT = 1,
    RUN = 2,
    FINISHED = 3
};

#endif // PACKAGE_HPP
