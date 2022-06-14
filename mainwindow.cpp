#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <QProcess>
#include <QDebug>

#include "configsetter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();
    for (QString path : m_openUrls){
        ui->urlList->addItem(path);
    }

    m_timer= new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::slotOpenUrls);

    saveSettings();
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::readSettings()
{
    QString strLocalPath;
    int localIndex = 0;
    do {
        strLocalPath = ConfigSetter::instance()->value(("openUrls/url" + QString::number(localIndex++))).toString();
        if (nullptr != strLocalPath) {
            if (!m_openUrls.contains(strLocalPath)) {
                m_openUrls.push_back(strLocalPath);
            }
        }
    } while (nullptr != strLocalPath);
     int timeMsec =ConfigSetter::instance()->value("Time").toInt();
     m_timeMSec = timeMsec >0 ?timeMsec : m_timeMSec;
     ui->timeEdit->setText(QString::number(m_timeMSec/1000));
}

void MainWindow::saveSettings()
{
    int index =0;
    for(QString path : m_openUrls){
        ConfigSetter::instance()->setValue(("openUrls/url" + QString::number(index++)),path);
    }
}

void MainWindow::on_addBtn_clicked()
{
    m_openUrls << ui->addEdit->text();
    ConfigSetter::instance()->setValue(("openUrls/url" + QString::number(m_openUrls.count()-1)),ui->addEdit->text());
    ui->urlList->addItem(ui->addEdit->text());
}

void MainWindow::slotOpenUrls()
{
    QThread *th = QThread::create([ = ]() {
        for (QString path : m_openUrls){
            if(!isStop){
                QThread::sleep(2);
                QDesktopServices::openUrl(QUrl(path));
            }
        }
        if(!isStop){
            unsigned long s =m_timeMSec/1000 -m_openUrls.count() *2 - 5;
            if(s > 0){
               QThread::sleep(s);
            }

        }
        QProcess::execute("killall chrome");
    });

    connect(th, &QThread::finished, th, &QObject::deleteLater);
    th->start();
}

void MainWindow::on_deleteBtn_clicked()
{
    if(ui->urlList->currentRow()>0){
        m_openUrls.removeOne(ui->urlList->currentItem()->text());
        ui->urlList->takeItem(ui->urlList->currentRow());
    }
    ConfigSetter::instance()->clear();
    saveSettings();
}

void MainWindow::on_clearBtn_clicked()
{
    ui->urlList->clear();
    m_openUrls.clear();
    ConfigSetter::instance()->clear();

}

void MainWindow::on_startBtn_clicked()
{
    slotOpenUrls();
    isStop = false;
    m_timer->start(m_timeMSec);
}

void MainWindow::on_stopBtn_clicked()
{
    isStop = true;
    m_timer->stop();
}

void MainWindow::on_setTime_clicked()
{
    int s = ui->timeEdit->text().toInt();
    m_timeMSec = s*1000;
    ConfigSetter::instance()->setValue("Time",m_timeMSec);
}

