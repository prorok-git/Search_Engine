#include "include/SearchServer.h"
#include <stack>

void SearchServer::multiThreadIndexing(const vector<string> &unique_words, vector<RelativeIndex> &request_relative_relevance_vector) {
    vector<thread> thrd_Vec;
    for (const auto& word : unique_words) {
        thrd_Vec.push_back(thread([&, word] {
            for (const auto& [fst, snd] : _index.GetFreqDictionary()) {
                if (word == fst) {
                    for (int i = 0; i < snd.size(); ++i) {
                        RelativeIndex idx;
                        idx.doc_id = snd[i].doc_id;
                        idx.rank = snd[i].count;
                        bool boolyan = false;
                        for (const auto& [doc_id, count] : request_relative_relevance_vector) {
                            if (doc_id == snd[i].doc_id) {
                                boolyan = true;
                            }
                        }
                        if (request_relative_relevance_vector.empty()) {
                            request_relative_relevance_vector.push_back(idx);
                        }
                        else {
                            for (auto& [doc_id, count] : request_relative_relevance_vector) {
                                if (doc_id == snd[i].doc_id) {
                                    count += snd[i].count;
                                }
                                else if (boolyan == false) {
                                    request_relative_relevance_vector.push_back(idx);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            }));
    }
    for (auto& thrd : thrd_Vec) if (thrd.joinable()) thrd.join();
}

void SearchServer::calcMaxAbsoluteRelevance(vector<RelativeIndex> &request_relative_relevance_vector) {
    int abs_max_relevance = 0;
    for (const auto& [doc_id, count] : request_relative_relevance_vector) {
        if (count > abs_max_relevance) {
            abs_max_relevance = count;
        }
    }
    calcRelativeRelevance(request_relative_relevance_vector, abs_max_relevance);
}

void SearchServer::calcRelativeRelevance(vector<RelativeIndex> &request_relative_relevance_vector, int abs_max_relevance) {
    for (auto& [doc_id, count] : request_relative_relevance_vector) {
        if (abs_max_relevance != 0) {
            count /= abs_max_relevance;
        }
    }
}

void SearchServer::quickSort(vector<RelativeIndex> &request_relative_relevance_vector) {

    if (!request_relative_relevance_vector.empty()) {
        int i, j, pivot;
        int left = 0;
        int right = request_relative_relevance_vector.size() - 1;
        stack<int> stk;
        stk.push(left);
        stk.push(right);
        do {
            right = stk.top();
            stk.pop();
            left = stk.top();
            stk.pop();
            i = left;
            j = right;
            pivot = (i + j) / 2;

            do {
                while (request_relative_relevance_vector[i].rank >= request_relative_relevance_vector[pivot].rank &&
                    request_relative_relevance_vector[i].doc_id < request_relative_relevance_vector[pivot].doc_id && i < request_relative_relevance_vector.size() - 1) {
                    i++;
                }
                while (request_relative_relevance_vector[j].rank <= request_relative_relevance_vector[pivot].rank &&
                    request_relative_relevance_vector[i].doc_id > request_relative_relevance_vector[pivot].doc_id) {
                    j--;
                }
                if (i <= j) {
                    RelativeIndex t = request_relative_relevance_vector[i];
                    request_relative_relevance_vector[i] = request_relative_relevance_vector[j];
                    request_relative_relevance_vector[j] = t;
                    i++;
                    j--;
                }
            } while (i <= j);
            if (left < j) {
                stk.push(left);
                stk.push(j);
            }
            if (i < right) {
                stk.push(i);
                stk.push(right);
            }
        } while (!stk.empty());
    }

}

vector<pair<size_t, float>> SearchServer::collapseAndFillRelativeRelevanceVector(vector<RelativeIndex> &request_relative_relevance_vector, vector<pair<size_t, float>> &request_answer) {
    if (request_relative_relevance_vector.size() > ConverterJSON::GetResponsesLimit()) {
        auto first = request_relative_relevance_vector.cbegin() + ConverterJSON::GetResponsesLimit();
        auto last = request_relative_relevance_vector.cend();
        request_relative_relevance_vector.erase(first, last);
    }

    for (const auto& [doc_id, rank] : request_relative_relevance_vector) {
        request_answer.emplace_back(doc_id, rank);
    }
    return request_answer;
}

vector<vector<pair<size_t, float>>> SearchServer::search(const vector<string> &queries_input) {
    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла requests.json
    * @return возвращает отсортированный список релевантных ответов для заданных запросов
    */
   vector<vector<pair<size_t, float>>> relative_relevance_vector; // вектор векторов относительной релевантности
   vector<string> unique_words; // список слов из requests
   vector<RelativeIndex> request_relative_relevance_vector; // вектор относительной релевантности request
   vector<pair<size_t, float>> request_answer;

   if (!queries_input.empty()) {
    for (int m = 0; m < queries_input.size(); ++m) {
        vector<string> temp_String;
        temp_String.push_back(queries_input[m]);
        
        for (const auto& word : temp_String) {
            istringstream ist(word);
            string tmp;
            while (ist >> tmp) {
                unique_words.push_back(tmp);
            }
        }

        multiThreadIndexing(unique_words, request_relative_relevance_vector);

        calcMaxAbsoluteRelevance(request_relative_relevance_vector);

        quickSort(request_relative_relevance_vector);

        relative_relevance_vector.push_back(collapseAndFillRelativeRelevanceVector(request_relative_relevance_vector, request_answer));

        request_answer.clear();
        request_relative_relevance_vector.clear();
        unique_words.clear();
    }

    ConverterJSON::putAnswers(relative_relevance_vector);
} else {
        cout << "queries_input cant be empty;" << endl;
}
return relative_relevance_vector;
}