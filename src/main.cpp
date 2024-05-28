#include "include/SearchServer.h"

int main () {
    InvertedIndex inverted_index;
    inverted_index.UpdateDocumentBase(ConverterJSON::GetTextDocuments());
    SearchServer search_server(inverted_index);
    const vector<vector<pair<size_t, float>>> result = search_server.search(ConverterJSON::GetRequests());
}