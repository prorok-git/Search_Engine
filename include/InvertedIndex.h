#pragma once

#include "ConverterJSON.h"
#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id, count;
    /* Данный оператор необходим для проведения тестовых сценариев */
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    InvertedIndex(const InvertedIndex& other) {
        this->docs.clear();
        this->docs.shrink_to_fit();
        this->docs = other.docs;

        this->freq_dictionary.clear();
        this->freq_dictionary = other.freq_dictionary;
    }

    InvertedIndex& operator=(const InvertedIndex& other) {
        if (this == &other)
            return *this;
        this->docs.clear();
        this->docs.shrink_to_fit();
        this->docs = other.docs;

        this->freq_dictionary.clear();
        this->freq_dictionary = other.freq_dictionary;
        return  *this;
    }

    void UpdateDocumentBase(const vector<string>&);
    vector<Entry> GetWordCount(const string&);
    static int GetWordCountInString(const string&, const string&);
    map<string, vector<Entry>> GetFreqDictionary ();
private:
    vector<string> docs; // список содержимого документов
    map<string, vector<Entry>> freq_dictionary; // частотный словарь
};