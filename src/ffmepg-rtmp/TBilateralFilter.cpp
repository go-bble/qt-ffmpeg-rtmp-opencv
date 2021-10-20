#include "TBilateralFilter.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

TBilateralFilter::TBilateralFilter()
{
    paras.insert(make_pair("d",5));
}

bool TBilateralFilter::Filter(Mat *src, Mat *des)
{
    double d = paras["d"];
    bilateralFilter(*src, *des, d, d*2, d/2);
    return true;

}
TBilateralFilter::~TBilateralFilter()
{

}
