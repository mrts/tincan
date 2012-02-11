#ifndef METAINFO_H__
#define METAINFO_H__

#include <string>

namespace tincan
{

template <const char* l>
struct Metainfo
{
    std::string label;

    Metainfo() :
        label(l)
    {}
};

}

#endif /* METAINFO_H */
