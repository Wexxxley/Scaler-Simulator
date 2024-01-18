#include "Computer.h"

// Inicialização da variável estática network
Resource *Computer::network = nullptr;

Computer::Computer()
{
    this->cpu = new Resource();
    this->disk1 = new Resource();
    this->disk2 = new Resource();

    // Inicializa o membro estático network, se ainda não estiver inicializado
    if (network == nullptr)
    {
        network = new Resource();
    }
}

Computer::~Computer()
{
    delete cpu;
    delete disk1;
    delete disk2;
    // Deletar a rede apenas quando apropriado
}

Resource &Computer::getCPU() const
{
    return *cpu;
}

Resource &Computer::getDisk1() const
{
    return *disk1;
}

Resource &Computer::getDisk2() const
{
    return *disk2;
}

Resource &Computer::getNetwork() const
{
    return *network;
}
