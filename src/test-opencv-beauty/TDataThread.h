#ifndef TDATATHREAD_H
#define TDATATHREAD_H

#include <QThread>
#include <list>
#include <TData.h>
#include <QMutex>
class TDataThread : public QThread
{
public:
    //默认缓冲列表最大值
    int maxList = 100;

    virtual bool Start();
    virtual void Stop();

    virtual void Push(TData d);
    virtual TData Pop();

    virtual void Clear();

    TDataThread();
    virtual ~TDataThread();
protected:
    std::list<TData> datas;
    int dataSize = 0;
    QMutex mutex;
    bool isExit = true;//<bo>

};

#endif // TDATATHREAD_H
