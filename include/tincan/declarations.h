#ifndef DECLARATIONS_H__
#define DECLARATIONS_H__

// interface has virtual destructor and no public constructors
#define INTERFACE(Cls) \
    public: \
        virtual ~Cls(); \
    private: \
        Cls(); \
        Cls(const Cls&); \
        Cls& operator=(const Cls&);

// factory must be a singleton class that has only static methods
#define FACTORY(Cls) \
    private: \
        virtual ~Cls(); \
        Cls(); \
        Cls(const Cls&); \
        Cls& operator=(const Cls&);

#endif /* DECLARATIONS_H */
