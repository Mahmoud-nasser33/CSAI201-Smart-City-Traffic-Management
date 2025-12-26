#pragma once
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

template <class T>
class PrNode {
public:
    T* item;
    int priority;
    PrNode<T>* next;

    PrNode(T* i, int p) {
        item = i;
        priority = p;
        next = nullptr;
    }
};

template <class T>
class PriorityQueue {
private:
    PrNode<T>* head;
    int itemCount;
public:
    PriorityQueue() {
        head = nullptr;
        itemCount = 0;
    }

    ~PriorityQueue() {
        while (head) {
            PrNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int count() const {
        return itemCount;
    }

    void Enqueue(T* item, int p) {
        PrNode<T>* newNode = new PrNode<T>(item, p);
        if (isEmpty() || p > head->priority) {
            newNode->next = head;
            head = newNode;
        }
        else {
            PrNode<T>* temp = head;
            while (temp->next != nullptr && p <= temp->next->priority) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
        itemCount++;
    }

    T* Dequeue() {
        if (isEmpty()) return nullptr;
        PrNode<T>* temp = head;
        T* item = temp->item;
        head = head->next;
        delete temp;
        itemCount--;
        return item;
    }

    bool peek(T*& item) const {
        if (isEmpty()) return false;
        item = head->item;
        return true;
    }
};
#endif