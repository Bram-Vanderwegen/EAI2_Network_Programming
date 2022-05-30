#include "weather_api.h"

weather_api::weather_api()
{
    }

void weather_api::weatherupdater(QNetworkReply* reply)
{
    std::cout << (string) reply->readAll() << endl;
}
