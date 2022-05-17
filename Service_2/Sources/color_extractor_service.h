#ifndef COLOR_EXTRACTOR_SERVICE_H
#define COLOR_EXTRACTOR_SERVICE_H

#include <iostream>
#include "zmq.hpp"
#include "vector"

using namespace std;

class Color_extractor_service
{
public:
    Color_extractor_service(string payload_in, int payload_size_in, string topicName_in);
    ~Color_extractor_service();

private:

    int write_file();
    int send_file(int nr);
    int split_file();
    void make_filenames();

    string payload;
    int payload_size;
    string topicName;

    string filename;
    vector<string>* subfilenames;
    zmq::socket_t* pusher;
};

#endif // COLOR_EXTRACTOR_SERVICE_H
