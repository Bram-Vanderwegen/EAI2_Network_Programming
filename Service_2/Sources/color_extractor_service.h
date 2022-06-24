#ifndef COLOR_EXTRACTOR_SERVICE_H
#define COLOR_EXTRACTOR_SERVICE_H

#include <iostream>
#include "zmq.hpp"
#include "vector"

using namespace std;


class Color_extractor_service
{
public:
    Color_extractor_service();
    ~Color_extractor_service();
    //constant topic name
    static string topicName;
    //vector container
    static vector<string> toPush;
    static vector<string> database;
    //mutex and condition
    static pthread_mutex_t pusher_mutex;
    static pthread_mutex_t insertion_mutex;
    static pthread_cond_t wake_pusher;

private:

    //main function call sequence
    static void* main(void* arg);

    //file edit functions
    static int write_file(string limited, string* filename, int size);
    static int split_file(string filename);
    static int create_md5(string* filename);

    //helper functions
    static void make_filenames();
    static int check_DB(string string_name);

    //network funtions
    static void* receiveloop(void* arg);
    static void* send_loop(void* arg);

};

#endif // COLOR_EXTRACTOR_SERVICE_H
