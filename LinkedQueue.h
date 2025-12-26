#pragma once
#ifndef LINKED_QUEUE_H
#define LINKED_QUEUE_H

#include "Node.h"
#include "QueueADT.h"

template <typename T>
class LinkedQueue : public QueueADT<T> {
private:
    Node<T>* backPtr;
    Node<T>* frontPtr;
    int itemCount;

public:
    LinkedQueue();
    virtual ~LinkedQueue();
    bool isEmpty() const;
    bool enqueue(const T& newEntry);
    bool dequeue(T& frntEntry);
    bool peek(T& frntEntry) const;
    int count() const;
};

template <typename T> LinkedQueue<T>::LinkedQueue() {
    backPtr = nullptr;
    frontPtr = nullptr;
    itemCount = 0;
}

template <typename T> LinkedQueue<T>::~LinkedQueue() {
    T temp;
    while (dequeue(temp));
}

template <typename T> bool LinkedQueue<T>::isEmpty() const {
    return frontPtr == nullptr;
}

template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry) {
    Node<T>* newNodePtr = new Node<T>(newEntry);
    if (isEmpty()) {
        frontPtr = newNodePtr;
    }
    else {
        backPtr->setNext(newNodePtr);
    }
    backPtr = newNodePtr;
    itemCount++;
    return true;
}

template <typename T>
bool LinkedQueue<T>::dequeue(T& frntEntry) {
    if (isEmpty()) {
        return false;
    }
    Node<T>* nodeToDeletePtr = frontPtr;
    frntEntry = frontPtr->getItem();
    frontPtr = frontPtr->getNext();
    if (frontPtr == nullptr) {
        backPtr = nullptr;
    }
    delete nodeToDeletePtr;
    itemCount--;
    return true;
}

template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const {
    if (isEmpty()) {
        return false;
    }
    frntEntry = frontPtr->getItem();
    return true;
}

template <typename T> int LinkedQueue<T>::count() const {
    return itemCount;
}
#endif