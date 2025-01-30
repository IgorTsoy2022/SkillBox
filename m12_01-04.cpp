#include <iostream>
#include <string>
#include <cassert>
//#include <vector>

template<typename T>
void print(const T * arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << ", ";
        }
    } else {
        for (size_t i = from; i < to; ++i) {
            std::cout << arr[i] << ", ";
        }
    }
    std::cout << arr[to] << "]";
}

template<typename T>
void swap(T & a, T & b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void heapify(T * arr, int n, int i){
    // Initialize largest as root
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template<typename T>
void heapSort(T * arr, int size){
    for (int i = size / 2 - 1; i >= 0; --i) {
        heapify(arr, size, i);
    }

    // One by one extract an element from heap
    for (int i = size - 1; i > 0; --i) {
        // Move current root to end
        swap(arr[0], arr[i]);
        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

template<typename T>
void merge(T * arr, int left, int mid, int right) {
    int size1 = mid - left + 1;
    int size2 = right - mid;

    T left_arr[size1], right_arr[size2];

    for (int i = 0; i < size1; i++) {
        left_arr[i] = arr[left + i];
    }
    for (int j = 0; j < size2; j++) {
        right_arr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0;
    int k = left;

    while (i < size1 && j < size2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < size1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < size2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

template<typename T>
void mergeSort(T * arr, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

float travelTime(float distance, float speed) {
    assert (speed > 0);
    return distance / speed;
}

int main () {
    std::cout << std::boolalpha;
 
    std::cout << "Task 1. Apartment building.\n";
    std::string residents[10] = {
        "SidorovA", "IvanovA", "PetrovA",
        "SidorovB", "IvanovB", "PetrovB",
        "SidorovC", "IvanovC", "PetrovC", 
        "SidorovD"
    };

    int appart = 0;
/*
    std::cout << "Enter apartment number: ";
    for (int i = 0; i < 3; ++i) {
        std::cin >> appart;
        if (appart > 0 && appart < 11) {
            std::cout << residents[appart - 1] << "\n";
        }
        else {
            std::cout << "There is no apartment with this number.\n";
        }
    }

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');

    std::cout << "\nTask 2. Sorting.\n";
    float numbers[] = { 
        1.2, 2.3, 1.11, 3.4, 5.5,
        5.4, 5.3, 5.1, 1.5, 1.25,
        5.41, 5.31, 5.11, 1.51, 1.251
    };

    std::cout << "Input:\n";
    print(numbers, 0, 14);
    std::cout << "\n";
 //   heapSort(numbers, 15);
    mergeSort(numbers, 0, 14);
    std::cout << "Output:\n";
    print(numbers, 14, 0);
    std::cout << "\n";

    std::cout << "\nTask 3. Using assert.\n";
    float distance = 0;
    float speed = 0;
    std::cout << "Enter distance: ";
    std::cin >> distance;
    std::cout << "Enter speed: ";
    std::cin >> speed;
 
    std::cout << "Travel time is " <<  travelTime(distance, speed) << "\n";
*/
    std::cout <<  "\nTask 4. A series of numbers.\n";
    int numbers_count = 15;
    int set_numbers = 14;
    int snumbers[] = {
    	114, 111, 106, 107, 108,
    	105, 115, 108, 110, 109,
    	112, 113, 116, 117, 118
    };
    print(snumbers, 0, 14);
    std::cout << "\n";

    int start = snumbers[0];
    int repeating_number = 0;
    for (int & num : snumbers) {
        start = start < num ? start : num;
    }
    std::cout << "Given " << numbers_count << " numbers from a set of " << set_numbers << " consecutive numbers.\n";
    std::cout << "This set starts with the number " << start << ".\n";
    std::cout << "The repeating number is " << repeating_number << ".\n";

    return 0;
}
