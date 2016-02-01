//  This application was created in order to test the code exposed in :
//  http://tcbrindle.github.io/posts/a-quicker-study-on-tokenising/
//
//  I did not try the last solution because of it is only compatible with c++14
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

static bool isDelim1(char tst);
vector<string> DoJoshsWay(std::string& str);

bool is_delimiter(char value, const string& delims);
vector<string> split1(string str, string delims);
vector<string> split2(const string & str, const string  &delims);
vector<string> split3(const string & str, const string  &delims);
vector<string> split4(const string& str, const string& delims);
vector<string> split5(const string& str, const string& delims);

vector<string> split6(const string& str, const string& delims = " ", bool skip_empty = true);
 
int main (int argc, char **argv) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " file.csv" << endl;
	}

	ifstream csv(argv[1]);
	if (csv.is_open() == false) {
		cerr << "Error opening input file" << endl;
	}
	stringstream str;
	str << csv.rdbuf();
	string text (str.str());
	cout << "Text size: " << text.size() << endl;

	auto start = chrono::steady_clock::now();
	auto tokens1 = DoJoshsWay(text);
	cout << "Tokens1: " << tokens1.size() << endl;
	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

//	start = chrono::steady_clock::now();
//	auto tokens2 = split1(text, ";");
//	cout << "Tokens2: " << tokens1.size() << endl;
//	end = chrono::steady_clock::now();
//	diff = end - start;
//	cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

	start = chrono::steady_clock::now();
	auto tokens2 = split2(text, ";");
	cout << "version2: " << tokens2.size() << endl;
	end = chrono::steady_clock::now();
	diff = end - start;
	cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

	start = chrono::steady_clock::now();
	auto tokens3 = split3(text, ";");
	cout << "version3: " << tokens3.size() << endl;
	end = chrono::steady_clock::now();
	diff = end - start;
	cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

	start = chrono::steady_clock::now();
	auto tokens4 = split4(text, ";");
	cout << "version4: " << tokens4.size() << endl;
	end = chrono::steady_clock::now();
	diff = end - start;
	cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

	start = chrono::steady_clock::now();
	auto tokens5 = split5(text, ";");
	cout << "version5: " << tokens5.size() << endl;
	end = chrono::steady_clock::now();
	diff = end - start;
	cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

	return EXIT_SUCCESS;
}

static bool isDelim1(char tst) {
    const char* DELIMS = ";";
    do // Delimiter string cannot be empty, so don't check for it
    {
        if( tst == *DELIMS )
            return true;
        ++DELIMS;
    } while( *DELIMS );

    return false;
}

vector<string> DoJoshsWay(std::string& str)
{
	vector<string> tokens;
    char* pMutableString = (char*) malloc( str.size()+1 );
    strcpy( pMutableString, str.c_str() );

    char* p = pMutableString;

    // skip leading delimiters
    while( *p && isDelim1(*p) )
        ++p;

    while( *p )
    {
        // note start of token
        char* pTok = p;

        do// skip non-delimiters
        {
            ++p;
        } while( !isDelim1(*p) && *p );

        // clobber trailing delimiter with null
        *p = 0;
        tokens.push_back(pTok); // send the token

        do // skip null, and any subsequent trailing delimiters
        {
            ++p;
        } while( *p && isDelim1(*p) );
    }

    free(pMutableString);
	return tokens;
}

// ------------------------------------------------------------------------
// Version 1
bool is_delimiter(char value, const string& delims)
{
       for (auto d : delims) {
           if (d == value)
			   return true;
       }
       return false;
}

vector<string> split1(string str, string delims)
{
    vector<string> output;

    while (str.size() > 0) {
        if (is_delimiter(str[0], delims)) {
            output.push_back("");
            str = str.substr(1);
        } else {
            size_t i = 1;
            while (i < str.size() &&
                   !is_delimiter(str[i], delims))  {
                i++;
            }
            output.emplace_back(str.begin(), str.begin() + i);
            if (i + 1 < str.size()) {
                str =  str.substr(i + 1);
            } else {
                str = "";
            }
        }
    }

    return output;
}

vector<string> split2(const string& str, const string& delims)
{
    vector<string> output;
    int first = 0;

    while (str.size() - first > 0) {
        if (is_delimiter(str[first], delims)) {
            output.push_back("");
            ++first;
        } else {
            size_t second = first + 1;
            while (second < str.size() &&
                   !is_delimiter(str[second], delims))  {
                ++second;
            }
            output.emplace_back(str.begin() + first, str.begin() + second);
            if (second == str.size()) {
                break;
            }
            first =  second + 1;
        }
    }

    return output;
}

vector<string> split3(const string& str, const string& delims)
{
    vector<string> output;
    size_t first = 0;

    while (first < str.size()) {
        size_t second = first;
        while (second < str.size() &&
               !is_delimiter(str[second], delims))  {
            ++second;
        }
        output.emplace_back(str.begin() + first, str.begin() + second);
        if (second == str.size()) {
            break;
        }
        first =  second + 1;
    }

    return output;
}

vector<string> split4(const string& str, const string& delims)
{
    vector<string> output;
    auto first = begin(str);

    while (first != end(str)) {
        auto second = first;
        while (second != end(str) &&
               !is_delimiter(*second, delims))  {
            ++second;
        }
        output.emplace_back(first, second);
        if (second == end(str)) {
            break;
        }
        first =  next(second);
    }

    return output;
}

vector<string> split5(const string& str, const string& delims)
{
    vector<string> output;
    auto first = begin(str);

    while (first != end(str)) {
        const auto second = find_first_of(first, end(str), begin(delims), end(delims));
        output.emplace_back(first, second);
        if (second == end(str)) 
			break;
        first =  next(second);
    }

    return output;
}
