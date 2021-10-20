#include "TFilter.h"
#include "TBilateralFilter.h"
#include <iostream>

TFilter *TFilter::Get(TFilterType t)
{
    static TBilateralFilter tbf;
    switch (t) {
    case TBILATERAL:
        return &tbf;
        break;
    default:
        break;
    }
    return nullptr;
}

bool TFilter::Set(std::string key, double value)
{
    if(paras.find(key) == paras.end())
    {
        std::cout << key <<" is not support! " << std::endl;
        return false;
    }
    paras[key] = value;
    return true;
}
/*--start: add by bo*/

/*--end : add by bo*/
//bool TFilter::ChangeVal(std::string key, double value)
//{
//    if(paras.find(key) == paras.end())
//    {
//        std::cout << key <<" ChangeVal is not support! " << std::endl;
//        return false;
//    }
//    paras.find(key)->second = value;
//}
TFilter::TFilter()
{

}
TFilter::~TFilter()
{

}
