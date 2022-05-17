#include <iostream>
#include <pthread.h>
#include <vector>
#include <zmq.hpp>
#include <fstream>
#include <QProcess>

#include <color_extractor_service.h>

using namespace std;

void* color_extractor_function(void *arg);

int main()
{
    pthread_t color_handle;
    pthread_create(&color_handle, NULL, *color_extractor_function, NULL);
    pthread_join(color_handle, NULL);

    cout << "Hello krokodil!" << endl;
    return 0;
}

void* color_extractor_function(void *arg){

    //socket setup
    zmq::context_t context(1);
    zmq::socket_t receiver( context, ZMQ_SUB);
    //make connection
    receiver.connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicName = "bram>imageeditC";
    receiver.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());

    //loop receiving
    zmq::message_t* datapayload = new zmq::message_t;
    Color_extractor_service* current_service;
    while(1){
        //receive file
        receiver.recv(datapayload);
        current_service = new Color_extractor_service(datapayload->to_string(), datapayload->size(), topicName);
        delete current_service;
    }
}





