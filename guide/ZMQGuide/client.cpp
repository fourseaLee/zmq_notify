#include <zmq.hpp>
#include <string>
#include <iostream>
#include "util.h"
#include <sstream>
#include <unistd.h>
#include <unordered_map>


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


    //zmq::socket_base_t *s = static_cast<zmq::socket_base_t *> (client);

    //if (!s->check_tag ()) {
     //  std::cout << " -----------------------------------" <<std::endl;
    //}


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
            zmq::pollitem_t items[1];// = { { client, 0, ZMQ_POLLIN, 0 } };
            items[0].socket = *client;
            items[0].events = ZMQ_POLLIN;
            items[0].fd = 0;
            items[0].revents = 0;
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


#define NBR_CLIENTS 10
#define NBR_WORKERS 3
#define WORKER_READY "READY"
void lvcache()
{
    zmq::context_t context(1);
    zmq::socket_t frontend(context ,ZMQ_SUB);
    zmq::socket_t backend(context, ZMQ_XPUB);

    frontend.connect("tcp://localhost:5557");
    backend.bind("tcp://*:5558");

    frontend.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    std::unordered_map<std::string, std::string> cache_map;

    zmq::pollitem_t items[2] ={
      {frontend, 0, ZMQ_POLLIN,0},
      {backend, 0, ZMQ_POLLIN, 0}
    };

    while (1)
    {
        if(zmq::poll(items, 2, 1000) == -1)
            break;
        if(items[0].revents&ZMQ_POLLIN)
        {
            std::string topic = RecieveMessage(frontend);
            std::string data = RecieveMessage(frontend);

            if(topic.empty())
                break;

            cache_map[topic] = data;
            SendMore(backend,topic);
            SendMessage(backend,data);
        }

        if (items[1].revents & ZMQ_POLLIN)
        {
            zmq::message_t msg;
            backend.recv(&msg);
            if(msg.size() == 0)
                break;

            uint8_t* event = (uint8_t *)msg.data();

            if(event[0] == 1)
            {
                std::string topic((char*)(event+1),msg.size()-1);
                auto i = cache_map.find(topic);

                if(i != cache_map.end())
                {
                    SendMore(backend,topic);
                    SendMessage(backend,i->second);
                }
            }
        }
    }
}



int main()
{
    //hwclient();
    //lpclient();
    lvcache();
    return 0;
}
