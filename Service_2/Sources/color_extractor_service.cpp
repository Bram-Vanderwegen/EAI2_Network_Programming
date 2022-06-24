#include "color_extractor_service.h"
#include "zmq.hpp"
#include <fstream>
#include <qprocess.h>

using namespace std;

Color_extractor_service::Color_extractor_service()
{

    //start receiver thread
    pthread_t receiverHandle;
    pthread_t pusherHandle;
    pthread_create(&receiverHandle, NULL, *receiveloop, NULL);
    pthread_create(&pusherHandle, NULL, *send_loop, NULL);
}

Color_extractor_service::~Color_extractor_service()
{
}

void* Color_extractor_service::receiveloop(void* arg)
{

    //socket setup
    zmq::context_t context(1);
    zmq::socket_t receiver( context, ZMQ_SUB);
    //make connection
    receiver.connect("tcp://benternet.pxl-ea-ict.be:24042");
    receiver.setsockopt(ZMQ_SUBSCRIBE, topicName.c_str(), topicName.size());

    //loop receiving
    pthread_t* currentThread;

    while(1){
        //receive file
        zmq::message_t* datapayload = new zmq::message_t;
        receiver.recv(datapayload);
        currentThread = new pthread_t;
        pthread_create(currentThread, NULL, main, (void*) datapayload);
    }
}

void* Color_extractor_service::send_loop(void* arg)
{
    //create socket
    zmq::context_t context(1);
    zmq::socket_t* pusher;
    pusher = new zmq::socket_t( context, ZMQ_PUSH);
    pusher->connect("tcp://benternet.pxl-ea-ict.be:24041");
    //prelocks itself
    pthread_mutex_lock( &insertion_mutex);
    //starts while looping
    while(1){
        //pauses itself and opens its mutex
        cout << "got to lock" << endl;
        pthread_cond_wait( &wake_pusher, &insertion_mutex );
        cout << "exited lock" << endl;
        //loops through all elements before waiting again
        while(toPush.size() != 0){
            try{
                //create and open file
                ifstream fp;
                string name = toPush.front();
                name.append(".bmp");
                fp.open(name, ifstream::binary);
                if(fp.is_open()){
                        //reads file
                        // checks for length
                        fp.seekg (0, fp.end);
                        int length = fp.tellg();
                        fp.seekg (0, fp.beg);
                        //copys to buffer
                        char * buffer = new char[length];
                        fp.read ((char*) &buffer[0] ,length);
                        //add delim
                        string message = "bram>imageeditS>f>";
                        message.append(name);
                        message.append(">");
                        //adds to string
                        for(int i = 0; i < length; i++){
                            message.push_back(buffer[i]);
                        }
                        //sends message
                        pusher->send(message.c_str(), message.length());
                        //cleanup
                        delete [] buffer;
                        fp.close();
                        cout << name << endl;
                        //remove(name.c_str());
                        toPush.erase(toPush.begin());
                }
                else{
                    cout << "error making files " << name << endl;
                }
            }
            catch(zmq::error_t& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
}

void* Color_extractor_service::main(void* arg)
{
    zmq::message_t* payload = static_cast<zmq::message_t*>(arg);
    string filename;
    string pay_string = string((char*) payload->data(), payload->size());
    cout << "received something" << endl;
    cout << pay_string[16] << endl;
    if(pay_string[16] == 'r'){
        string MD5string = pay_string;
        MD5string.erase(0, 18);
        string clean_name = MD5string;
        clean_name.erase(0,33);
        clean_name.erase(clean_name.size()-4, 5);
        cout << clean_name << endl;
        cout << MD5string << endl;
        //check com value
        int inDB = check_DB(MD5string);
        //create socket
        zmq::context_t context(1);
        zmq::socket_t* com_push;
        com_push = new zmq::socket_t( context, ZMQ_PUSH);
        com_push->connect("tcp://benternet.pxl-ea-ict.be:24041");
        if(inDB){
            string to_send = "bram>imageeditS>r>";
            to_send.append(MD5string);
            to_send.append(">upload>nopls");
            cout << to_send << endl;
            com_push->send(to_send.c_str(), to_send.size());
            //sends files
            vector<string> subfilenames (3, clean_name);
            subfilenames[0].append("_red");
            subfilenames[1].append("_green");
            subfilenames[2].append("_blue");
            //lock mutex
            pthread_mutex_lock(&pusher_mutex);
            pthread_mutex_lock(&insertion_mutex);
            //inserts filenames to send
            toPush = subfilenames;
            //wakes the pusher
            pthread_cond_signal(&wake_pusher);
            pthread_mutex_unlock(&insertion_mutex);
            pthread_mutex_unlock(&pusher_mutex);
            vector<string>* test = &toPush;
            cout << "completely ended" << endl;
            return 0;
        }
        else{
            string to_send = "bram>imageeditS>r>";
            to_send.append(MD5string);
            to_send.append(">upload>yespls");
            cout << to_send << endl;
            com_push->send(to_send.c_str(), to_send.size());
            return 0;
        }
    }
    //start flow
    write_file(string((char*) payload->data(), payload->size()), &filename, payload->size());
    string md5hash = filename;
    create_md5(&md5hash);
    md5hash.append(">");
    md5hash.append(filename);
    cout << md5hash << endl;
    database.push_back(md5hash);
    split_file(filename);
    string raw_name = filename.substr(0, filename.size() - 4);
    vector<string> subfilenames (3, string(raw_name));
    subfilenames[0].append("_red");
    subfilenames[1].append("_green");
    subfilenames[2].append("_blue");
    //lock mutex
    pthread_mutex_lock(&pusher_mutex);
    pthread_mutex_lock(&insertion_mutex);
    //inserts filenames to send
    toPush = subfilenames;
    //wakes the pusher
    pthread_cond_signal(&wake_pusher);
    pthread_mutex_unlock(&insertion_mutex);
    pthread_mutex_unlock(&pusher_mutex);
    vector<string>* test = &toPush;
    cout << "completely ended" << endl;
    delete payload;
}

int Color_extractor_service::write_file(string limited, string* filename, int size)
{
        //cout << limited << endl;
        limited.erase(0, topicName.size() + 1);
        //paste data to file
        ofstream filerReceiveptr;
        //get filename
        (*filename) = limited.substr(0, limited.find_first_of('>'));
        cout << *filename << endl;
        filerReceiveptr.open((*filename), ofstream::binary);
        if(filerReceiveptr.is_open()){
            //starts loop on delimiter and writes file
            for(int i = limited.find_first_of('>') + 1; i < size - 16; i++){
                filerReceiveptr.put(limited[i]);
            }
            cout << "writtend" << endl;
            //cleanup
            filerReceiveptr.close();
            return 0;
        }
        else{
            cout << "failed to create file" << endl;
            return 1;
        }
}

int Color_extractor_service::split_file(string filename)
{
    cout << "splittig files " << filename << endl;
    //extract colors
    //prepare command line
    QProcess process_handler;
    QString program ("programma.exe");
    QStringList arguments;
    arguments << "-i" << filename.c_str();
    //start and wait for exit
    process_handler.start(program, arguments);
    process_handler.waitForFinished(600000);
    //remove(filename.c_str());
}

int Color_extractor_service::create_md5(string* filename)
{
    string md5;
    //extract colors
    //prepare command line
    QProcess process_handler;
    QString program ("md5sum.exe");
    QStringList arguments;
    arguments << "-i" << filename->c_str();
    //start and wait for exit
    process_handler.start(program, arguments);
    process_handler.waitForFinished(600000);
    md5 = process_handler.readAllStandardOutput();
    md5.erase(32, md5.length());
    cout << md5 << endl;
    (*filename) = md5;
}

int Color_extractor_service::check_DB(string string_name)
{
    for(std::vector<string>::iterator it = database.begin(); it != database.end(); it++){
        if (*it == string_name){
            cout << "inDB" << endl;
            return 1;
        }
    }
    cout << "not inDB" << endl;
    return 0;
}

