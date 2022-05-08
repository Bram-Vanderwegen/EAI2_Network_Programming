#include <iostream>
#include <pthread.h>
#include <zmq.hpp>
#include <fstream>

using namespace std;


void *requestSender( void *arg );

int main()
{
    /*ifstream fp;
    fp.open("war_criminal.bmp", ifstream::binary);
    if(fp.is_open()){
            cout << "open" << endl;
            // checks for length
            fp.seekg (0, fp.end);
            int length = fp.tellg();
            fp.seekg (0, fp.beg);
            //copys
            char * buffer = new char[length];
            fp.read ((char*) &buffer[0] ,length);

            //write data
            ofstream fpw;
            fpw.open("copy.bmp", ofstream::binary);
            if(fpw.is_open()){
                cout << "open" << endl;
                for(int i = 0; i < length; i++){
                    fpw << buffer[i];
                }
            }
            else{
                cout << "not open" << endl;
            }
            delete[] buffer;
    }*/





    pthread_t thread2;
    pthread_create(&thread2, NULL, requestSender, NULL);
    pthread_join(thread2, NULL);

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

            ifstream fp;
            fp.open(message_tosend, ifstream::binary);
            if(fp.is_open()){
                    cout << "open" << endl;
                    // checks for length
                    fp.seekg (0, fp.end);
                    int length = fp.tellg();
                    fp.seekg (0, fp.beg);
                    //copys
                    char * buffer = new char[length];
                    fp.read ((char*) &buffer[0] ,length);

                    //add delim

                    string message = "bram>image>";
                    for(int i = 0; i < 100; i++){
                        message.push_back(buffer[i]);
                    }


                    message_tosend.insert(0, "");
                    pusher.send(message.c_str(), message.length());
                    std::cout << "Pushed : " << message_tosend << std::endl;
                    delete [] buffer;
            }
            else{
                cout << "try again" << endl;
            }
        }
        catch(zmq::error_t& e) {
            std::cout << e.what() << std::endl;
        }
    }

}
