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
void swap(T * a, T * b) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

template<typename T>
void reverse(T* p, const size_t size) {
	for (int i = 0; i < size / 2; ++i) {
		swap(&(*(p + i)), &(*(p + size - 1 - i)));
	}
}

template<size_t N>
void prefix_func(const char* str, int (&z)[N]) {
	z[0] = 0;
	for (size_t i = 1; i < N; ++i) {
		int pos = z[i - 1];
		while (pos > 0 && str[pos] != str[i]) {
            pos = z[pos - 1];
		}
	    z[i] = pos + (str[pos] == str[i] ? 1 : 0);
	}
}

template<size_t N>
int find_first_kmp(const char* text, const char* str) {
	int pf[N];
	prefix_func(str, pf);

	int pos = 0;
	for (int i = 0; text[i] != '\0'; ++i) {
		while (pos < N ?
			(pos > 0) && (text[i] != str[pos]) : pos > 0) {
			pos = pf[pos - 1];
		}
		if (text[i] == str[pos]) {
			++pos;
		}
		if (pos == N) {
			return i - pos + 1;
		}
	}

	return -1;
}

template<size_t N>
void substr_kmp(const char* a, const char* b) {
	auto idx = first_kmp<N>(a, b);
	if (idx > -1) {
		std::cout << "String \"" << b << "\" is a substring of the string \"" << a << "\"\n";
		std::cout << "and starts at position " << idx << " of the string.\n";
	}
	else {
		std::cout << "String \"" << b << "\" isn't a substring of the string \"" << a << "\"\n";
	}
}

int find_first_brutal(const char * a, const char * b) {

    for (int i = 0; a[i] != '\0'; ++i) {
        bool matched = true;
        for (int a_i = i, b_i = 0;
                b[b_i] != '\0';
                ++a_i, ++b_i) {
            if (a[a_i] == '\0' ||
                 a[a_i] != b[b_i]) {
                 matched = false;
                 break;
             }
        }
        if (matched) {
        	return i;
        }
    }

    return -1;
}

void substr_brutal(const char* a, const char* b) {
	auto idx = find_first_brutal(a, b);
	if (idx > -1) {
		std::cout << "String \"" << b << "\" is a substring of the string \"" << a << "\"\n";
		std::cout << "and starts at position " << idx << " of the string.\n";
	}
	else {
		std::cout << "String \"" << b << "\" isn't a substring of the string \"" << a << "\"\n";
	}
}

int main() {

	std::cout << "Task 1. SWAP.\n";
	{
		int a = 1, b = 2;
		std::cout << "a = " << a << " b = " << b << "\n";
		swap(&a, &b);
		std::cout << "a = " << a << " b = " << b << "\n";
	}

	std::cout << "\nTask 2. Reverse.\n";
	{
		int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		print(arr, 0, 9);
		reverse(arr, 10);
		print(arr, 0, 9);
	}


	std::cout << "\nTask 3. Substring.\n";
	{
		const char* a = "Hello, world!";
		const char* b = "wor";
		const char* c = "banana";

//		substr_kmp<3>(a, b);
//		substr_kmp<6>(a, c);

        substr_brutal(a, b);
        substr_brutal(a, c);
	}
	
	return 0;
}