#include <iostream>
#include <pthread.h>
#include <vector>
#include <zmq.hpp>
#include <fstream>
#include <QProcess>

#include <color_extractor_service.h>
#include <ping.h>

using namespace std;

//color extractor constansts
string Color_extractor_service::topicName = string("bram>imageeditC");
pthread_mutex_t Color_extractor_service::pusher_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Color_extractor_service::insertion_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Color_extractor_service::wake_pusher = PTHREAD_COND_INITIALIZER;
vector<string> Color_extractor_service::toPush = vector<string>();
vector<string> Color_extractor_service::database = vector<string>();

void* color_extractor_function(void *arg);

int main()
{
    //initialize service classes
    Color_extractor_service service_1;
    ping service_2;

    while(1);

    cout << "Hello krokodil!" << endl;
    return 0;
}








