#include <iostream>

template<typename T>
void print(const T* arr, size_t from, size_t to) {
	std::cout << "[";
	if (from > to) {
		for (size_t i = from; i > to; --i) {
			std::cout << arr[i] << ", ";
		}
	}
	else {
		for (size_t i = from; i < to; ++i) {
			std::cout << arr[i] << ", ";
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

template<typename T>
void reverse(T* p, int size = 10) {
	for (int i = 0; i < size / 2; ++i) {
		swap(*(p + i), *(p + size - 1 - i));
    }
}

void prefix_func(const char* str, int z[]) {

	z.resize(str.size());
	z[0] = 0;
	for (int i = 1; i < z.size(); ++i) {
		int pos = z[i - 1];
		while (pos > 0 && str[pos] != str[i])
			pos = z[pos - 1];
		z[i] = pos + (str[pos] == str[i] ? 1 : 0);
	}
}

void kmp(const string& text, const string& str) {
	vector<int> z;
	prefix_func(str, z);
	int pos = 0;
	for (int i = 0; i < text.size(); ++i) {
		while (pos > 0 && (pos >= str.size() || str[pos] != text[i]))
			pos = z[pos - 1];
		if (text[i] == str[pos]) pos++;
		if (pos == str.size())
			printf("%d ", i - pos + 1);
	}
}

int main() {
	std::cout << "Task 1. SWAP.\n";
	{
		int a = 1, b = 2;
		std::cout << "a = " << a << " b = " << b << "\n";
		swap(a, b);
		std::cout << "a = " << a << " b = " << b << "\n";
	}

	std::cout << "\nTask 2. Reverse.\n";
	{
		int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		print(arr, 0, 9);
		reverse(arr);
		print(arr, 0, 9);
	}


	std::cout << "\nTask 3. Substring.\n";
	{
		const char* a = "Hello, world!";
		const char* b = "wor";
		const char* c = "banana";

		std::cout << std::boolalpha;

	}
	return 0;
}