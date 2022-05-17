#include "color_extractor_service.h"
#include "zmq.hpp"
#include <fstream>
#include <qprocess.h>



Color_extractor_service::Color_extractor_service(string payload_in, int payload_size_in, string topicName_in)
    : payload(payload_in),
      payload_size(payload_size_in),
      topicName(topicName_in)
{
    //create socket
    zmq::context_t context(1);
    pusher = new zmq::socket_t( context, ZMQ_PUSH);
    pusher->connect("tcp://benternet.pxl-ea-ict.be:24041");

    //start flow
    write_file();
    split_file();
    make_filenames();
    for(int i = 0; i < 3; i++){
        send_file(i);
    }
}

Color_extractor_service::~Color_extractor_service()
{
    delete subfilenames;
    delete pusher;
}


int Color_extractor_service::write_file()
{
        string limited = payload;
        limited.erase(0, topicName.size() + 1);

        //paste data to file
        ofstream filerReceiveptr;
        //get filename
        filename = limited.substr(0, limited.find_first_of('>'));
        cout << filename << endl;
        filerReceiveptr.open(filename, ofstream::binary);
        if(filerReceiveptr.is_open()){
            //starts loop on delimiter and writes file
            for(int i = limited.find_first_of('>') + 1; i < payload_size; i++){
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

int Color_extractor_service::send_file(int nr)
{
    try{
        //create and open file
        ifstream fp;
        string name = (*subfilenames)[nr];
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
                string message = "bram>imageeditS>";
                message.append((*subfilenames)[nr] + ".bmp>");
                //adds to string
                for(int i = 0; i < length; i++){
                    message.push_back(buffer[i]);
                }
                //sends message
                pusher->send(message.c_str(), message.length());
                //cleanup
                delete [] buffer;
                fp.close();
                return 0;
        }
        else{
            cout << "error making files" << endl;
            return 1;
        }
    }
    catch(zmq::error_t& e) {
        std::cout << e.what() << std::endl;
        return 2;
    }
}

int Color_extractor_service::split_file()
{
    //extract colors
    //prepare command line
    QProcess process_handler;
    QString program ("programma.exe");
    QStringList arguments;
    arguments << "-i" << filename.c_str();
    //start and wait for exit
    process_handler.start(program, arguments);
    process_handler.waitForFinished(600000);
}

void Color_extractor_service::make_filenames()
{
    //prepares filenames for sending
    string raw_name = filename.substr(0, filename.size() - 4);
    subfilenames = new vector<string> (3, raw_name);
    (*subfilenames)[0].append("_red");
    (*subfilenames)[1].append("_green");
    (*subfilenames)[2].append("_blue");
}
