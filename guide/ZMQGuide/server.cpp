//#include <QCoreApplication>
#include <zmq.hpp>
#include <string>
#include "util.h"
#include <unistd.h>
#include <iostream>
#include <queue>
#include <pthread.h>

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

void identity()
{
    zmq::context_t context(1);
    zmq::socket_t sink(context, ZMQ_ROUTER);
    sink.bind("inproc://example");

    zmq::socket_t anonymous(context,ZMQ_REQ);
    anonymous.connect("inproc://example");

    SendMessage(anonymous, "ROUTER uses a generated 5 byte identity");
    socketDump(sink);

    zmq::socket_t identified(context, ZMQ_REQ);
    identified.setsockopt(ZMQ_IDENTITY, "PEER2", 5);
    identified.connect("inproc://example");

    SendMessage(identified, "ROUTER socket uses REQ's socket identity");
    socketDump(sink);

}
static void* clientThread(void* arg)
{
    zmq::context_t context(1);
    zmq::socket_t client(context, ZMQ_REQ);
    socketSetId(client);
    client.connect("ipc://frontend.ipc");
    SendMessage(client, "HELLO");
    std::string reply = RecieveMessage(client);
    std::cout << "client: " << reply << std::endl;
    return (NULL);
}

static void* workerThread(void* arg)
{
    zmq::context_t context(1);
    zmq::socket_t worker(context, ZMQ_REQ);

    socketSetId(worker);
    worker.connect("ipc://backend.ipc");

    SendMessage(worker, "READY");
    while (1)
    {
        std::string address = RecieveMessage(worker);
        {
            std::string empty = RecieveMessage(worker);
            assert(empty.size() == 0);
        }
        std::string request = RecieveMessage(worker);
        std::cout  << "Worker: " << request << std::endl;

        SendMore(worker, address);
        SendMore(worker, "");
        SendMore(worker, "OK");
    }
    return (NULL);
}

void broker()
{
    zmq::context_t context(1);
    zmq::socket_t frontend(context, ZMQ_ROUTER);
    zmq::socket_t backend(context, ZMQ_ROUTER);

    frontend.bind("ipc://frontend.ipc");
    backend.bind("ipc://backend.ipc");

    int client_nbr;
    for(client_nbr =0; client_nbr < 10; client_nbr++)
    {
        pthread_t client ;
        pthread_create(&client, NULL, clientThread, (void*)(intptr_t)client_nbr);
    }

    int worker_nbr;
    for(worker_nbr = 0; worker_nbr < 3; worker_nbr ++)
    {
        pthread_t worker;
        pthread_create(&worker, NULL, workerThread, (void *)(intptr_t)worker_nbr);
    }

    std::queue<std::string> worker_queue;

    while (1)
    {
        zmq::pollitem_t items[] =
        {
            {backend, 0, ZMQ_POLLIN, 0},
            {frontend, 0, ZMQ_POLLIN, 0}
        };

        if(worker_queue.size())
            zmq::poll(&items[0], 2, -1);
        else
            zmq::poll(&items[1],1, -1);


        if(items[0].revents & ZMQ_POLLIN)
        {
            worker_queue.push(RecieveMessage(backend));
            {
                std::string empty =RecieveMessage(backend);
                assert(empty.size() == 0);
            }

            std::string  client_addr = RecieveMessage(backend);

            if(client_addr.compare("READY") != 0)
            {
                {
                    std::string empty = RecieveMessage(backend);
                    assert(empty.size() == 0);
                }
                std::string reply = RecieveMessage(backend);
                SendMore(frontend, client_addr);
                SendMore(frontend, "");
                SendMessage(frontend,reply);
                if(--client_nbr == 0)
                    break;
            }
        }

        if(items[1].revents & ZMQ_POLLIN)
        {
            std::string client_addr = RecieveMessage(frontend);
            {
                std::string empty = RecieveMessage(frontend);
                assert(empty.size() == 0);
            }

            std::string request = RecieveMessage(frontend);
            std::string worker_addr = worker_queue.front();
            worker_queue.pop();
            SendMore(backend,worker_addr);
            SendMore(backend,"");
            SendMore(backend,client_addr);
            SendMore(backend,"");
            SendMessage(backend,request);
        }
    }
}



int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//   hwserver();
//      lpcserver();
    //   identity();
    //    return a.exec();
    broker();
    return 0;
}
