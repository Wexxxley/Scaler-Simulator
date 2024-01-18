#ifndef COMPUTER_H
#define COMPUTER_H
#include "Resource.h"

class Computer
{
private:
    Resource *cpu;
    Resource *disk1;
    Resource *disk2;
    static Resource *network;

public:
    Computer();
    ~Computer();
    Resource &getCPU() const;
    Resource &getDisk1() const;
    Resource &getDisk2() const;
    Resource &getNetwork() const;
};

#endif
