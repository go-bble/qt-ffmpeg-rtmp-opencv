#include "TData.h"

#include <stdlib.h>
#include <string.h>
extern "C"
{
    #include<libavutil/time.h>
}

long long getCurTime()
{
    return  av_gettime();
}

void TData::Drop()
{
    if (data)
		delete data;
	data = 0;
	size = 0;
}
TData::TData(char *data, int size, long long p)
{
    this->data = new char[size];
    memcpy(this->data, data, size);
    this->size = size;
    this->pts = p;
}

TData::TData()
{

}
TData::~TData()
{

}
