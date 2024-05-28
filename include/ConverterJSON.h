#pragma once

#include <iostream>
#include <vector>

//#define JSON_TRY_USER if(true)
//#define JSON_CATCH_USER(exception) if(false)
//#define JSON_THROW_USER(exception)                           \
//    {std::clog << "Error in " << __FILE__ << ":" << __LINE__ \
//               << " (function " << __FUNCTION__ << ") - "    \
//               << (exception).what() << std::endl;           \
//     std::abort();}
//
//# define JSON_DIAGNOSTICS 1

#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

using namespace std;
using json = nlohmann::json;

    /** Класс для работы с JSON-файлами */
class ConverterJSON {
public:
    ConverterJSON() = default;

    static vector<string> GetTextDocuments();
    static json GetDataFromJsonFile(const string&);
    static void putDicToJsonFile(const json&, string&);
    static int GetResponsesLimit();
    static vector<string> GetRequests();
    static void putAnswers(const vector<vector<pair<size_t, float>>>&);
};