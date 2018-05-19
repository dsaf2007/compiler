#ifndef TEXT_UTILS_CPP
#define TEXT_UTILS_CPP

#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
void split(const string &s, const char delim, T result) 
{
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim))
	{
        *(result++) = item;
    }
}


inline vector<string> split(const string &s, const char delim) 
{
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

inline string get_contained(const string& str, const char start, const char end) //{ }안의 내용을 가져온다.
{
    size_t first = str.find_first_of(start);
    size_t last = str.find_last_of(end);

    if(first == string::npos || last == string::npos)
        throw "Parse Error : All NFA arguments must be enclosed within {}";

    return str.substr(first+1, last-first-1);
}

inline string strip(string& str) //공백 제거
{
    for (int i = 0; i < str.length(); ++i) 
	{
        char current = str[i];
        if(current == ' ') 
		{
            str.erase(str.begin() + i);
            i--;
        }
    }

    return str;
}


template <typename T, typename V>
inline bool contains(T contain, V obj)//contain이 obj를 포함하는지 확인
{
    return std::find(contain.begin(), contain.end(), obj) != contain.end();
}

template <typename T>
inline const string to_string(T contain, char delim)//""+contain+delim+" "+contain+" " ...의 형태로 contain을 string화 시킨다.
{
    typename T::const_iterator pos;

    string out;

    string sep = "";
    string sep2; 
	sep2 += delim; 
	sep2 += ' ';

    for (pos= contain.begin(); pos!= contain.end(); ++pos)
	{
        out += sep + *pos;
        sep = sep2;
    }

    return out;
}

template <typename T>
inline const string to_string(T container) 
{
    return to_string(container, ' ');
}



#endif //TEXT_UTILS_CPP