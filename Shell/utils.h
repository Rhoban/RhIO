#include <sstream>

// Trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// Trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// Trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

// Split a string
static inline std::vector<std::string> split(const std::string &s, char delim, int limit=-1) {
    if (limit > 0) limit--;
    std::vector<std::string> parts;

    std::stringstream ss(s);
    std::string item;
    while (limit!=0 && getline(ss, item, delim)) {
        parts.push_back(item);
        limit--;
    }
    unsigned int c;
    std::string end = "";
    while ((c = ss.get()) <= 256) {
        end += (char)c;
    }
    if (end != "") {
        parts.push_back(end);
    }
    return parts;
}
