#ifndef TBILATERALFILTER_H
#define TBILATERALFILTER_H

#include "TFilter.h"

class TBilateralFilter : public TFilter
{
public:
    TBilateralFilter();
    bool Filter(cv::Mat *src, cv::Mat *des) override;
    virtual ~TBilateralFilter();
};

#endif // TBILATERALFILTER_H
