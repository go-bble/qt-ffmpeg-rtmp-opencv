#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
extern "C"{
#include "libavformat/avformat.h"
}
#include <QComboBox>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TVideoCapture.h"
#include "TRtmp.h"
#include "TDataThread.h"
#include "TData.h"
#include "TMediaEncode.h"


#include "TController.h"
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //-------------------------T------------------------------


    TController::Get()->camIndex = 0;
//    TController::Get()->outUrl = "rtmp://127.0.0.1/live/home";ffmpeg -re -i v1080.mp4 -vcodec libx264 -acodec aac -f flv rtmp://192.168.31.93:1935/live/home
    TController::Get()->outUrl = "rtmp://192.168.31.93:1935/live/home";

    TController::Get()->SetFilter("d", (ui->comboBox_2->currentIndex()+1)*3);//369
    TController::Get()->Start();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
//    TController::Get()->FchangeVal("bbb", (ui->comboBox_2->currentIndex()+1)*3);

    TController::Get()->Stop();
    TController::Get()->camIndex = 1;
   // TController::Get()->outUrl = "rtmp://127.0.0.1/live/home";
    TController::Get()->SetFilter("d", (ui->comboBox_2->currentIndex()+1)*3);//369
//    TController::Get()->Start();

}
