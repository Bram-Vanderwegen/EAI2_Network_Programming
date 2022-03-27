#include <iostream>
#include <thread>
#include <zmq.hpp>

using namespace std;

void requestCollector();
void responder();

int main()
{
    thread t1(requestCollector);
    t1.join();
    cout << "Hello krokodil!" << endl;
    return 0;
}

void requestCollector(){

    //connection setup
    zmq::context_t context(1);
    zmq::socket_t netScanner( context, ZMQ_SUB);
    netScanner.connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicName = "service>bram>test>";
    netScanner.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());

    zmq::message_t * msg = new zmq::message_t();
    while(1)
    {
        netScanner.recv(msg);
        //parse
        string stringmsg = string((char*) msg->data(), msg->size());
        stringmsg.erase(0, topicName.size());
        cout << stringmsg << endl;
    }

}
