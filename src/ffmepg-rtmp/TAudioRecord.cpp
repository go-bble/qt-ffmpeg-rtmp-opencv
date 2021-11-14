#include "TAudioRecord.h"
#include <QAudioInput>
#include <iostream>
#include <list>

using namespace std;
class CTAudioRecord :public TAudioRecord
{
public:
    void run()
    {
        cout << "CTAudioRecord->TDataThread.start" << endl;
        int readSize = nbSamples *channels *sampleByte;
        char *pcm = new char[readSize];
        while (!isExit)
        {
            if(input->bytesReady() <readSize)
            {
                QThread::msleep(1);
                continue;
            }

            int useSize = 0;
            while (useSize != readSize)
            {
                int len = io->read(pcm + useSize,readSize - useSize);
                if(len<0)break;
                useSize += len;
            }
            if(useSize != readSize)continue;
            long long pts = getCurTime();
            Push(TData(pcm, readSize, pts));
        }


        delete[] pcm;
        cout << "CTAudioRecord->TDataThread.quit" << endl;

    }
    bool Init()
    {
        Stop();
        QAudioFormat fmt;
        fmt.setSampleRate(sampleRate);
        fmt.setChannelCount(channels);
        fmt.setSampleSize(sampleByte * 8);
        fmt.setCodec("audio/pcm");
        fmt.setByteOrder(QAudioFormat::LittleEndian);
        fmt.setSampleType(QAudioFormat::UnSignedInt);
        QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
        if (!info.isFormatSupported(fmt))
        {
            cout << "Audio Format not Supported!" << endl;
            fmt = info.nearestFormat(fmt);
        }
        input = new QAudioInput(fmt);

        //开始录制音频
        io = input->start();
        if (!io)
            return false;
        return true;

    }
    void Stop(){
        TDataThread::Stop();
        if(input)
        {
            input->stop();
        }
        if(io)
        {
            io->close();
        }
        input = nullptr;
        io = nullptr;
    }
    QAudioInput *input = nullptr;
    QIODevice *io = nullptr;
};




TAudioRecord *TAudioRecord:: Get(unsigned char index)
{
    static CTAudioRecord record[255];
    return &record[index];
}
TAudioRecord::~TAudioRecord()
{

}

TAudioRecord::TAudioRecord()
{

}
