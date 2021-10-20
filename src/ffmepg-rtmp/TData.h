#ifndef TDATA_H
#define TDATA_H

extern "C"
{
#include <libavutil/time.h>
}


class TData
{
public:
    char *data = 0;
    int size = 0;
    long long pts = 0;
    
    void Drop();
   
    TData();
    
    TData(char *data, int size, long long p =0);
    
    virtual ~TData();
};

long long getCurTime();


#endif // TDATA_H
