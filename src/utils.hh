#ifndef UTILS_HH
#define UTILS_HH

#include <cctype>
#include <string>

// Returns the part of a string up to the first space.
inline std::string get_first_word(std::string s) {
    size_t n = s.find(" ");
    if (n == std::string::npos) {
        return s;
    }
    return s.substr(0, n);
}

// Returns the part of a string after (not including) the first space.
inline std::string get_tail(std::string s) {
    // TODO: deal with unexpected whitespace
    size_t n = s.find(" ");
    if (n == std::string::npos || n == s.length() - 1) {
        return "";
    }
    return s.substr(n + 1, s.length());
}

inline bool is_valid_filename(std::string name) {
    for (unsigned int i = 0; i < name.size(); ++i) {
        if(name[i] == ' ' && (i == 0 || name[i-1] != '\\')) {
            return false;
        }
    }
    return true;
}

inline std::string to_upper(std::string s) {
    std::string out = "";
    for (unsigned int i = 0; i < s.length(); ++i) {
        out += (char)toupper(s[i]);
    }
    return out;
}


#endif
