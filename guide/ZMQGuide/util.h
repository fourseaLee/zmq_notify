#ifndef UTIL_H
#define UTIL_H
#include <zmq.h>
#include <zmq.hpp>
#include <string>
#include <iomanip>
#include <string.h>
#include <iostream>


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

static void  socketDump(zmq::socket_t& socket)
{
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    while(1)
    {
        zmq::message_t message;
        socket.recv(&message);

        int size = message.size();
        std::string data(static_cast<char*>(message.data()),size);
        bool is_text = true;

        int char_nbr;
        unsigned char byte;
        for(char_nbr = 0; char_nbr < size; char_nbr ++)
        {
            byte = data[char_nbr];
            if(byte < 32 || byte > 127)
                is_text = false;
        }
        std::cout  << "[" << std::setfill('0') << std::setw(3) << size << "]" << std::endl;

        for(char_nbr =0; char_nbr < size; char_nbr++)
        {
            if(is_text)
                std::cout << (char)data[char_nbr];
            else
                std::cout << std::setfill('0') << std::setw(2)
                          << std::hex << (unsigned int) data[char_nbr];
        }
        std::cout << std::endl;

        int more = 0;
        size_t more_size =sizeof(more);
        socket.getsockopt(ZMQ_RCVMORE,&more, &more_size);
        if(!more)
            break;
    }
}
#define within(num) (int) ((float) (num) * random () / (RAND_MAX + 1.0))
static std::string socketSetId(zmq::socket_t &socket)
{
    std::stringstream ss;
    ss << std::hex  << std::uppercase
       << std::setw(4) << std::setfill('0') << within(0x10000) << "-"
       << std::setw(4) << std::setfill('0') << within(0x10000);
    socket.setsockopt(ZMQ_IDENTITY, ss.str().c_str(), ss.str().length());
    return ss.str();
}

#endif // UTIL_H
