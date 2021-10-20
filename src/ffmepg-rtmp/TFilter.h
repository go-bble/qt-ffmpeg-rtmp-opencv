#ifndef TFILTER_H
#define TFILTER_H

enum TFilterType
{
    TBILATERAL
};
namespace cv
{
class Mat;
}

#include <string>
#include <map>

class TFilter
{
public:

    static TFilter *Get(TFilterType t= TBILATERAL);
    virtual bool Filter(cv::Mat *src, cv::Mat *des) = 0;
    virtual bool Set(std::string key, double value);
//    virtual bool ChangeVal(std::string key, double value);
    virtual ~TFilter();

protected:
    std::map<std::string, double>paras;
    TFilter();
};

#endif // TFILTER_H
