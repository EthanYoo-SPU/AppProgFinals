#include "httpmanager.h"

HTTPManager::HTTPManager(QObject *parent) :
    QObject(parent),
    imageDownloadManager(new QNetworkAccessManager)
{
    connect(imageDownloadManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(ImageDownloadedHandler(QNetworkReply*)));
}

HTTPManager::~HTTPManager() {
    delete imageDownloadManager;
}

void HTTPManager::sendImageRequest()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://i.imgur.com/z98fwzQ.jpg"));
    imageDownloadManager->get(request);
    qDebug() << "Image request sent to address" << request.url();
}

void HTTPManager::ImageDownloadedHandler(QNetworkReply *reply)
{
    qDebug() << "Image reply has arrived";
    if(reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    qDebug() << "Download was successful";
    QPixmap *image = new QPixmap;
    image->loadFromData(reply->readAll());
    emit ImageReady(image);
    qDebug() << "Image emitted";
}
