#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <iostream>
#include <random>
#include <iomanip>
#include "Queue.h"
#include "Computer.h"
#include "Process.h"
#include "Resource.h"

std::mt19937 globalRandomGenerator{std::random_device{}()};

class Simulator
{
private:
    Queue<Computer> computers;            // Fila com N computadores
    Queue<Process> ArrivalQueue;          // Fila para armazenar processos da entrada
    std::string escalationPolicy;         // Política de escalonamento (FCFS ou SJF)
    unsigned int numberActiveProcesses{}; // Quantidade de processos ativos no simulador
    unsigned int logicalClock{};          // Relogio logico
    double beginning_first_process;       // Variavel que captura o instant do primeiro processo

    // Variaveis do tipo string que indica o tipo dos recursos. Essas variaveis sao utilizadas nas chamadas dos metodos internos da classe
    std::string cpu = "CPU";
    std::string disco = "DISCO";
    std::string disco1 = "DISCO 1";
    std::string disco2 = "DISCO 2";
    std::string rede = "REDE";

public:
    // Construtor
    Simulator(unsigned int &numComputers, std::string &policy, std::string &processFile)
    {
        // Inicializa os computadores
        for (int i = 0; i < numComputers; i++)
        {
            Computer *newComputer = new Computer();
            computers.push(*newComputer);
        }

        // Atribui a politica desejada
        this->escalationPolicy = policy;

        // Carrega os processos do arquivo processFile
        FILE *file;
        file = fopen(processFile.c_str(), "rb");
        int instant, d_cpu, d_disk1, d_disk2;
        while (fscanf(file, "%i %i %i %i", &instant, &d_cpu, &d_disk1, &d_disk2) != EOF)
        {
            ArrivalQueue.push(Process(instant, d_cpu, d_disk1, d_disk2));
        }
        fclose(file);

        // Pega o instant do primeiro processo
        beginning_first_process = ArrivalQueue.front().getInstant();

        // Quantidade de processos ativos é iniciado
        numberActiveProcesses = static_cast<unsigned int>(ArrivalQueue.size());
    }

    // Destrutor da classe
    ~Simulator()
    {
        // Libera a memória alocada dinamicamente para objetos da classe Process
        for (Process &process : ArrivalQueue)
        {
            delete &process;
        }
        // Libera a memória alocada dinamicamente para objetos da classe computer
        for (Computer &computer : computers)
        {
            delete &computer;
        }
    }

    void simulate()
    {
        auto initialIteratorArrivalQueue = ArrivalQueue.begin(); // Inicializa o iterador com o início da ArrivalQueue

        while (true)
        {
            //  Condicao de parada
            if (numberActiveProcesses <= 0)
            {
                std::cout << "\nTempo medio de execucao: " << std::fixed << std::setprecision(5) << averageExecutionTime() << " segundos\n";
                std::cout << "Tempo de espera medio  : " << std::fixed << std::setprecision(5) << averageWaitingTime() << " segundos\n";
                std::cout << "Taxa de processamento  : " << std::fixed << std::setprecision(7) << processingRate() << " segundos\n";
                break;
            }
            //---------------------------------------------------------------------------------------------------------

            // verifica se no instante atual algum processo deve ser movido da fila de chegada para algum computador
            for (; initialIteratorArrivalQueue != ArrivalQueue.end(); ++initialIteratorArrivalQueue)
            {
                Process &process = *initialIteratorArrivalQueue;
                if (static_cast<unsigned int>(process.getInstant()) == logicalClock)
                {
                    // Escolhe um computador aleatório
                    int number = randomNumberGenerator(static_cast<int>(computers.size()));
                    auto computerIt = computers.begin();
                    for (int i = 1; i < number; ++i)
                    {
                        ++computerIt;
                    }
                    (*computerIt).getCPU().getQueue().push(&process);
                }
                else if (static_cast<unsigned int>(process.getInstant()) > logicalClock)
                {
                    // Os processos estão ordenados por instante; se o instante for maior que o relógio lógico, sai do loop.
                    break;
                }
            }
            //---------------------------------------------------------------------------------------------------------

            // Itera sobre os computadores
            for (Computer &computer : computers)
            {
                // Atualizando os recursos ocupados e passando para a fila do proximo recurso
                Process *p1 = updateCurrentProcess(computer.getCPU(), cpu);
                moveToNextResource(p1, computer, cpu);

                Process *p2 = updateCurrentProcess(computer.getDisk1(), disco);
                moveToNextResource(p2, computer, disco);

                Process *p3 = updateCurrentProcess(computer.getDisk2(), disco);
                moveToNextResource(p3, computer, disco);

                updateCurrentProcess(computer.getNetwork(), rede);
                //---------------------------------------------------------------------------------------------------------

                // É chamado o metodo responsavel por implementar a politica de escalonamento
                escalation(computer.getCPU(), cpu, computer, escalationPolicy);
                escalation(computer.getDisk1(), disco1, computer, escalationPolicy);
                escalation(computer.getDisk2(), disco2, computer, escalationPolicy);
                //---------------------------------------------------------------------------------------------------------
            }

            // E necessario analisar analisar a necessidade de escalonamento da rede uma vez por segundo
            escalation((*computers.begin()).getNetwork(), rede, (*computers.begin()), escalationPolicy);

            // Incrementar o logicalClock
            logicalClock++;
        }
    }

private:
    // Metodo que gera um numero aleatorio entre 1 e n
    int randomNumberGenerator(int n)
    {
        std::uniform_int_distribution<int> dist(1, n);
        return dist(globalRandomGenerator);
    }

    // Verifica se um recurso está ocupado. Caso esteja, o metodo verifica se no determinado instante o proceso termina de ser executado.
    // Caso sim, o recurso eh liberado e o processo eh retornado.
    // Se o processo for liberado da rede, o processo sai da simulação.
    Process *updateCurrentProcess(Resource &resource, std::string &type) // type ("CPU, "DISCO", "REDE")
    {
        if (resource.isAvailable())
        {
            // O recurso está disponível, nada a fazer.
            return nullptr;
        }

        // O recurso está em uso, eh verificado se ele deve ser liberado.
        Process &process = *(resource.getCurrentProcess());
        unsigned int demand = 0;

        if (type == cpu)
        {
            demand = process.getD_cpu();
        }
        else if (type == disco)
        {
            demand = process.getD_disk();
        }
        else if (type == rede)
        {
            demand = process.getD_rede();
        }

        if (process.getInstant() + demand == logicalClock)
        {
            // Recurso deve ser liberado.
            if (type == rede)
            {
                numberActiveProcesses -= 1;
            }
            resource.release();
            return &process;
        }
        // Recurso ainda não deve ser liberado.
        return nullptr;
    }

    // Metodo que recebe um proceso e move ele para a fila do proximo recurso.
    void moveToNextResource(Process *process, Computer &computer, std::string &type) // type ("CPU", "DISCO")
    {
        if (process == nullptr)
        {
            return;
        }

        if (type == cpu)
        {
            // escolhe um dos dois discos para colocar o processo na fila de espera
            int number = randomNumberGenerator(2);
            // instant é setado para o momento em que o processo entra em uma fila de espera
            process->setInstant(logicalClock);
            if (number == 1)
            {
                computer.getDisk1().getQueue().push(process);
            }
            else if (number == 2)
            {
                computer.getDisk2().getQueue().push(process);
            }
        }
        else if (type == disco)
        {
            // instant é setado para o momento em que o processo entra em uma fila de espera
            process->setInstant(logicalClock);
            computer.getNetwork().getQueue().push(process);
        }
    }

    // Metodo que escalo os processos de acordo com a politica de escalonamento
    void escalation(Resource &resource, std::string &type, Computer &computer, std::string &escalationPolicy) // type ("CPU", "DISCO 1", "DISCO 2", "REDE")
    {
        if (!resource.isAvailable() || resource.getQueue().empty())
        {
            return;
        }
        if (escalationPolicy == "SJF")
        {
            // Ordena a fila de espera do recurso
            sortProcessQueue(resource.getQueue(), type);
        }
        // Obtem o próximo processo na fila de espera do recurso
        Process *nextProcess = resource.getQueue().front();
        // soma o tempo que o processo passou nessa fila de espera a waitingTime
        nextProcess->setWatingTime(logicalClock - nextProcess->getInstant());
        // altera o instant para o instante atual (o instant é atualizada para facilitar a contagem do tempo em que o processo passa no recurso)
        nextProcess->setInstant(logicalClock);
        // Aloca o processo no recurso
        resource.allocate(nextProcess);
        // Remove o processo da fila de espera do recurso
        resource.getQueue().pop();

        // Caso o processo escalonado tenha demanda zero
        if (type == cpu)
        {
            if (nextProcess->getD_cpu() == 0)
            {
                Process *p1 = updateCurrentProcess(computer.getCPU(), cpu);
                moveToNextResource(p1, computer, cpu);
            }
        }
        else if (type == disco1)
        {
            if (nextProcess->getD_disk() == 0)
            {
                Process *p2 = updateCurrentProcess(computer.getDisk1(), disco);
                moveToNextResource(p2, computer, disco);
            }
        }
        else if (type == disco2)
        {
            if (nextProcess->getD_disk() == 0)
            {
                Process *p3 = updateCurrentProcess(computer.getDisk2(), disco);
                moveToNextResource(p3, computer, disco);
            }
        }
        else if (type == rede)
        {
            if (nextProcess->getD_rede() == 0)
            {
                updateCurrentProcess(computer.getNetwork(), rede);
            }
        }
    }

    // metodo que ordena a fila de espera do recurso
    // criterio: processo com menor demanda, em caso de empate o criterio é o menor instant(o processo que chegou primeiro na fila)
    void sortProcessQueue(Queue<Process *> &queue, const std::string &type) // type ("CPU", "DISCO", "REDE")
    {
        if (queue.size() <= 1)
        {
            // Nada a ordenar
            return;
        }

        // Usa o método getFirst para obter o primeiro elemento
        Node<Process *> *current = queue.getFirst();

        // Ordenando
        while (current != nullptr)
        {
            Node<Process *> *minNode = current;
            Node<Process *> *nextNode = current->next;

            while (nextNode != nullptr)
            {
                bool shouldSwap = false;

                if (type == cpu)
                {
                    shouldSwap = (nextNode->value->getD_cpu() < minNode->value->getD_cpu()) ||
                                 (nextNode->value->getD_cpu() == minNode->value->getD_cpu() && nextNode->value->getInstant() < minNode->value->getInstant());
                }
                else if (type == disco)
                {
                    shouldSwap = (nextNode->value->getD_disk() < minNode->value->getD_disk()) ||
                                 (nextNode->value->getD_disk() == minNode->value->getD_disk() && nextNode->value->getInstant() < minNode->value->getInstant());
                }
                else if (type == rede)
                {
                    shouldSwap = (nextNode->value->getD_rede() < minNode->value->getD_rede()) ||
                                 (nextNode->value->getD_rede() == minNode->value->getD_rede() && nextNode->value->getInstant() < minNode->value->getInstant());
                }

                if (shouldSwap)
                {
                    minNode = nextNode;
                }

                nextNode = nextNode->next;
            }

            // Troca os elementos
            Process *temp = current->value;
            current->value = minNode->value;
            minNode->value = temp;

            current = current->next;
        }
    }

    // Metodo que calcula o tempo medio de execução dos processos no simulador
    double averageExecutionTime()
    {
        double sum = 0.0;
        for (Process &p : ArrivalQueue)
        {
            int sumDemands = p.getD_cpu() + p.getD_disk() + p.getD_rede();
            // runtime eh igual a soma das demandas mais o tempo de espera do processo
            p.setRuntime(sumDemands + p.getWaitingTime());
            sum += p.getRuntime();
        }
        return sum / static_cast<double>(ArrivalQueue.size());
    }

    // Metodo que calcula o tempo medio de espera dos processos no simulador
    double averageWaitingTime()
    {
        double sum = 0.0;
        for (Process &p : ArrivalQueue)
        {
            sum += p.getWaitingTime();
        }
        return sum / static_cast<double>(ArrivalQueue.size());
    }

    // Metodo que alcula a taxa de processamento
    double processingRate()
    {
        int computer_number = computers.size();

        double finishing_last_process = 0.0; // instante em que o último processo termina

        finishing_last_process = logicalClock - 1; // pois o relógio para quando o último processo termina sua execução, ou seja, ele corresponde ao instante de seu término - 1

        return static_cast<double>(computer_number) / (finishing_last_process - beginning_first_process);
    }
};
#endif