// Fuzz UriQuery.c:
//   uriDissectQueryMallocA
//   uriComposeQueryA

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::make_pair;
#include "uriparser/include/uriparser/Uri.h"
#include "uriparser/include/uriparser/UriBase.h"

std::vector<std::pair<string, string>> ToVector(UriQueryListA *query_list) {
  std::vector<std::pair<string, string>> result;
  if (query_list == nullptr) return result;
  for (UriQueryListA *entry = query_list; entry != nullptr;
       entry = entry->next) {
    // The value can be a nullptr.
    result.push_back(
        make_pair(entry->key, entry->value == nullptr ? "null" : entry->value));
  }
  uriFreeQueryListA(query_list);
  return result;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  const std::string query(reinterpret_cast<const char *>(data), size);

  UriQueryListA *query_list = nullptr;
  int item_count = -1;

  const char *query_start = query.c_str();
  const char *query_end = query_start + size;

  // Break a query like "a=b&2=3" into key/value pairs.
  int result =
      uriDissectQueryMallocA(&query_list, &item_count, query_start, query_end);
  
  
  

  if (query_list == nullptr || result != URI_SUCCESS) {
    return 0;
  }

  size_t max_size = 5000;
  std::vector<char> buf(max_size, 0);
  int written = -1;
  char *dest = &buf[0];
  // Reverse the process of uriDissectQueryMallocA.
  result = uriComposeQueryA(dest, query_list, max_size, &written);
  
  

  auto queries = ToVector(query_list);
  

  return 0;
}
