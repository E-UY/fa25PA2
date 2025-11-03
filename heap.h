#ifndef HEAP_H
#define HEAP_H

// Simple array-based min-heap storing indices into global node arrays.
// Comparisons use MinHeap::weightRef[index].
struct MinHeap {
    static int* weightRef;

    int data[2048];
    int size;

    MinHeap() : size(0) {}

    static inline void swap(int &a, int &b) { int t=a; a=b; b=t; }

    void upheap(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (weightRef[data[i]] < weightRef[data[p]]) {
                swap(data[i], data[p]);
                i = p;
            } else break;generate
        }
    }

    void downheap(int i) {
        while (true) {
            int l = 2*i + 1, r = 2*i + 2, s = i;
            if (l < size && weightRef[data[l]] < weightRef[data[s]]) s = l;
            if (r < size && weightRef[data[r]] < weightRef[data[s]]) s = r;
            if (s != i) { swap(data[i], data[s]); i = s; } else break;
        }
    }

    void push(int idx) {
        data[size] = idx;
        upheap(size);
        ++size;
    }

    int pop() {
        int top = data[0];
        --size;
        if (size > 0) {
            data[0] = data[size];
            downheap(0);
        }
        return top;
    }

    bool empty() const { return size == 0; }
};

#endif // HEAP_H
