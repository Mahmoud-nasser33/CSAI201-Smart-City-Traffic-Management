#pragma once
#ifndef NODE_H
#define NODE_H

template<typename T>
class Node {
private:
    T item;
    Node<T>* next;

public:
    Node() {
        next = nullptr;
    }

    Node(const T& newItem) {
        item = newItem;
        next = nullptr;
    }

    void setItem(const T& newItem) {
        item = newItem;
    }

    void setNext(Node<T>* nextPtr) {
        next = nextPtr;
    }

    T getItem() const {
        return item;
    }

    Node<T>* getNext() const {
        return next;
    }
};
#endif