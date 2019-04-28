#include <zmq.hpp>
#include <string>
#include <iostream>
#include "util.h"
#include <sstream>
#include <unistd.h>

void hwclient()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    std::cout << "connect to hello world server" << std::endl;
    socket.connect("tcp://localhost:5555");

    for(int i =0; i < 10; i++)
    {
        zmq::message_t request(6);
        memcpy((void*) request.data(), "Hello", 5);
        std::cout << "Sending Hello " << i << "...." << std::endl;
        socket.send(request);

        zmq::message_t reply;
        socket.recv(&reply);
    }
}

#define REQUEST_TIMEOUT 2500
#define REQUEST_RETRIES 3

static zmq::socket_t* ClientSocket(zmq::context_t& context)
{
    std::cout << "I : connecting to server ..." << std::endl;
    zmq::socket_t * client = new zmq::socket_t (context, ZMQ_REQ);
    client->connect("tcp://localhost:5555");

    int linger = 0;
    client->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    return client;
}

void lpclient()
{
    zmq::context_t context(1);
    zmq::socket_t *client = ClientSocket(context);

    int sequence = 0;
    int retries_left = REQUEST_RETRIES;
    while(retries_left)
    {
        std::stringstream request;
        request << ++sequence;
        SendMessage(*client, request.str());
        sleep(1);

        bool expect_reply = true;
        while(expect_reply)
        {
            //zmq::pollitem_t items[] = {{*client, 0, ZMQ_POLLIN, 0}};
            zmq::pollitem_t items[] = { { client, 0, ZMQ_POLLIN, 0 } };
            zmq::poll(&items[0], 1, REQUEST_TIMEOUT * 1000);

            if(items[0].revents & ZMQ_POLLIN)
            {
                std::string reply = RecieveMessage(*client);
                if(std::atoi(reply.c_str()) == sequence)
                {
                    std::cout << "I: server replied OK (" << reply <<  ")" << std::endl;
                    retries_left = REQUEST_RETRIES;
                    expect_reply = false;
                }
                else
                {
                    std::cout << "E: malformed reply from server : " << reply << std::endl;
                }
            }
            else  if(--retries_left == 0)
            {
                std::cout << "E: server seems to be offline, abandoning" << std::endl;
                expect_reply = false;
                break;
            }
            else
            {
                std::cout << "W: no response from server, retrying..." << std::endl;
                delete client;
                client = ClientSocket(context);
                SendMessage(*client, request.str());
            }
        }
    }
    delete client;

}

int main()
{
    //hwclient();
    lpclient();
    return 0;
}
