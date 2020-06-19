#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer),
    galleryTimer(new QTimer),
    httpManager(new HTTPManager)
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(setCurrentTime()));
    connect(timer, SIGNAL(timeout()),
            this, SLOT(setWorldTime()));
    
    setCurrentTime();
    setWorldTime();
    timer->start(1000);
    changeGreetingLabel();

    connect(galleryTimer, SIGNAL(timeout()),
            this, SLOT(loadGalleryPic()));
    loadGalleryPic();
    galleryTimer->start(10000);
    
    connect(httpManager, SIGNAL(ImageReady(QPixmap *)),
            this, SLOT(processImage(QPixmap *)));
    connect(httpManager, SIGNAL(WeatherJsonReady(QJsonObject *)),
            this, SLOT(processWeatherJson(QJsonObject *)));

//    QString cloudImageName = ":/images/cloud.jpg";
//    if(imageCloud.load(cloudImageName)) {
//        imageCloud = imageCloud.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
//    }
//    ui->weatherImage->setPixmap(imageCloud);

//    int remainder = galleryLocation % 5;
//    QString galleryImageName = ":/images/gallery" + QString::number(remainder) + ".jpg";
//    qDebug() << galleryImageName;




    //Automatically sets zip to 98119/SPU
    httpManager->sendImageRequest("98119");
    httpManager->sendWeatherRequest("98119");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCurrentTime()
{
    QTime time = QTime::currentTime();
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");

    ui->hourLCD->display(hour);
    ui->minuteLCD->display(minute);
    ui->secondLCD->display(second);
}

void MainWindow::setWorldTime() {
    QTime localTime = QTime::currentTime();
    QTime time = localTime;
    time = time.addSecs(timeZoneDifference);
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");

    ui->hourLCD_2->display(hour);
    ui->minuteLCD_2->display(minute);
    ui->secondLCD_2->display(second);
}

void MainWindow::loadGalleryPic()
{
    int remainder = galleryLocation % 5;
    QString galleryImageName = ":/images/gallery" + QString::number(remainder) + ".jpg";
    if(gallery.load(galleryImageName)) {
        gallery = gallery.scaled(ui->galleryLabel->size(), Qt::KeepAspectRatio);
    }
    ui->galleryLabel->setPixmap(gallery);

    galleryLocation += 1;

}

void MainWindow::changeGreetingLabel()
{
    if (ui->hourLCD->intValue() >= 6 && ui->hourLCD->intValue() < 12) {
        ui->greetingLabel->setText("Good Morning!");
    }
    if (ui->hourLCD->intValue() >= 12 && ui->hourLCD->intValue() < 17) {
        ui->greetingLabel->setText("Good Afternoon!");
    }
    if (ui->hourLCD->intValue() >= 17 && ui->hourLCD->intValue() <= 23) {
        ui->greetingLabel->setText("Good Evening!");
    }
    if (ui->hourLCD->intValue() >= 0 && ui->hourLCD->intValue() < 6) {
        ui->greetingLabel->setText("Go to bed!");
    }
}


void MainWindow::on_timeZoneBox_activated(int index)
{
    if (index == 0) {
        return;
    }
    if (index == 1) {
        timeZoneDifference = 0;
        setWorldTime();
    }
    if (index == 2) {
        timeZoneDifference = 10800;
        setWorldTime();
    }
    if (index == 3) {
        timeZoneDifference = -28800;
        setWorldTime();
    }


}

void MainWindow::processImage(QPixmap *image)
{
    ui->imageLabel->setPixmap(*image);
}

void MainWindow::on_imageDownloadButton_clicked()
{
    QString zip = ui->zipCodeEdit->text();
    qDebug() << zip;
    httpManager->sendImageRequest(zip);
    //qDebug() << "Clicked weather button";
    //QString zip = ui->zipCodeEdit->text();
    qDebug() << zip;
    httpManager->sendWeatherRequest(zip);
}

void MainWindow::processWeatherJson(QJsonObject *json)
{
    qDebug() << "Json ready";
    QString weather = json->value("weather").toArray()[0].toObject()["main"].toString();
    QString weatherDesc = json->value("weather").toArray()[0].toObject()["description"].toString();
    double temp = json->value("main").toObject()["temp"].toDouble();
    double tempMin = json->value("main").toObject()["temp_min"].toDouble();
    double tempMax = json->value("main").toObject()["temp_max"].toDouble();

    qDebug() << weather;
    qDebug() << weatherDesc;
    qDebug() << temp;
    qDebug() << tempMin;
    qDebug() << tempMax;

    QString weatherDescAlt = weatherDesc.toUpper();



    ui->minTempLabel->setNum(tempMin);
    ui->currentTempLabel->setNum(temp);
    ui->maxTempLabel->setNum(tempMax);
    ui->weatherDescLabel->setText(weatherDescAlt);

    //ui->weatherLabel->setText("Current weather: " + weather + ", temp: " + QString::number(temp));

    QString cloudImageName = ":/images/cloud.jpg";
    QString clearImageName = ":/images/clear.jpg";
    QString sunImageName = ":/images/sun.jpg";
    QString rainImageName = ":/images/rain.jpg";
    QString thunderImageName = ":/images/thunderstorm.jpg";
    QString snowImageName = ":/images/snow.png";


    if(imageCloud.load(cloudImageName)) {
        imageCloud = imageCloud.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(imageRain.load(rainImageName)) {
        imageRain = imageRain.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(imageSun.load(sunImageName)) {
        imageSun = imageSun.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(imageClear.load(clearImageName)) {
        imageClear = imageClear.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(imageThunder.load(thunderImageName)) {
        imageThunder = imageThunder.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(imageSnow.load(snowImageName)) {
        imageSnow = imageSnow.scaled(ui->weatherImage->size(), Qt::KeepAspectRatioByExpanding);
    }

    if(weather == "Clouds") {
        ui->weatherImage->setPixmap(imageCloud);
    }
    if(weather == "Rain" || weather == "Drizzle") {
        ui->weatherImage->setPixmap(imageRain);
    }
    if(weather == "Clear") {
        ui->weatherImage->setPixmap(imageClear);
    }
    if(weather == "Snow") {
        ui->weatherImage->setPixmap(imageSnow);
    }
    if(weather == "Thunderstorm") {
        ui->weatherImage->setPixmap(imageThunder);
    }


    /*
     *{
     * "coord":{"lon":-122.36,"lat":47.64},
     * "weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],
     * "base":"stations",
     * "main":{"temp":59.7,"feels_like":57.4,"temp_min":57.99,"temp_max":62.01,"pressure":1013,"humidity":58},
     * "wind":{"speed":1.99,"deg":210,"gust":5.99},
     * "clouds":{"all":97},
     * "dt":1592192544,
     * "sys":{"type":3,"id":19628,"country":"US","sunrise":1592136662,"sunset":1592194127},
     * "timezone":-25200,"id":0,"name":"Seattle","cod":200}
     */
}

void MainWindow::on_weatherDownloadButton_clicked()
{
    qDebug() << "Clicked weather button";
    QString zip = ui->zipCodeEdit->text();
    qDebug() << zip;
    httpManager->sendWeatherRequest(zip);
}

void MainWindow::on_imageDownloadButton_2_clicked()
{

}
