#ifndef DISABLE_COPY_H__
#define DISABLE_COPY_H__

#define DISABLE_COPY(Cls) \
    Cls(const Cls&); \
    Cls& operator=(const Cls&);

#endif /* DISABLE_COPY_H */
