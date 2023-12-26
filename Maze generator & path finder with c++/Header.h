#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <random>
#include <fstream>
#include <string>
#include <algorithm>


using namespace std;



template <class Type>
class Stack {
public:
    Stack();
    Stack(const Stack& rhs);
    ~Stack();

    bool isEmpty() const;
    bool isFull() const;
    void makeEmpty();

    void pop();
    void push(const Type& x);
    Type topAndPop();
    const Type& top() const;

    const Stack& operator=(const Stack& rhs);

private:
    struct Node
    {
        Type element;
        Node* next;

        Node(const Type& _element, Node* _next = NULL) : element(_element), next(_next) {}
    };

    Node* topOfStack;
};

template <class Type>
Stack<Type>::Stack()
{
    topOfStack = NULL;
}

template <class Type>
bool Stack<Type>::isFull() const
{
    return false;
}

template <class Type>
bool Stack<Type>::isEmpty() const
{
    return topOfStack == NULL;
}

template <class Type>
const Type& Stack<Type>::top() const
{
    if (isEmpty())
        throw;
    return topOfStack->element;
}

template <class Type>
void Stack<Type>::pop()
{
    if (isEmpty())
        throw;
    Node* oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;
}

template <class Type>
void Stack<Type>::push(const Type& x)
{
    topOfStack = new Node(x, topOfStack);
}

template <class Type>
Type Stack<Type>::topAndPop()
{
    Type topItem = top();
    pop();
    return topItem;
}

template <class Type>
void Stack<Type>::makeEmpty()
{
    while (!isEmpty())
        pop();
}

template <class Type>
const Stack<Type>& Stack<Type>::operator=(const Stack<Type>& rhs)
{
    if (this != &rhs)
    {
        makeEmpty();
        if (rhs.isEmpty())
            return *this;

        Node* rptr = rhs.topOfStack;
        Node* ptr = new Node(rptr->element);
        topOfStack = ptr;

        for (rptr = rptr->next; rptr != NULL; rptr = rptr->next)
            ptr = ptr->next = new Node(rptr->element);
    }
    return *this;
}

template <class Type>
Stack<Type>::Stack(const Stack<Type>& rhs)
{
    topOfStack = NULL;
    *this = rhs; // deep copy 
}

template <class Type>
Stack<Type>::~Stack()
{
    makeEmpty();
}