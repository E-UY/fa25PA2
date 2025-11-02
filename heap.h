//
// Created by Manju Muralidharan on 10/19/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

struct MinHeap {
    int data[256];
    int size;

    MinHeap() { size = 0; }

    inline bool empty() const { return size == 0; }
    extern int *weightRef;

    //added swap function
    void swap(int &a, int &b) {
        int t = a; a = b; b = t;
    }


    void push(int idx) {
        // insert index at end of heap, restore order using upheap()
        data[size] = idx;
        upheap(size);
        ++size;
    }

    int pop() {
        // remove and return smallest index
        int top = data[0];
        --size;
        if (size > 0) {
            data[0] = data[size];
            downheap(0);
        }
        return top;
    }

    void upheap(int i) {
        // TODO: swap child upward while smaller than parent
        while (i>0) {
            int p = (i-1)/2;
            if (weightRef[data[i]] < weightRef[data[p]]) {
                swap(data[i],data[p]);
                i = p;
            }
            else {
                break;
            }
        }
    }

    void downheap(int i) {
        // TODO: swap parent downward while larger than any child
        while (true) {
            int l =  2 * i + 1 ;
            int r =  2 * i + 2;
            int smallest = i;

            if (l < size && weightRef[data[l]] < weightRef[data[smallest]])
                smallest = l;
            if (r < size && weightRef[data[r]] < weightRef[data[smallest]])
                smallest = r;

            if  (smallest != i) {
                swap(data[i], data[smallest]);
                i = smallest;
            }
            else {
                break;
            }
        }
    }


#endif