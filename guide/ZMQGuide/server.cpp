//#include <QCoreApplication>
#include <zmq.hpp>
#include <string>
#include "util.h"
#include <unistd.h>
#include <iostream>

void hwserver()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");
    while (true)
    {
        zmq::message_t request;
        socket.recv(&request);
        std::cout << "Recieve hello" << std::endl;

        sleep(1);

        zmq::message_t reply (5);
        memcpy((void*)reply.data(), "World", 5);
        socket.send(reply);
    }
}

#define within(num) (int) ((float) (num) * random () / (RAND_MAX + 1.0))
void lpcserver()
{
    srandom((unsigned) time (NULL));
    zmq::context_t context(1);
    zmq::socket_t server(context, ZMQ_REP);
    server.bind("tcp://*:5555");

    int cycles = 0;

    while(1)
    {
        std::string request = RecieveMessage(server);
        cycles ++;
        if(cycles > 3 && within(3) == 0)
        {
            std::cout << "I:  simalating CPU overload" << std::endl;
            sleep(2);
        }
        std::cout << "I: normal request (" << request << ")" << std::endl;
        sleep(1);
        SendMessage(server, request);
    }

}
int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//    hwserver();
      lpcserver();
//    return a.exec();
    return 0;
}
