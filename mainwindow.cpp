#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer),
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

    connect(httpManager, SIGNAL(ImageReady(QPixmap *)),
            this, SLOT(processImage(QPixmap *)));
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
}
