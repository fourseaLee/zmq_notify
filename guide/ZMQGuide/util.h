#ifndef UTIL_H
#define UTIL_H
#include <zmq.h>
#include <zmq.hpp>
#include <string>
#include <iomanip>
#include <string.h>


static std::string RecieveMessage(zmq::socket_t& socket)
{
    zmq::message_t message;
    socket.recv(&message);
    return std::string(static_cast<char*>(message.data()),message.size());
}

static bool SendMessage(zmq::socket_t& socket, const std::string & data)
{
    zmq::message_t message(data.size());
    memcpy(message.data(),data.data(), data.size());
    bool rc = socket.send(message);
    return (rc);
}

static bool SendMore(zmq::socket_t& socket, const std::string& data)
{
    zmq::message_t message(data.size());
    memcpy(message.data(), data.data(), data.size());
    bool rc = socket.send(message,ZMQ_SNDMORE);
    return (rc);
}

#endif // UTIL_H
