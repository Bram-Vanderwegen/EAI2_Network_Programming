#include <iostream>
#include <pthread.h>
#include <zmq.hpp>

using namespace std;


void *requestSender( void *arg );

int main()
{
    FILE * fp;
    fp = fopen("war_criminal.png", "rb");
    std::cout << fp->_base << endl;
    std::cout << fp->_bufsiz << endl;
    std::cout << fp->_charbuf << endl;
    std::cout << fp->_cnt << endl;
    std::cout << fp->_file << endl;
    std::cout << fp->_flag << endl;
    std::cout << fp->_ptr << endl;
    std::cout << fp->_tmpfname << endl;









    /*pthread_t thread2;
    pthread_create(&thread2, NULL, requestSender, NULL);
    pthread_join(thread2, NULL);*/
    cout << "Hello krokodil!" << endl;
    return 0;
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
