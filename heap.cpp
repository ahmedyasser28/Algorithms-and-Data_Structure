#include <iostream>
#include <vector>
#include <limits>
#include <stdexcept>
using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}


template <typename T>
class Heap {
    vector<T> heap;

    void heapify(int i, int heapSize) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < heapSize && heap[left] > heap[largest]) {
            largest = left;
        }

        if (right < heapSize && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest != i) {
            swap(heap[i], heap[largest]);
            heapify(largest, heapSize);
        }
    }

public:
    void insert(T key) {
        heap.push_back(numeric_limits<T>::min());
        increase(heap.size() - 1, key);
    }

    void increase(int i, T key) {
        if (key < heap[i]) {
            throw invalid_argument("New key is smaller than the current key.");
        }

        heap[i] = key;
        while (i > 0 && heap[(i - 1) / 2] < heap[i]) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    T extractMax() {
        if (heap.empty()) {
            throw out_of_range("Heap underflow.");
        }

        T max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapify(0, heap.size());
        }

        return max;
    }

    T extractMin() {
        if (heap.empty()) {
            throw out_of_range("Heap underflow.");
        }

        int minIndex = heap.size() / 2;
        T minValue = heap[minIndex];

        for (int i = minIndex + 1; i < heap.size(); i++) {
            if (heap[i] < minValue) {
                minValue = heap[i];
                minIndex = i;
            }
        }

        swap(heap[minIndex], heap.back());
        heap.pop_back();

        return minValue;
    }

    int size() const {
        return heap.size();
    }

    void printHeap() const {
        for (const T el : heap) {
            cout << el << " ";
        }
        cout << endl;
    }

    void heapSort(vector<T>& array) {
        buildMaxHeap(array);

        for (int i = heap.size() - 1; i >= 0; i--) {
            swap(heap[0], heap[i]);
            heapify(0, i);
        }

        array = heap;
    }

    void buildMaxHeap(const vector<T>& array) {
        heap = array;
        for (int i = heap.size() / 2 - 1; i >= 0; i--) {
            heapify(i, heap.size());
        }
    }
};

template <typename T>
class PriorityQueue {
    Heap<pair<int, T>> pqHeap;

public:
    void insert(T value, int priority) {
        pqHeap.insert(make_pair(priority, value));
    }

    T extractHighestPriority() {
        if (pqHeap.size() == 0) {
            throw out_of_range("Priority queue underflow.");
        }
        return pqHeap.extractMax().second;
    }

    bool isEmpty() const {
        return pqHeap.size() == 0;
    }

    int size() const {
        return pqHeap.size();
    }

    void printQueue() const {
        pqHeap.printHeap();
    }
};

int main() {
    PriorityQueue<int> pq;

    pq.insert(100, 5);
    pq.insert(200, 1);
    pq.insert(300, 3);
    pq.insert(400, 4);
    pq.insert(500, 2);

    cout << "Priority Queue elements (in heap): " << endl;
    pq.printQueue();

    cout << "Extracting highest priority task: " << pq.extractHighestPriority() << endl;
    cout << "Priority Queue after extraction: " << endl;
    pq.printQueue();

    cout << "Extracting highest priority task: " << pq.extractHighestPriority() << endl;
    cout << "Priority Queue after extraction: " << endl;
    pq.printQueue();

    return 0;
}
