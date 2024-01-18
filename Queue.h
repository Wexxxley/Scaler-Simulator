#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>

//--------------------------------------------------------------------------------------------------------------
// Struct de nó genérico que contém um valor e um ponteiro para o próximo nó.
template <typename T>
struct Node
{
    T value;
    Node<T> *next;

    // Construtor que inicializa um nó com um valor e um ponteiro para o próximo nó.
    Node(const T &val, Node *nextPtr)
    {
        value = val;
        next = nextPtr;
    }

    // Destrutor que libera a memória do nó e seus nós subsequentes.
    ~Node()
    {
        delete next;
    }
};
// fim da Struct Node
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
// Classe que implementa um iterador para percorrer a fila.
template <typename T>
class Iterator
{
private:
    Node<T> *current;

public:
    // Construtor que inicializa o iterador com um nó.
    Iterator(Node<T> *node) : current(node) {}

    // Retorna o valor atual apontado pelo iterador.
    T &operator*()
    {
        return current->value;
    }

    // Operador de pre-incremento
    Iterator &operator++()
    {
        current = current->next;
        return *this;
    }

    // Operador de diferença. Verifica se são diferentes
    bool operator!=(const Iterator &other) const
    {
        return current != other.current;
    }
};
// Fim da classe Iterator
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
// Classe que implementa uma fila do tipo FIFO.
template <typename T>
class Queue
{
private:
    Node<T> *m_first{}; // ponteiro para o inicio da fila
    Node<T> *m_last{};  // ponteiro para o final da fila
    unsigned m_size{};  // numero de elemntos na fila

public:
    // Retorna um iterador apontando para o início da fila.
    Iterator<T> begin()
    {
        return Iterator<T>(m_first);
    }

    // Retorna um iterador apontando para o final da fila
    Iterator<T> end()
    {
        return Iterator<T>(nullptr);
    }

    // Construtor default
    Queue() = default;

    // Retorna o número de elementos na fila.
    unsigned size() const
    {
        return m_size;
    }

    // Verifica se a fila esta vazia
    bool empty() const
    {
        return m_size == 0;
    }

    // Insere um elemento no final da fila
    void push(const T &elem)
    {
        if (m_first == nullptr)
        { // Caso fila vazia
            m_last = m_first = new Node<T>(elem, nullptr);
        }
        else
        {
            m_last->next = new Node<T>(elem, nullptr);
            m_last = m_last->next;
        }
        m_size++;
    }

    // Remove um elemento do  início da fila
    // Se a fila estiver vazia nao faz nada
    void pop()
    {
        if (m_size != 0)
        {
            Node<T> *aux = m_first;
            m_first = m_first->next;
            aux->next = nullptr;
            delete aux;
            m_size--;
            if (m_size == 0)
            {
                m_last = nullptr;
            }
        }
    }

    // Acessa o primeiro elemento da fila
    T &front()
    {
        if (m_size == 0)
        {
            static T nullObject;
            return nullObject;
        }
        else
        {
            return m_first->value;
        }
    }

    // Funcao const que acessa o primeiro elemento da fila.
    const T &front() const
    {
        if (m_size == 0)
        {
            static T nullObject;
            return nullObject;
        }
        else
        {
            return m_first->value;
        }
    }

    // Funcao que acessa o ultimo elemento da fila.
    T &back()
    {
        if (m_size == 0)
        {
            static T nullObject;
            return nullObject;
        }
        else
        {
            return m_last->value;
        }
    }

    // Acessa o ultimo elemento da fila.
    const T &back() const
    {
        if (m_size == 0)
        {
            static T nullObject;
            return nullObject;
        }
        else
        {
            return m_last->value;
        }
    }

    // Itera sobre a fila e imprime o estado atual.
    void queueState(const Iterator<T> &begin, const Iterator<T> &end) const
    {
        for (auto it = begin; it != end; ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    Node<T> *getFirst() const
    {
        return m_first;
    }
};
// Fim da classe Queue
//--------------------------------------------------------------------------------------------------------------
#endif