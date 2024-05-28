#include "include/InvertedIndex.h"
#include <mutex>

mutex mtx;

void InvertedIndex::UpdateDocumentBase(const vector<string> &input_docs) {
    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    for (const auto &doc : input_docs) {
        docs.push_back(doc);
    }

    for (const auto &word : docs) {
        istringstream ist(word);
        string tmp;
        while (ist >> tmp) {
            freq_dictionary.insert(make_pair(tmp,GetWordCount(tmp)));
        }
    }
}

int InvertedIndex::GetWordCountInString(const string &text, const string &word) {
    int count = 0;
    stringstream  stream(text);
    string buffer;
    const string &search_substr = word;
    while(stream >> buffer) {
        if (search_substr == buffer) count++;
    }
    return count;
}

vector<Entry> InvertedIndex::GetWordCount(const string &word) {
    /**
    * Метод определяет количество вхождений слова word в загруженной базе документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    lock_guard<mutex> lg(mtx);
    vector<Entry> temp_Entry_Vector;
    if (!word.empty()) {
        if (const auto &it = freq_dictionary.find(word); it != freq_dictionary.end()) {
            return it->second;
        } else {
            for (int i = 0; i < this->docs.size(); ++i) {
                if (const int counter = GetWordCountInString(docs[i], word)) {
                    Entry entry{};
                    entry.doc_id = i;
                    entry.count = counter;
                    temp_Entry_Vector.push_back(entry);
                }
            }
        }
    } else {
        cout << "Word empty!" << std::endl;
    }
    return temp_Entry_Vector;
}

map<string, vector<Entry>> InvertedIndex::GetFreqDictionary() {
    return freq_dictionary;
}