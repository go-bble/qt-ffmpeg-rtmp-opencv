#include "TController.h"
#include "TVideoCapture.h"
#include "TAudioRecord.h"
#include "TMediaEncode.h"
#include "TRtmp.h"
#include <iostream>
#include "TFilter.h"




void TController::run()
{
    long long beginPts = getCurTime();
    while (!isExit)
    {
        TData ad  = TAudioRecord::Get()->Pop();
        TData vd  = TVideoCapture::Get()->Pop();
        if(ad.size <= 0 && vd.size <=0)
        {
            msleep(1);
            continue;
        }
        if(ad.size > 0)
        {
            ad.pts = ad.pts - beginPts;

            TData pcm = TMediaEncode::Get()->Resample(ad);
            ad.Drop();
            TData pkt = TMediaEncode::Get()->encodeAudio(pcm);
            if(pkt.size > 0)
            {
                if(TRtmp::Get()->sendFrame(pkt,aIndex))
                {
                    //std::cout << " -A ";
                }
            }
        }

        if(vd.size > 0)
        {
            vd.pts = vd.pts - beginPts;

            TData yuv = TMediaEncode::Get()->RGBtoYUV(vd);
            vd.Drop();
            TData pkt = TMediaEncode::Get()->encodeVideo(yuv);
            if(pkt.size > 0)
            {
                if(TRtmp::Get()->sendFrame(pkt,vIndex))
                {
                    std::cout << " -V ";
                }
            }

        }


    }
}
TController *TController::Get()
{
    static TController tc;
    return &tc;
}

bool TController::SetFilter(std::string key, double val)
{
    TFilter::Get()->Set(key,val);
    return  true;
}

//bool TController::FchangeVal(std::string key, double val)
//{
//    TFilter::Get()->ChangeVal(key,val);
//}


bool TController::Start()
{

    //-------------------------1.过滤器------------------------------


    TVideoCapture::Get()->addFilter(TFilter::Get(TBILATERAL));
    std::cout << " 1.addFilter()  " << std::endl;

    //-------------------------2.视频采集初始化------------------------------

    if(camIndex >= 0)
    {
        if(!TVideoCapture::Get()->Init(camIndex))
        {
            std::cout << " 2.err TVideoCapture::Get()->Init(camIndex) " << std::endl;
            return false;
        }
    }
    else if (!inUrl.empty())
    {
        if(!TVideoCapture::Get()->Init(inUrl.c_str()))
        {
         std::cout << " 2.err TVideoCapture::Get()->Init(inUrl.c_str()) " << std::endl;
         return false;
        }
    }
    else
    {
        std::cout << " 2.err  TVideoCapture  " << std::endl;
        return false;
    }
    std::cout << " 2.TVideoCapture init;  " << std::endl;


    //-------------------------3.音频采集初始化，启动音视频采集线程------------------------------

    if(!TAudioRecord::Get()->Init())
    {
        std::cout << " 3.err  TAudioRecord Init  " << std::endl;
        return false;
    }

    std::cout << "  3. TAudioRecord Init " << std::endl;

    TAudioRecord::Get()->Start();
    TVideoCapture::Get()->Start();

    //-------------------------4.格式转换初始化------------------------------

    TMediaEncode::Get()->inWidth = TVideoCapture::Get()->width;
    TMediaEncode::Get()->outWidth = TVideoCapture::Get()->width;
    TMediaEncode::Get()->inHeight = TVideoCapture::Get()->height;
    TMediaEncode::Get()->outHeight = TVideoCapture::Get()->height;

    if(!TMediaEncode::Get()->InitScale())
    {
        std::cout << " 4.err  InitScale()" << std::endl;
        return false;
    }
     std::cout << " 4. InitScale()" << std::endl;


     //-------------------------5.音频重采样------------------------------

    TMediaEncode::Get()->inChannels = TAudioRecord::Get()->channels;
    TMediaEncode::Get()->outNbSamples = TAudioRecord::Get()->nbSamples;
    TMediaEncode::Get()->inSampleRate = TAudioRecord::Get()->sampleRate;
    if(!TMediaEncode::Get()->InitResample())
    {
        std::cout << " 5.err  InitResample()" << std::endl;
        return false;
    }
    std::cout << " 5.InitResample()" << std::endl;


     //-------------------------6.初始化音频编码器------------------------------

     if(!TMediaEncode::Get()->initAudioCodec())
     {
         std::cout << " 6.err  initAudioCodec()" << std::endl;
         return false;
     }
     std::cout << " 6.initAudioCodec()" << std::endl;


     //-------------------------7.初始化视频编码器------------------------------


     if(!TMediaEncode::Get()->initVideoCodec())
     {
         std::cout << " 7.err  initVideoCodec()" << std::endl;
         return false;
     }
     std::cout << " 7.initVideoCodec()" << std::endl;


     //-------------------------8.初始化封装器------------------------------

     if(!TRtmp::Get()->Init(outUrl.c_str()))
     {
         std::cout << " 8.err  TRtmp init()" << std::endl;
         return false;
     }
     std::cout << " 8.TRtmp init()" << std::endl;


     //-------------------------9.添加音视频流------------------------------
    vIndex = TRtmp::Get()->addStream(TMediaEncode::Get()->vc);
    aIndex = TRtmp::Get()->addStream(TMediaEncode::Get()->ac);
    if(vIndex<0)
    {
        std::cout << " 9.err  add video Stream" << std::endl;
        return false;
    }
    if(aIndex<0)
    {
        std::cout << " 9.err  add audio Stream" << std::endl;
        return false;
    }
    std::cout << " 9.  add audio&video Stream" << std::endl;

    //-------------------------10.发送封装头------------------------------

    if(!TRtmp::Get()->sendHead())
    {
        std::cout << " 10.err TRtmp sendHead" << std::endl;
        return false;
    }

    std::cout << " 10.TRtmp sendHead" << std::endl;


    //-------------------------11.------------------------------
    TAudioRecord::Get()->Clear();
    TVideoCapture::Get()->Clear();
    TDataThread::Start();


    return  true;
}

void TController::Stop()
{
    TDataThread::Stop();
    TAudioRecord::Get()->Stop();
    TVideoCapture::Get()->Stop();
    TMediaEncode::Get()->Close();
    TRtmp::Get()->Close();
    camIndex = -1;
    inUrl = "";
}

TController::TController()
{

}
TController::~TController()
{

}
