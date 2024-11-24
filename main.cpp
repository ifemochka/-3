#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

class ArrayGenerator {
public:
    static std::vector<int> generateRandomArray(int size, int minValue, int maxValue) {
        std::vector<int> array(size);
        std::uniform_int_distribution<int> dist(minValue, maxValue);
        std::mt19937 rng(std::random_device{}());
        for (int i = 0; i < size; ++i) {
            array[i] = dist(rng);
        }
        return array;
    }

    static std::vector<int> generateReverseSortedArray(int size) {
        std::vector<int> array(size);
        for (int i = 0; i < size; ++i) {
            array[i] = size - i;
        }
        return array;
    }

    static std::vector<int> generateNearlySortedArray(int size) {
        int swaps = 10;
        std::vector<int> array = generateRandomArray(size, 0, 6000);
        std::sort(array.begin(), array.end());
        for (int i = 0; i < swaps; ++i) {
            int index1 = rand() % size;
            int index2 = rand() % size;
            std::swap(array[index1], array[index2]);
        }
        return array;
    }
};


// Стандартная реализация Quick Sort
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // выбираем последний элемент в качестве опорного
    int i = (low - 1); // индекс меньшего элемента

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

class SortTester {
public:
    static double testQuickSort(vector<int>& arr) {
        auto start = chrono::high_resolution_clock::now();
        quickSort(arr, 0, arr.size() - 1);
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration<double, milli>(end - start).count(); // возвращаем время в миллисекундах
    }
};

void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(arr[left + i], arr[left]);
        heapify(arr, n - 1, 0);
    }
}

void quickSort(vector<int>& arr, int left, int right, int depthLimit) {
    if (left >= right)
        return;

    if (right - left < 16) {
        insertionSort(arr, left, right);
        return;
    }

    if (depthLimit == 0) {
        heapSort(arr, left, right);
        return;
    }

    int pivot = arr[(left + right) / 2];
    int i = left;
    int j = right;

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    quickSort(arr, left, j, depthLimit - 1);
    quickSort(arr, i, right, depthLimit - 1);
}

class SortTester2 {
public:
    static double testHybridSort(vector<int>& arr) {
        int n = arr.size();
        int depthLimit = 2 * (int)(log2(n));
        auto start = chrono::high_resolution_clock::now();
        quickSort(arr, 0, n - 1, depthLimit);
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration<double, milli>(end - start).count(); // возвращаем время в миллисекундах
    }
};

int main() {
    int size = 10000;
    std::vector<int> randomArray = ArrayGenerator::generateRandomArray(size, 0, 6000);
    std::vector<int> reverseArray = ArrayGenerator::generateReverseSortedArray(size);
    std::vector<int> nearlySortedArray = ArrayGenerator::generateNearlySortedArray(size);

    std::cout << "QUICK SORT \n";
    double timeTaken = SortTester::testQuickSort(randomArray);
    cout << timeTaken << '\n';
    timeTaken = SortTester::testQuickSort(reverseArray);
    cout << timeTaken << '\n';
    timeTaken = SortTester::testQuickSort(nearlySortedArray);
    cout << timeTaken << '\n';

    std::cout << "QUICK + HEAP + INSERTION SORT \n";
    double timeTaken2 = SortTester2::testHybridSort(randomArray);
    cout << timeTaken2 << '\n';
    timeTaken2 = SortTester2::testHybridSort(reverseArray);
    cout << timeTaken2 << '\n';
    timeTaken2 = SortTester2::testHybridSort(nearlySortedArray);
    cout << timeTaken2 << '\n';
}
