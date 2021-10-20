#include "TEncode.h"

extern "C"
{
    #include <libswscale/swscale.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswresample/swresample.h>
}
#if defined WIN32 || defined _WIN32
#include <windows.h>
#endif
static int getCpuNum()
{
#if defined WIN32 || defined _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return (int)sysinfo.dwNumberOfProcessors;
#elif defined __linux__
	return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined __APPLE__
	int numCPU = 0;
	int mib[4];
	size_t len = sizeof(numCPU);

	// set the mib for hw.ncpu
	mib[0] = CTL_HW;
	mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

						   // get the number of CPUs from the system
	sysctl(mib, 2, &numCPU, &len, NULL, 0);

	if (numCPU < 1)
	{
		mib[1] = HW_NCPU;
		sysctl(mib, 2, &numCPU, &len, NULL, 0);

		if (numCPU < 1)
			numCPU = 1;
	}
	return (int)numCPU;
#else
	return 1;
#endif
}
class CTEncode:public TEncode
{
public:
    bool initScale()
    {
        return  true;
    }

    bool initResample(TData d)
    {
        return  true;
    }

    AVFrame* RGBtoYUV(TData rgb)
    {
        return  nullptr;
    }

    bool initVideoCodec()
    {
        return  true;
    }

    bool initAudioCodec()
    {
        return  true;
    }

    TData encodeVideo(TData frame)
    {
        TData a;
        return a;
    }

    TData encodeAudie(TData frame)
    {
        TData a;
        return a;
    }

    void Close()
    {

    }

    ~CTEncode(){

    }


};

TEncode * TEncode::Get(unsigned char index)
{
    static bool isFirst = true;
    if(isFirst)
    {
        avcodec_register_all();
        isFirst = false;
    }
    static CTEncode ctc[255];
    return &ctc[index];
}
TEncode::TEncode()
{

}
TEncode::~TEncode()
{

}


