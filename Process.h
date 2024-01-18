#ifndef PROCESS_H
#define PROCESS_H
#include <string>

class Process
{
private:
    int instant;     // Inicialmente, indica o instante em que o processo é iniciado no simulador. Posteriormente, é setado para o momento que entra em alguma fila de espera e quando é alocado para um recurso.
    int d_cpu;       // indica o tempo necessário para ser executado na cpu
    int d_disk;      // indica o tempo necessário no disco
    int d_rede;      // indica o tempo necessário na rede
    int runtime;     // indica o tempo que o processo passou no simulador (entrada em um computador até a saida na rede)
    int waitingTime; // indica o tempo que o processo passou em filas de espera

public:
    // construtor padrao
    Process()
    {
        instant = 0;
        d_cpu = 0;
        d_disk = 0;
        d_rede = 0;
        runtime = 0;
        waitingTime = 0;
    }
    // construtor com todos os parametros
    Process(int instant, int d_cpu, int d_disk, int d_rede)
    {
        this->instant = instant;
        this->d_cpu = d_cpu;
        this->d_disk = d_disk;
        this->d_rede = d_rede;
        this->runtime = 0;
        this->waitingTime = 0;
    }

    // retorna instant
    int getInstant() const
    {
        return instant;
    }
    // retorna d_cpu
    int getD_cpu() const
    {
        return d_cpu;
    }
    // retorna d_disk
    int getD_disk() const
    {
        return d_disk;
    }
    // retorna d_rede
    int getD_rede() const
    {
        return d_rede;
    }
    // retorna runtime
    int getRuntime()
    {
        return runtime;
    }
    // retorna waitingTime
    int getWaitingTime()
    {
        return waitingTime;
    }
    // passa um novo valor para instante
    void setInstant(int value)
    {
        this->instant = value;
    }
    // seta o valor de runtime
    void setRuntime(int value)
    {
        this->runtime = value;
    }
    // soma waitingTime com value
    void setWatingTime(int value)
    {
        this->waitingTime += value;
    }
};
#endif