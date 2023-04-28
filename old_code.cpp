#include "embedding.h"
#include "prompts.h"
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
  void storeEmbeddings(string &paragraph);
  size_t writeFunction(void *ptr, size_t size, size_t nmemb, string *data);
  string getParagraphs(vector<double> &embedding);
  string entryFunction(string &prompt);

public:
  enum CurlType { EMBED = 0, PROMPT = 1 };
  OpenAPI(string api_key, int token_limit = 1000,
          string embeddingModel = "text-davinci-002",
          string promotModel = "davinci")
      : api_key(api_key), TOKEN_LIMIT(token_limit),
        embeddingModel(embeddingModel), promotModel(promotModel) {
    curl_global_init(CURL_GLOBAL_ALL);

    setupCurlHandle(embeddingCurl, EMBED);
    setupCurlHandle(promptCurl, PROMPT);
  };

  void setupCurlHandle(CURL *curl, CurlType curlType) {
    curl = curl_easy_init();
    if (curl) {
      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers,
                                  ("Authorization: Bearer " + api_key).c_str());

      if (curlType == EMBED) {
        string url = "https://api.openai.com/v1/embeddings";
      } else {
        string url =
            "https://api.openai.com/v1/engines/" + promotModel + "/completions";
      }

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    } else {
    } // TODO: Handle error
  }
  json &sendRequest(string paragraph, CurlType curlType) {
    string postFields, responseString;

    if (CurlType == EMBED) {
      postFields = "{\"input\":\"" + paragraph + "\",\"model\":\"" +
                   embeddingModel + "\"}";
    } else {
      postFields = "{\"prompt\":\"" + prompt + "\",\"max_tokens\":2000}";
    }
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    json response_json = json::parse(response_string);
    return response_json;
  }

  void sendEmbeddingRequest(string chunk, vector<double> &embedding) {
    json &response_json = sendRequest(chunk, EMBED);
    embedding = response_json["data"][0]["embedding"];
  }
  string sendPromptRequest(string prompt) {
    json &response_json = sendRequest(prompt, PROMPT);
    return response_json["choices"][0]["text"];
  }

  // write a function to find similarity between two vector embeddings
  // using cosine similarity
  double findVectorSimilarity(vector<double> &embedding1,
                              vector<double> &embedding2) {
    double mag1 = (inner_product(embedding1.begin(), embedding1.end(),
                                 embedding1.begin(), 0.0));
    double mag2 = (inner_product(embedding2.begin(), embedding2.end(),
                                 embedding2.begin(), 0.0));
    double dot = (inner_product(embedding1.begin(), embedding1.end(),
                                embedding2.begin(), 0.0));
    return dot / (mag1 * mag2);
  }
  // write a function to store embeddings in a vector

  void storeEmbeddings(string &paragraph) {
    stringstream ss(paragraph);
    string word;
    string chunk = "";
    int count = 0;
    while (ss >> word) {
      chunk += word + " ";
      count++;
      if (count >= TOKEN_LIMIT * 0.75) {
        vector<double> embedding;
        sendEmbeddingRequest(chunk, embedding);
        data.push_back(
            make_pair((make_pair(chunk, std::move(embedding))), count));
        count = 0;
        chunk = "";
      }
    }
    if (chunk != "") {
      vector<double> embedding;
      sendEmbeddingRequest(chunk, embedding);
      data.push_back(make_pair((make_pair(chunk, std::move(embedding)), count));
    }
  }

  size_t writeFunction(void *ptr, size_t size, size_t nmemb, string *data) {
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
  }

  string getParagraphs(vector<double> &embedding) {

    // sorting embeddings based on the given embedding
    sort(embedding_data.begin(), embedding_data.end(), [](auto &a, auto &b) {
      findVectorSimilarity(a.first.second, embedding) >
          findVectorSimilarity(b.first.second, embedding);
    });

    // getting the first paragraphs till the token limit is reached
    int count = 0;
    int i = 0;
    string paragraphs = "";
    while (count < 2 * TOKEN_LIMIT * 0.75) {
      paragraphs + = embedding_data[i].first.first + '\n';
      count += embedding_data[i].second;
    }
    return paragraphs;
  }

  string entryFunction(string &prompt) {
    vector<double> embedding;
    api.sendEmbeddingRequest(prompt, embedding);
    string paragraphs = api.getParagraphs(embedding);

    string updated_prompt =
        format(Prompts::UPDATED_PROMPT, "", pargraphs, prompt);
    string response = api.sendPromptRequest(updated_prompt);
    return response;
  }
};
int main() {

  string paragraph = "This is an example paragraph for which we want to "
                     "create a vector embedding using the OpenAI API.";
  string model =
      "text-embedding-ada-002"; // OpenAI's best embeddings as of Apr 2023

  // Initialize the OpenAI API with your API key
  OpenAPI api("YOUR_API_KEY_HERE");
  stringstream ss(paragraph);
  EmbeddingDataType data;
  api.storeEmbeddings(paragraph);

  // Create the vector embedding for the paragraph

  // Print the embedding
  cout << "Embedding: [";
  for (size_t i = 0; i < embedding.size(); i++) {
    cout << embedding[i];
    if (i != embedding.size() - 1) {
      cout << ", ";
    }
  }
  cout << "]" << endl;

  string prompt;
  cout << "Enter a prompt: ";

  cin >> prompt;
  return api.entryFunction(prompt);
}