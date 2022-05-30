#ifndef PING_H
#define PING_H

#include <pthread.h>
#include <zmq.hpp>
#include <iostream>

using namespace std;

class ping
{
public:
    ping();
private:
    static void* pingloop(void* arg);
};

#endif // PING_H
