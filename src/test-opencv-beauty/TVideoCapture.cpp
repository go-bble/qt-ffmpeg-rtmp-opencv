#include "TVideoCapture.h"
#include "opencv2/highgui.hpp"
#include <QDebug>
using namespace cv;
class FVideoCapture :public TVideoCapture
{
public:
    VideoCapture cam;

    void run()
    {

        namedWindow("v");
        Mat frame;
        int filterSize = static_cast<int>(filters.size());
        while (!isExit)
        {
            if(!cam.read(frame))
            {
                msleep(1);
                continue;
            }
            if(frame.empty())
            {
                msleep(1);
                continue;
            }
            imshow("v", frame);
            waitKey(1);
//            Tmutex.lock();
//            for(int i = 0; i < filterSize;i++)
//            {
//                Mat des;
//                filters[i]->Filter(&frame, &des);
//                frame = des;
//            }
//            Tmutex.unlock();

            TData d((char*)frame.data, frame.cols*frame.rows*frame.elemSize(),getCurTime());
            Push(d);


        }
    }

    bool Init(int camIndex = 0)
    {
        cam.open(camIndex);
        if(!cam.isOpened())
        {
            qDebug()<<"!!!!!cam.isNOTOpened()";
            return false;
        }
        qDebug()<<"cam.isOpened()";
        width = cam.get(CAP_PROP_FRAME_WIDTH);
        height = cam.get(CAP_PROP_FRAME_HEIGHT);
        fps = cam.get(CAP_PROP_FPS);
        qDebug()<<"cam.fps--->"<<fps;
        qDebug()<<"cam.width--->"<<width;
        qDebug()<<"cam.height--->"<<height;

        if (fps == 0) fps = 30;
        return true;
    }
    bool Init(const char *url)
    {
        cam.open(url);
        if (!cam.isOpened())
        {
            qDebug()<<"!!!!!cam.isNOTOpened()";
            return false;
        }
        qDebug()<<"cam.isOpened()";
        width = cam.get(CAP_PROP_FRAME_WIDTH);
        height = cam.get(CAP_PROP_FRAME_HEIGHT);
        fps = cam.get(CAP_PROP_FPS);
        if (fps == 0) fps = 25;
        return true;
    }
    void Stop()
    {
        TDataThread::Stop();
        if (cam.isOpened())
        {
            cam.release();
        }
    }
};

TVideoCapture *TVideoCapture::Get(unsigned char index)
{
    static FVideoCapture xc[255];
    return &xc[index];
}

TVideoCapture::~TVideoCapture()
{

}

TVideoCapture::TVideoCapture()
{

}
