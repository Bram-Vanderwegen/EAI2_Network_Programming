#include <iostream>
#include <pthread.h>
#include <zmq.hpp>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;


void* requestSender( void *arg );
void* requestReceiver(void* arg);

void* ping_sender(void* arg);
void* help_function(void* arg);

string* create_randnum();

int main()
{
    //start sender tread
    pthread_t thread1;
    pthread_create(&thread1, NULL, requestSender, NULL);
    pthread_join(thread1, NULL);

    cout << "Hello krokodil!" << endl;
    return 0;
}



void *requestSender(void *arg){

    //connection setup
    zmq::context_t context(1);
    zmq::socket_t pusher( context, ZMQ_PUSH);
    pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    cout << "typ 'help' voor info" << endl;
    while(1)
    {
        try {
            //get filename
            std::string filename;
            std::cin >> filename;
            if(filename == string("ping")){
                pthread_create(NULL, NULL, &ping_sender, NULL);
                continue;
            }
            else if(filename == string("help")){
                help_function(NULL);
                continue;
            }
            //open and read file
            ifstream fp;
            fp.open(filename, ifstream::binary);
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
                    string message = "bram>imageeditC>";

                    string* id = create_randnum();
                    message.append(*id);
                    message.append(filename);
                    message.append(">");
                    //add data
                    for(int i = 0; i < length; i++){
                        message.push_back(buffer[i]);
                    }
                    //starts receiver thread and sends subscribe string
                    pthread_create(NULL, NULL, requestReceiver, (void*) (id));
                    //push
                    pusher.send(message.c_str(), message.length());
                    std::cout << "Pushed : " << filename << " to server." << std::endl;

            }
            else{
                cout << "error, try again" << endl;
            }
        }
        catch(zmq::error_t& e) {
            cout << "zmq error: ";
            std::cout << e.what() << std::endl;
        }
    }

}

void* requestReceiver(void* arg){
    //connection setup
    zmq::context_t context(1);
    zmq::socket_t receiver( context, ZMQ_SUB);

    //cout << * (string*) arg << endl;

    receiver.connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicName = "bram>imageeditS>";
    topicName.append(*(string*) arg);
    //cout << topicName << endl;
    receiver.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());

    zmq::message_t* datapayload = new zmq::message_t;
    for(int i = 0; i < 3; i++){
        receiver.recv(datapayload);
        cout << "received something: " << endl;
        string limited = string((char*) datapayload->data(), datapayload->size());
        limited.erase(0, topicName.size());
        string filename = limited.substr(0, limited.find_first_of('>'));


        ofstream filerReceiveptr;
        filerReceiveptr.open(filename, ofstream::binary);
        if(filerReceiveptr.is_open()){
            for(int i = limited.find_first_of('>') + 1; i < datapayload->size(); i++){
                filerReceiveptr.put(limited[i]);
            }
            cout << "wrote file: " << filename << endl;
        }

    }
    delete datapayload;
    delete (string*) arg;
}

void* ping_sender(void* arg){
    //connection setup
    zmq::context_t context(1);
    zmq::socket_t pusher( context, ZMQ_PUSH);
    pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    string* id = create_randnum();
    string payload = "bram>ping>";
    payload.append(*id);
    //cout << payload << endl;
    pusher.send(payload.c_str(), payload.size());


    zmq::socket_t receiver(context, ZMQ_SUB);
    receiver.connect("tcp://benternet.pxl-ea-ict.be:24042");
    string topicName = "bram>pong>";
    topicName.append(*id);
    receiver.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());
    zmq::message_t* void_mess;
    void_mess = new zmq::message_t;
    receiver.recv(void_mess);
    cout << "pong" << endl;
    delete void_mess;
    delete id;
}

void* help_function(void* arg){
    cout << "###############" << endl;
    cout << "# Handleiding #" << endl;
    cout << "###############" << endl << endl;;
    cout << "-------------------" << endl;
    cout << "- Kleurensplitser -" << endl;
    cout << "-------------------" << endl;
    cout << "Om de kleurenservice te gebruiken moet in dezelfde map als de executable een image in bitmap formaat gestoken worden." << endl;
    cout << "Deze bitmap moet als resolutie een veelvoud van 4 hebben." << endl;
    cout << "typ in de console de naam van de image met extensie." << endl;
    cout << "De server zal vervolgens de 3 kleurenimages terugsturen." << endl;
    cout << "(Dit kan even duren, afhankelijk van de grootte van de image." << endl << endl;
    cout << "-------------" << endl;
    cout << "- ping pong -" << endl;
    cout << "-------------" << endl;
    cout << "Voor de ping service te gebruiken moet er 'ping' worden ingetypt, waarna er pong wordt teruggestuurd." << endl << endl;
    cout << "###############" << endl;
}

string* create_randnum(){
    srand(time(NULL));
    //creating char buffer to convert int to char
    char conversionbuffer[6];
    //clearing the buffer to prevent undefined behavior
    memset(conversionbuffer, 'a', 5);
    int number = 10000 + (rand()) % 89999;
    itoa(number, conversionbuffer, 10);
    /*conversionbuffer[0] = 'a';
    cout << conversionbuffer << endl;*/
    return(new string(conversionbuffer));

}
