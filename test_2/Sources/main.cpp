#include <iostream>
#include <thread>
#include <zmq.hpp>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif


void pub();
void sub();
int main( void )
{

    std::thread t2(sub);
    std::thread t1(pub);
        t1.join();
        t2.join();

    return 0;
}

void pub(){
    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH );
//        ventilator.connect( "tcp://localhost:24041" );
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );

        while(1)
        {
            try {
                sleep(1000);
                std::string message_tosend;
                std::cin >> message_tosend;
                message_tosend.insert(0, "example>quest?>bram>");
                ventilator.send(message_tosend.c_str(), message_tosend.size());
                std::cout << "Pushed : " << message_tosend << std::endl;
            }
            catch(zmq::error_t& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

}

void sub(){
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB );
        //subscriber.connect( "tcp://localhost:24042" );
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "example>quest?>bram>", 20 );

        zmq::message_t * msg = new zmq::message_t();
        while(true)
        {
            subscriber.recv( msg );
            //parse
            std::string stringmsg = std::string( (char*) msg->data(), msg->size() );
            stringmsg.erase(0, 20);
            std::cout << "Subscribed : [" << stringmsg << "]" << std::endl;
        }
    }
