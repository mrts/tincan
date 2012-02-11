#ifndef METAINFO_H__
#define METAINFO_H__

#include <string>

#include "disable_copy.h"

namespace tincan
{

template <const char* l>
struct Metainfo
{
    std::string label;

    Metainfo() :
        label(l)
    {}
private:
    DISABLE_COPY(Metainfo)
};

}

#endif /* METAINFO_H */
