#include <iostream>
#include <pthread.h>
#include <zmq.hpp>

using namespace std;

void *requestCollector( void *arg );
void *requestSender( void *arg );
void responder();

int main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, requestCollector, NULL);
    pthread_create(&thread2, NULL, requestSender, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    cout << "Hello krokodil!" << endl;
    return 0;
}

void *requestCollector(void *arg){

    //connection setup
    zmq::context_t context(1);
    zmq::socket_t netScanner( context, ZMQ_SUB);
    netScanner.connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicName = "";
    netScanner.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());

    zmq::message_t * msg = new zmq::message_t();
    while(1)
    {
        netScanner.recv(msg);
        //parse
        string stringmsg = string((char*) msg->data(), msg->size());
        stringmsg.erase(0, topicName.size());
        cout << "received " << stringmsg << endl;
    }

}

void *requestSender(void *arg){

    //connection setup
    zmq::context_t context(1);
    zmq::socket_t pusher( context, ZMQ_PUSH);
    pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");

    while(1)
    {
        try {
            std::string message_tosend;
            std::cin >> message_tosend;
            message_tosend.insert(0, "");
            pusher.send(message_tosend.c_str(), message_tosend.size());
            std::cout << "Pushed : " << message_tosend << std::endl;
        }
        catch(zmq::error_t& e) {
            std::cout << e.what() << std::endl;
        }
    }

}
