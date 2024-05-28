#include "include/ConverterJSON.h"

json ConverterJSON::GetDataFromJsonFile(const string& input_json_file_path) {
    try {
        json json_Dict;
        ifstream in_Json(input_json_file_path);
        if (!in_Json.is_open()) {
            cout << "File with path: " << "../" + input_json_file_path << " not found!" << endl;
            return json_Dict;
        }
        json_Dict = json::parse(in_Json);
        in_Json.close();
        return json_Dict;
    }
    catch (const json::parse_error &e) {
        cout << e.what() << endl;
    }
}

void ConverterJSON::putDicToJsonFile(const json &json_Dic, string &answers_Str) {
    ifstream in_Json("answers.json");
    if (!in_Json.is_open()) {
        cout << "File with path: /answers.json not found!" << endl << "Generated new answers.json file" << endl << endl;
    }
    ofstream out_Json_File("answers.json", ios::trunc);
    cout << answers_Str << endl;
    out_Json_File.clear();
    out_Json_File << json_Dic;
    out_Json_File.close();
}

vector<string> ConverterJSON::GetTextDocuments() {
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    json input_dict_list = GetDataFromJsonFile("config.json");
    vector<string> text_List_From_Config;
    for (const auto &it : input_dict_list["files"]) {
        ifstream in_File("" + string(it));
        if (!in_File.is_open()) {
            cout << "File with path: " << "../" + string(it) << " not found!" << endl;
            break;
        } else {
            string text;
            while (getline(in_File,text)) {
                in_File.close();
                text_List_From_Config.push_back(text);
            }
        }
    }
    return text_List_From_Config;
}

int ConverterJSON::GetResponsesLimit() {
    /**
    * Метод считывает поле max_responses для определения предельного количества ответов на один запрос
    * @return
    */
    json config_Json_File = GetDataFromJsonFile("config.json");
    return config_Json_File["config"]["max_responses"];
}

vector<string> ConverterJSON::GetRequests() {
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    vector<string> requests_List_From_Json;
    json requests_Json_Dict = GetDataFromJsonFile("requests.json");
    for (const auto &it : requests_Json_Dict["requests"]) {
        requests_List_From_Json.push_back(it);
    }
    return requests_List_From_Json;
}

void ConverterJSON::putAnswers(const vector<vector<pair<size_t, float>>> &answers) {
    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    string answers_Str = "{\n\t\"answers\":{\n\t\t";
    for (int i = 0; i < answers.size(); ++i) {
        float count = 0;
        for (const auto &val: answers[i] | views::values) {
            if (val > count) {
                count += val;
                break;
            }
        }
        if (count == 0) {
            answers_Str += "\"request00" + std::to_string(i + 1) + "\":{\n\t\t\t" + R"("result":"false")";
        } else {
            answers_Str += "\"request00" + std::to_string(i + 1) + "\":{\n\t\t\t" + R"("result":"true",)"
                + "\n\t\t\t\"relevance\":[\n\t\t\t\t";
            for (int j = 0; j < answers[i].size(); ++j) {
                answers_Str += "{\n\t\t\t\t\t\"docid\":" +
                    std::to_string(answers[i][j].first) + ",\"rank\":" +
                    std::to_string(answers[i][j].second) + "\n\t\t\t\t}";
                if (j < answers[i].size() - 1)
                    answers_Str += ",\n\t\t\t\t";
            }
            answers_Str += "\n\t\t\t]";
        }
        if (i < answers.size() - 1) {
            answers_Str += "\n\t\t},\n\t\t";
        } else
            answers_Str += "\n\t\t}";
    }
    answers_Str += "\n\t}\n}";

    putDicToJsonFile(json::parse(answers_Str), answers_Str);
}