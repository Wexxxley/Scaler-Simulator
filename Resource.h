#ifndef RESOURCE_H
#define RESOURCE_H
#include "Queue.h"
#include "Process.h"

class Resource
{
private:
    bool available = true;             // Indica se o recurso está disponível
    Process *currentProcess = nullptr; // Processo atualmente alocado ao recurso, se houver
    Queue<Process *> waitingQueue{};   // Fila de espera de processos

public:
    Resource() = default;

    // verifica se o recurso esta disponivel
    bool isAvailable() const
    {
        return available;
    }

    // Aloca o recurso ao processo
    void allocate(Process *process)
    {
        this->currentProcess = process;
        this->available = false;
    }
    // libera o recurso
    void release()
    {
        this->currentProcess = nullptr;
        this->available = true;
    }
    // retorna o processo que está alocado no recurso
    Process *getCurrentProcess() const
    {
        return currentProcess;
    }

    // retorna a fila de espera do recurso
    Queue<Process *> &getQueue()
    {
        return waitingQueue;
    }
};
#endif
