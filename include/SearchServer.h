#pragma once

#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include <thread>

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    explicit SearchServer(const InvertedIndex& idx) : _index(idx) {
        /**
        * @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
        * чтобы SearchServer мог узнать частоту слов встречаемых в запросе
        */
    };

    vector<vector<pair<size_t, float>>> search(const vector<string>&);
    void multiThreadIndexing(const vector<string>&, vector<RelativeIndex>&);
    void calcMaxAbsoluteRelevance(vector<RelativeIndex>&);
    void calcRelativeRelevance(vector<RelativeIndex>&, int);
    void quickSort(vector<RelativeIndex>&);
    vector<pair<size_t, float>> collapseAndFillRelativeRelevanceVector(vector<RelativeIndex>&, vector<pair<size_t, float>>&);
private:
    InvertedIndex _index;
};