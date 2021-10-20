#ifndef TMEDIAENCODE_H
#define TMEDIAENCODE_H
#include "TData.h"

class AVCodecContext;

enum XSampleFMT
{
    T_S16 = 1,
    T_FLATP = 8
};

class TMediaEncode
{
public:


    int inWidth      = 1280;
    int inHeight     = 720;
    int inPixSize    = 3;
    int inChannels   = 2;
    int inSampleRate = 44100;
    XSampleFMT inSampleFmt = T_S16;

    int outWidth     = 1280;
    int outHeight    = 720;
    int outBitrate   = 50 *1024*8;
    int outfps       = 30;
    int outNbSamples  = 1024; //采样数
    int outGopSize   = 50;
    int outMaxBFrames= 0;
    int outAudioBitrate = 40000;
    XSampleFMT outSampleFmt = T_FLATP;

    static TMediaEncode *Get(unsigned char index = 0);

    virtual bool InitScale() = 0;
    virtual TData RGBtoYUV(TData rgb) = 0;
    virtual bool initVideoCodec() = 0;
    virtual TData encodeVideo(TData frame) = 0;


    virtual bool InitResample() = 0;
    virtual TData Resample(TData d) = 0;
    virtual bool initAudioCodec() = 0;
    virtual TData encodeAudio(TData frame) = 0;

    virtual void Close() = 0;
    virtual ~TMediaEncode();

    AVCodecContext *vc = 0;
    AVCodecContext *ac = 0;
protected:
    TMediaEncode();


};

#endif // TMEDIAENCODE_H
