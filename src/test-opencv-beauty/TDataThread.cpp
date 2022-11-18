#include "TDataThread.h"





bool TDataThread::Start()
{
    isExit = false;
    QThread::start();
    return true;
}

void TDataThread::Stop()
{
    isExit = true;
    wait();
}

void TDataThread::Push(TData d)
{
    mutex.lock();
    if(datas.size() > maxList)
    {
        datas.front().Drop();
        datas.pop_front();
    }
    datas.push_back(d);
    mutex.unlock();
}

TData TDataThread::Pop()
{
    mutex.lock();
    if(datas.empty())
    {
        mutex.unlock();
        return TData();
    }
    TData  d =  datas.front();
    datas.pop_front();
    mutex.unlock();
    return d;
}

void TDataThread::Clear()
{
    mutex.lock();
        while (!datas.empty())
        {
            datas.front().Drop();
            datas.pop_front();
        }
        mutex.unlock();
}

TDataThread::TDataThread()
{

}

TDataThread::~TDataThread()
{

}
