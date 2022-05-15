#include <iostream>
#include <pthread.h>
#include <zmq.hpp>
#include <fstream>

using namespace std;


void *requestSender( void *arg );
void* requestReceiver(void* arg);

int main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, requestReceiver, NULL);
    pthread_create(&thread2, NULL, requestSender, NULL);
    pthread_join(thread1, NULL);
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
            //get filename
            std::string message_tosend;
            std::cin >> message_tosend;
            //open and read file
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
                    string message = "bram>imageedit>";
                    message.append(message_tosend);
                    message.append(">");
                    //add data
                    for(int i = 0; i < length; i++){
                        message.push_back(buffer[i]);
                    }
                    //push
                    pusher.send(message.c_str(), message.length());
                    std::cout << "Pushed : " << message_tosend << std::endl;
                    delete [] buffer;
            }
            else{
                cout << "error, try again" << endl;
            }
        }
        catch(zmq::error_t& e) {
            std::cout << e.what() << std::endl;
        }
    }

}

void* requestReceiver(void* arg){
    //connection setup
    zmq::context_t context(1);
    zmq::socket_t receiver( context, ZMQ_SUB);

    receiver.connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicName = "bram>imageedit>";
    receiver.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());

    zmq::message_t* datapayload = new zmq::message_t;
    while(1){
        receiver.recv(datapayload);
        string limited = string((char*) datapayload->data(), datapayload->size());
        limited.erase(0, topicName.size());


        ofstream filerReceiveptr;
        filerReceiveptr.open("received.bmp", ofstream::binary);
        if(filerReceiveptr.is_open()){
            for(int i = 0; i < datapayload->size(); i++){
                filerReceiveptr.put(limited[i]);
            }
            cout << "writtend" << endl;
        }
    }
}

