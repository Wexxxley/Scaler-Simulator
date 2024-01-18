#include <iostream>
#include "Simulator.h"
#include "Process.h"
#include "Resource.h"
#include "Computer.h"
#include "Queue.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Uso: " << argv[0] << " <politica> <arquivo_de_processos>\n";
        return 1;
    }

    string policy = argv[1];
    string processFile = argv[2];

    unsigned int numComputers;

    cout << "---------------------------------------------------------------------\n";
    cout << "                   Bem-vindo ao NeoLook Simulator                    \n";
    cout << "---------------------------------------------------------------------\n";

    cout << "Digite a quantidade de computadores a serem utilizados no simulador: ";
    if (!(cin >> numComputers))
    {
        cerr << "Entrada invalida para a quantidade de computadores." << endl;
        return 1;
    }

    // Tenta criar o objeto Simulator
    Simulator *neoLookSimulator = new Simulator(numComputers, policy, processFile);
    try
    {
        // Tenta iniciar a simulação
        (*neoLookSimulator).simulate();
        cout << "\nSimulacao concluida com sucesso.\n\n";
    }
    catch (const exception &ex)
    {
        cerr << "Erro: " << ex.what() << endl;
    }
    delete neoLookSimulator;

    return 0;
}
