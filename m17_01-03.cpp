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

std::vector<int> prefix_function(const std::string & str) {
	auto size = str.size();
	if (size < 2) {
		return { 0 };
	}

	std::vector<int> max_prefixes(size, 0);

	for (size_t i = 1; i < size; ++i) {
		auto j = max_prefixes[i - 1];
		auto matched = (str[i] == str[j]);
		while (j > 0 && !matched) {
			j = max_prefixes[j - 1];
			matched = (str[i] == str[j]);
		}
		max_prefixes[i] = j + (matched ? 1 : 0);
	}

	return max_prefixes;
}

std::string from_prefix(const std::vector<int>& prefixes) {
	std::string text;
	char c = 'a';
	for (int i = 0; i < prefixes.size(); ++i) {
		if (prefixes[i] == 0) {
			text += c++;
		}
		else {
			text += text[prefixes[i] - 1];
		}
	}
	return text;
}

std::vector<int> z_function(const std::string & str) {
	auto size = str.size();
	if (size < 2) {
		return { 0 };
	}

	std::vector<int> max_z(size, 0);

	for (int i = 1, left = 0, right = 0; i < size; ++i) {
		if (i <= right) {
			max_z[i] = std::min(max_z[i - left], right - i + 1);
		}
		while (i + max_z[i] < size &&
			   str[max_z[i]] == str[max_z[i] + i]) {
			++max_z[i];
		}
		if (i + max_z[i] - 1 > right) {
			left = i;
			right = i + max_z[i] - 1;
		}
	}
	return max_z;
}


/*
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
*/


std::vector<int> kmp(const std::string& text,
	                 const std::string& pattern) {
	std::vector<int> matches;
	std::vector<int> prefixes = prefix_function(pattern);

	auto pattern_size = pattern.size();
	int pos = 0;
	for (int i = 0; i < text.size(); ++i) {
		while ((pos < pattern_size) ?
			(pos > 0) && (text[i] != pattern[pos]) : pos > 0) {
			pos = prefixes[pos - 1];
		}
		if (text[i] == pattern[pos]) {
			++pos;
		}
		if (pos == pattern_size) {
			matches.push_back(i - pos + 1);
		}
	}
	return matches;
}

std::vector<int> kmp_p(const std::string& text,
	                   const std::string& pattern) {
	std::vector<int> matches;
	const std::string str = pattern + "#" + text;
	auto pattern_size = pattern.size();
	std::vector<int> prefixes = prefix_function(str);
	for (int i = 0; i < prefixes.size(); ++i) {
		if (prefixes[i] == pattern_size) {
			matches.push_back(i - 2 * pattern_size);
		}
	}
	return matches;
}

std::vector<int> kmp_z(const std::string& text,
	                   const std::string& pattern) {
	std::vector<int> matches;
	const std::string str = pattern + "#" + text;
	auto pattern_size = pattern.size();
	std::vector<int> z = z_function(str);
	for (int i = 0; i < z.size(); ++i) {
		if (z[i] == pattern_size) {
			matches.push_back(i - pattern_size - 1);
		}
	}
	return matches;
}

int main() {

	int z[13] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

	prefix_func("Hello, world!", z);
	print(z, 0, 12);
	auto p = prefix_function("Hello, world!");
	print(p, 0, 12);
	std::cout << from_prefix(p) << "\n";

	auto zf = z_function("Hello, world!");
	print(zf, 0, 12);

	auto matches = kmp("abracadabra", "abra");
	print(matches, 0, matches.size() - 1);

	return 0;
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
		reverse<int, 10>(arr);
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