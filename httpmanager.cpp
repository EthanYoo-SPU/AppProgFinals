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

void HTTPManager::sendImageRequest(QString zip)
{
    QNetworkRequest request;
    QString address = "https://dev.virtualearth.net/REST/V1/Imagery/Map/AerialWithLabels/"
            + zip
            + "/8?mapSize=441,311&mapLayer=TrafficFlow&format=png&key=An0rf5XTxxopj6OG216Jv_Na_M6c4R_vneksNtPD0VCvh9N2ZMAziYU404jpW0YF";
    request.setUrl(QUrl(address));
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
