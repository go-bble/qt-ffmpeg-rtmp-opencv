 #ifndef TAUDIORECORD_H
#define TAUDIORECORD_H

#include "TDataThread.h"

class TAudioRecord : public TDataThread
{
public:
    int channels = 2;
    int sampleRate = 44100;
    int sampleByte = 2;
    int nbSamples = 1024;

    virtual bool Init() = 0;

    virtual void Stop() = 0;

    static TAudioRecord *Get(unsigned char index = 0 );
    virtual ~TAudioRecord();
protected:
    TAudioRecord();
};

#endif // TAUDIORECORD_H
