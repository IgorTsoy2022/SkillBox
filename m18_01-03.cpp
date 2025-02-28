#include <iostream>
#include <vector>

template<typename A>
void print(const A & arr, size_t from, size_t to) {
    std::cout << "[";
    if (from > to) {
        for (size_t i = from; i > to; --i) {
            std::cout << arr[i] << " ";
        }
    }
    else {
        for (size_t i = from; i < to; ++i) {
	    std::cout << arr[i] << " ";
	}
    }
    std::cout << arr[to] << "]\n";
}

template<typename T>
void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T, const size_t N>
void reverse(T* p) {
    for (int i = 0; i < N / 2; ++i) {
        swap(*(p + i), *(p + N - 1 - i));
    }
}

// Task 1. SWAP arrays.

template<typename T, const size_t N>
void swaparrays(std::vector<T>& v, T(&a)[N]) {
    for (size_t i = 0; i < N; ++i) {
        swap(v[i], a[i]);
    }
}

// Task 2. Count ways to reach N.

template<typename T, std::size_t N>
T count_ways(T step, T(&ways_storage)[N], T max_jump_range = 3) {
    if (step == 0) {
        return 1;
    }

    if (ways_storage[step - 1] > -1) {
        return ways_storage[step - 1];
    }

    T ways = 0;
    for (T i = 1; i <= max_jump_range && step - i >= 0; ++i) {
	ways += count_ways(step - i, ways_storage, max_jump_range);
    }
    ways_storage[step - 1] = ways;

    return ways;
}

template<typename T>
T count_ways(T step, std::vector<T> & ways_storage,
             T max_jump_range = 3) {
    if (step == 0) {
        return 1;
    }

    if (ways_storage.size() < step) {
	ways_storage.resize(step, -1);
    }

    if (ways_storage[step - 1] > -1) {
        return ways_storage[step - 1];
    }

    T ways = 0;
    for (T i = 1; i <= max_jump_range && step - i >= 0; ++i) {
	ways += count_ways(step - i, ways_storage, max_jump_range);
    }
    ways_storage[step - 1] = ways;

    return ways;
}

// Task 3. Count even digits.

void evendigits(long long value, int& evens_count) {
    if (value % 2 == 0) {
        ++evens_count;
    }

    if (value < 10) {
	return;
    }

    evendigits(value / 10, evens_count);
}

int main() {

    std::cout << "Task 1. SWAP arrays.\n";
    {
	std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8 };
	int a[8] = { 1, 4, 9, 16, 25, 36, 49, 56 };
	std::cout << "v = ";
	print(v, 0, 7);
	std::cout << "a = ";
	print(a, 0, 7);
	std::cout << "Arrays after swap:\n";
	swaparrays(v, a);
	std::cout << "v = ";
	print(v, 0, 7);
	std::cout << "a = ";
	print(a, 0, 7);
    }

    std::cout << "\nTask 2. Count ways to reach N.\n";
    {
	const int steps = 5;
	int max_jump_range = 3;
	int ways_storage[steps];
	for (size_t i = 0; i < steps; ++i) {
            ways_storage[i] = -1;
	}

	std::cout << "The rabbit can reach to step " << steps
                  << " with a maximum jump of " << max_jump_range << " in "
	          << count_ways(steps, ways_storage, max_jump_range)
		  << " ways.\n";
	}

    std::cout << "\nTask 3. Count even digits.\n";
    {
	long long bignum = 9223372036854775806;
	int evens_count = 0;
	evendigits(bignum, evens_count);
	std::cout << "There " << (evens_count > 1 ? "are " : "is ")
                  << evens_count << " even digit"
                  << (evens_count > 1 ? "s" : "")
                  << " in number: " << bignum << "\n";
   }

   return 0;
}
