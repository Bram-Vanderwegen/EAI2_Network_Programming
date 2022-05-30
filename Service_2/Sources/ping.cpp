#include "ping.h"

ping::ping()
{
    pthread_t handle;
    pthread_create(&handle, NULL, *pingloop, NULL);
}

void* ping::pingloop(void* arg)
{
    zmq::context_t context(1);
    zmq::socket_t* receiver;
    receiver = new zmq::socket_t( context, ZMQ_SUB);
    receiver->connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicname("bram>ping");
    receiver->setsockopt(ZMQ_SUBSCRIBE, topicname.c_str(), topicname.size());

    zmq::socket_t* sender;
    sender = new zmq::socket_t( context, ZMQ_PUSH);
    sender->connect("tcp://benternet.pxl-ea-ict.be:24041");
    zmq::message_t * data;
    while(1){
        data = new zmq::message_t;
        receiver->recv(data);
        string data_str = (char*) data->data();

        string id = data_str.substr(data_str.find_last_of('>'), data_str.length());
        string sendata ("bram>pong");
        sendata.append(id);
        cout << sendata << endl;
        sender->send(sendata.c_str(), sendata.length());
    }
}
