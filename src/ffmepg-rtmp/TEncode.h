#ifndef TENCODE_H
#define TENCODE_H

/**
 * @brief The TEncode class 格式转换与编码接口类
 */
#include "TData.h"


struct AVFrame;
struct AVPacket;
class AVCodecContext;

enum sampleFMT
{
    X_S16 = 1,
    X_FLATP =8
};

class TEncode
{
public:
    int inWidth = 1280;
    int inHeight = 720;
    int inPixSize = 3;
    int channels = 2;
    int sampleRate = 44100;
    sampleFMT inSampleFmt = X_S16;


    int outWidth = 1280;
    int outHeight = 720;
    int bitRate = 8 * 1024 *50 ;//50KB


    AVCodecContext *vc = 0;
    AVCodecContext *ac = 0;
    
    static TEncode* Get(unsigned char index = 0);
    
    virtual bool initScale() = 0;
    
    virtual bool initResample(TData d) = 0;
    
    virtual AVFrame* RGBtoYUV(TData rgb) = 0;
    
    virtual bool initVideoCodec() = 0;
    
    virtual bool initAudioCodec() = 0;
      
    virtual TData encodeVideo(TData frame) = 0;
    
    virtual TData encodeAudie(TData frame) = 0;
    
    virtual void Close() = 0;
    
    virtual ~TEncode();
protected:
    TEncode();
};

#endif // TENCODE_H
