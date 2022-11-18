#ifndef TVIDEOCAPTURE_H
#define TVIDEOCAPTURE_H

#include "TDataThread.h"
#include "TFilter.h"
#include "vector"
class TVideoCapture : public TDataThread
{
public:
    int width = 0;
    int height = 0;
    int fps = 0;
    static TVideoCapture *Get(unsigned char index = 0);
    void addFilter(TFilter *f)
    {
        Tmutex.lock();
        filters.push_back(f);
        Tmutex.unlock();

    }
    virtual bool Init(int camIndex = 0) = 0;
    virtual bool Init(const char *url) = 0;
    virtual void Stop() = 0;
    virtual ~TVideoCapture();
protected:
    std::vector<TFilter*> filters;
    QMutex Tmutex;
    TVideoCapture();
};

#endif // TVIDEOCAPTURE_H
