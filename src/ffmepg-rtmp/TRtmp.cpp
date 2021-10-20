#include "TRtmp.h"
#include <iostream>
#include <string>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavutil/time.h>
}

using namespace std;

class CTRtmp:public TRtmp
{
public:


     bool Init(const char *url)
     {
        int ret = avformat_alloc_output_context2(&ic, 0, "flv", url);
        this->url = url;
        if(ret != 0)
        {
            char buf[1024] = {0};
            av_strerror(ret, buf,sizeof(buf)-1);
            cout<<buf<<endl;
            return  false;
        }
        return true;
     }

     int addStream(const AVCodecContext *c)
     {
        if(!c)return -1;
        AVStream *st = avformat_new_stream(ic, nullptr);
        if(!st)
        {
            cout <<"avformat_new_stream() failed!!!!";
            return -1;
        }
        st->codecpar->codec_tag = 0;//若AVStream->codecpar->codec_tag为0（默认情况下为0），
                                    //则FFmpeg会根据编码ID（AVCodecID）从封装格式的codec_tag列表中，找到一个codec_tag。
        avcodec_parameters_from_context(st->codecpar,c);
        av_dump_format(ic, 0, url.c_str(), 1);

        if(c->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            vc = c;
            vs = st;
        }
        else if(c->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            ac = c;
            as = st;
        }

        return st->index;
     }
     bool sendHead()
     {

         int ret = avio_open(&ic->pb, url.c_str(), AVIO_FLAG_WRITE);
         if(ret != 0 )
         {
             char buf[1024] = {0};
             av_strerror(ret, buf,sizeof(buf)-1);
             cout<<buf<<"-avio_open()"<<endl;
             return  false;
         }

         ret = avformat_write_header(ic,nullptr);
         if(ret != 0 )
         {
             char buf[1024] = {0};
             av_strerror(ret, buf,sizeof(buf)-1);
             cout<<buf<<"-avformat_write_header()"<<endl;
             return  false;
         }
         return true;
     }

     bool sendFrame( TData d, int streamIndex = 0)
     {
        if(!d.data || d.size <= 0)return false;


        AVPacket *pack = (AVPacket *)d.data;
        pack->stream_index = streamIndex;
        AVRational stime;
        AVRational dtime;

        if(vs && vc && pack->stream_index == vs->index)
        {
            stime = vc->time_base;
            dtime = vs->time_base;
        }else if (as && ac && pack->stream_index == as->index)
        {
            stime = ac->time_base;
            dtime = as->time_base;
        }else
        {
            return false;
        }

        pack->pts = av_rescale_q(pack->pts, stime, dtime);
        pack->dts = av_rescale_q(pack->dts, stime, dtime);
        pack->duration = av_rescale_q(pack->duration,stime, dtime);

        int ret = av_interleaved_write_frame(ic, pack);//有缓存，而av_write_frame()没有
        if (ret == 0)
        {
            //cout << "." << flush;
            return true;
        }
        return false;

      }
     void Close()
     {
        if(ic)
        {
            avformat_close_input(&ic);
            vs = nullptr;
            as = nullptr;
        }
        vc =nullptr;
        ac =nullptr;
        url = "";
     }

private:
     AVFormatContext *ic = nullptr;

     const AVCodecContext *vc = nullptr;
     const AVCodecContext *ac = nullptr;

     AVStream *vs = nullptr;
     AVStream *as = nullptr;


    string url = "";
};



TRtmp *TRtmp::Get(unsigned char index)
{
    static CTRtmp ctr[255];
    static bool isFirst = true;
    if(isFirst)
    {
        av_register_all();
        avformat_network_init();
        isFirst = false;
    }
    return &ctr[index];
}

TRtmp::~TRtmp()
{

}

TRtmp::TRtmp()
{

}
