#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <QNetworkAccessManager>
#include <qobject.h>
#include <iostream>
#include <qnetworkreply.h>
#include <QApplication>

using namespace std;

class weather_api : public QObject
{
public:
    weather_api();
private:

    QNetworkAccessManager* internetCom;
    QNetworkReply* reqReply;

public slots:
    void weatherupdater(QNetworkReply* reply);

};

#endif // WEATHER_API_H
