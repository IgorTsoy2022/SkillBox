#include <iostream>
#include <sstream>
#include <cpr/cpr.h>

static std::string find_tag_string(std::string tag, std::string from) {
	auto begin = from.find("<" + tag + ">");
	if (begin == std::string::npos) {
		return "";
	}
	begin += tag.length() + 2;
	auto end = from.find("</" + tag + ">", begin);
	if (end == std::string::npos) {
		return "";
	}
	return from.substr(begin, end - begin);
}

int main() {

	cpr::Response r = cpr::Get(cpr::Url("http:/httpbin.org/html"),
		                       cpr::Header({ {"user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36"},
					                         {"Accept", "text/html"} }));
	std::cout << find_tag_string("h1", r.text) << std::endl;

	return 0;
}
