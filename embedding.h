#pragma once

#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;
using EmbeddingDataType = vector<pair<pair<string, vector<double>>, int>>;

class OpenAPI {

private:
  string api_key;
  const int TOKEN_LIMIT;
  EmbeddingDataType embedding_data;
  CURL *embeddingCurl;
  CURL *promptCurl;
  string embeddingModel;
  string promotModel;

  double findVectorSimilarity(vector<double> &a, vector<double> &b);
  void setupCurlHandle(CURL *curl, CurlType curlType);
  size_t writeFunction(void *ptr, size_t size, size_t nmemb, string *data);
  json &sendRequest(string paragraph, CurlType curlType);
  void sendEmbeddingRequest(string chunk, vector<double> &embedding);
  string sendPromptRequest(string prompt);
  size_t writeFunction(void *ptr, size_t size, size_t nmemb, string *data);
  string getParagraphs(vector<double> &embedding);
  setupCurlHandle(embeddingCurl, EMBED);
  setupCurlHandle(promptCurl, PROMPT);

public:
  enum CurlType { EMBED = 0, PROMPT = 1 };
  OpenAPI(string api_key, int token_limit = 1000,
          string embeddingModel = "text-davinci-002",
          string promotModel = "davinci");
  void storeEmbeddings(string &paragraph);
  string entryFunction(string &prompt);
};