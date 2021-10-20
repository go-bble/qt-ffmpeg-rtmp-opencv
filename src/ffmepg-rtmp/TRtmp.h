#ifndef TRTMP_H
#define TRTMP_H

#include "TData.h"
class AVCodecContext;
class AVPacket;


class TRtmp
{
public:


    static TRtmp *Get(unsigned char index = 0);

    virtual bool Init(const char *url) = 0;

    virtual int addStream(const AVCodecContext *c) = 0;

    virtual bool sendHead() = 0;

    virtual bool sendFrame( TData d, int streamIndex = 0) =0;

    virtual void Close() = 0;

    virtual ~TRtmp();

protected:
    TRtmp();


};

#endif // TRTMP_H
