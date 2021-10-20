#ifndef TCONTROLLER_H
#define TCONTROLLER_H

#include <TDataThread.h>
#include <string>
class TController : public TDataThread
{
public:
    static TController * Get();

    virtual bool SetFilter(std::string key , double val);
//    virtual bool FchangeVal(std::string key , double val);
    virtual bool Start();
    virtual void Stop();
    void run();

    virtual ~TController();

    std::string inUrl = "";
    std::string outUrl = "";
    int camIndex = -1;

protected:
    int vIndex = 0;
    int aIndex = 1;
    TController();
};

#endif // TCONTROLLER_H
