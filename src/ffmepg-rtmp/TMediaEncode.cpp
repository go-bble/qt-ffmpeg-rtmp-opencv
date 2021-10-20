#include "TMediaEncode.h"


#if defined WIN32 || defined _WIN32
#include <windows.h>
#include <iostream>
using namespace std;
#endif
extern "C"
{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}


static int GetCpuNum()
{
#if defined WIN32 || defined _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    return (int)sysinfo.dwNumberOfProcessors;
#elif defined __linux__
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined __APPLE__
    int numCPU = 0;
    int mib[4];
    size_t len = sizeof(numCPU);

    // set the mib for hw.ncpu
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

                           // get the number of CPUs from the system
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    if (numCPU < 1)
    {
        mib[1] = HW_NCPU;
        sysctl(mib, 2, &numCPU, &len, NULL, 0);

        if (numCPU < 1)
            numCPU = 1;
    }
    return (int)numCPU;
#else
    return 1;
#endif
}
class CTMediaEncode:public TMediaEncode
{
public:
    /**
     * @brief InitScale 格式转换、输出结构frame的初始化
     * @return
     */
    bool InitScale()
    {
        vsc = sws_getCachedContext(vsc,
                                   inWidth,inHeight,AV_PIX_FMT_BGR24,
                                   outWidth,outHeight,AV_PIX_FMT_YUV420P,
                                   SWS_BICUBIC,
                                   0,0,0);
        if(!vsc)
        {
            cout<<"SwsContext failed!";
            return false;
        }

        yuv = av_frame_alloc();
        yuv->format = AV_PIX_FMT_YUV420P;
        yuv->width = inWidth;
        yuv->height = inHeight;
        yuv->pts = 0;

        int ret = av_frame_get_buffer(yuv,32);
        if(ret != 0)
        {
            char buf[1024] = { 0 };
            av_strerror(ret, buf, sizeof(buf) - 1);
            cout<<buf;
            return false;
        }
        return true;

    }
    TData RGBtoYUV(TData rgb)
    {
        TData t;
        t.pts = rgb.pts;

        uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };
        int inLineSize[AV_NUM_DATA_POINTERS] = { 0 };
        indata[0] = (uint8_t*)rgb.data;
        inLineSize [0] = inWidth * inPixSize;

        int h = sws_scale(vsc, indata, inLineSize, 0, inHeight, yuv->data, yuv->linesize);
        if(h <= 0)
        {
            return t;
        }
        yuv->pts = rgb.pts;
        t.data = (char*)yuv;
        int *lis = yuv->linesize;
        while ((*lis))
        {
            t.size += (*lis)*outHeight;
            lis++;
        }
        return t;



    }
    bool initVideoCodec()
    {
        if(!(vc = CreateCodec(AV_CODEC_ID_H264)))
        {
            return false;
        }
        vc->bit_rate = outBitrate;
        vc->width    = outWidth;
        vc ->height  = outHeight;
        vc->framerate = { outfps,1 };
        vc->gop_size = outGopSize;
        vc->max_b_frames = outMaxBFrames;
        vc->pix_fmt = AV_PIX_FMT_YUV420P;

        return openCodec(&vc);
    }
    TData encodeVideo(TData frame)
    {
        av_packet_unref(&vPacket);
        TData t;
        if(frame.size<=0 || !frame.data)return t;

        AVFrame *p = (AVFrame *)frame.data;
        int ret = avcodec_send_frame(vc,p);
        if(ret != 0)return t;

        ret = avcodec_receive_packet(vc ,&vPacket);
        if(ret != 0||vPacket.size <=0)return t;

        t.data =(char *)&vPacket;
        t.size = vPacket.size;
        t.pts = frame.pts;
        return t;
    }



    bool InitResample()
    {
        asc = nullptr;
        asc = swr_alloc_set_opts(asc,
                                 av_get_default_channel_layout(inChannels), (AVSampleFormat)outSampleFmt, inSampleRate,//输出格式
                                 av_get_default_channel_layout(inChannels), (AVSampleFormat)inSampleFmt, inSampleRate, 0, 0);//输入格式

        if(!asc)
        {
            cout << "swr_alloc_set_opts() failed!";
            return false;
        }
        int ret = swr_init(asc);
        if (ret != 0)
        {
            char err[1024] = { 0 };
            av_strerror(ret, err, sizeof(err) - 1);
            cout << err << endl;
            return false;
        }
        cout << "swr_init() OK!" << endl;

        pcm = av_frame_alloc();

        pcm->format = outSampleFmt;
        pcm->channel_layout = av_get_default_channel_layout(inChannels);
        pcm->channels = inChannels;
        pcm->nb_samples = outNbSamples;

        ret = av_frame_get_buffer(pcm, 0); // 给pcm分配存储空间
        if (ret != 0)
        {
            char err[1024] = { 0 };
            av_strerror(ret, err, sizeof(err) - 1);
            cout << err << endl;
            return false;
        }
        return true;

    }
    TData Resample(TData d)
    {
        TData r;
        const uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
        indata[0] = (uint8_t *)d.data;
        int len = swr_convert(asc, pcm->data, pcm->nb_samples,indata, pcm->nb_samples);
        if(len <= 0)
        {
            return r;
        }
        pcm->pts = d.pts;
        r.data = (char *)pcm;
        r.size = pcm->nb_samples * pcm->channels *2;
        r.pts = d.pts;
        return r;

    }
    bool initAudioCodec()
    {
        ac = CreateCodec(AV_CODEC_ID_AAC);
        if(!ac)return false ;
        ac->bit_rate = outAudioBitrate;
        ac->sample_rate = inSampleRate;
        ac->sample_fmt = AV_SAMPLE_FMT_FLTP;
        ac->channels = inChannels;
        ac->channel_layout = av_get_default_channel_layout(inChannels);
        return openCodec(&ac);

    }
    long long lasta = -1;//(bo ??)
    TData encodeAudio(TData frame)
    {
        TData r;
        if(frame.size <= 0 || !frame.data)return r;
        AVFrame *p = (AVFrame *)frame.data;
        if(lasta == p->pts)
        {
            p->pts += 1000;
        }
        lasta = p->pts;
        int ret = avcodec_send_frame(ac, p);

        if (ret != 0)
            return r;
        av_packet_unref(&aPacket);
        ret = avcodec_receive_packet(ac, &aPacket);
        if (ret != 0)
            return r;
        r.data = (char*)&aPacket;
        r.size = aPacket.size;
        r.pts = frame.pts;
        return r;
    }



    void Close()
    {
        if(vsc)
        {
            sws_freeContext(vsc);
            vsc = nullptr;
        }
        if(yuv)
        {
            av_frame_free(&yuv);
        }
        if(vc)
        {
            avcodec_free_context(&vc);
        }
        av_packet_unref(&vPacket);

        if(asc)
        {
            swr_free(&asc);
            asc = nullptr;
        }
        if (pcm)
        {
            av_frame_free(&pcm);
        }
        av_packet_unref(&aPacket);
    }
private:
    SwsContext *vsc = nullptr;
    AVFrame *yuv = nullptr;
    AVPacket vPacket = {0};

    SwrContext *asc = nullptr;
    AVFrame *pcm = nullptr;
    AVPacket aPacket {0};

    bool openCodec(AVCodecContext **c)
    {
        int ret = avcodec_open2(*c, 0, 0);
        if(ret != 0)
        {
            char err[1024] = { 0 };
            av_strerror(ret, err, sizeof(err) - 1);
            cout << err;
            avcodec_free_context(c);
            return false;
        }
        cout<<"openCodec OK!!";
        return true;
    }
    AVCodecContext* CreateCodec(AVCodecID cid)
    {
        AVCodec * codec = avcodec_find_encoder(cid);
        if(!codec)
        {
            cout<<"avcodec_find_encoder failed!";
            return nullptr;
        }
        AVCodecContext *c = avcodec_alloc_context3(codec);
        if(!c)
        {
            cout<<"avcodec_alloc_context3 failed!";
            return nullptr;
        }
        cout<<"CreateCodec ok!";
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        c->thread_count = GetCpuNum();
        c->time_base = { 1,1000000 };
        return c;
    }




};


TMediaEncode *TMediaEncode::Get(unsigned char index)
{
    static bool isFirst = true;
    if(isFirst)
    {
        avcodec_register_all();
        isFirst = false;
    }

    static CTMediaEncode tme[255];
    return &tme[index];
}

TMediaEncode::~TMediaEncode()
{

}

TMediaEncode::TMediaEncode()
{

}
