#include "header/embedding.h"
#include <fmt/core.h>
OpenAPI::OpenAPI(string api_key, int token_limit, string embeddingModel,
                 string promotModel)
    : api_key(api_key), TOKEN_LIMIT(token_limit),
      embeddingModel(embeddingModel), promotModel(promotModel) {
  curl_global_init(CURL_GLOBAL_ALL);

  setupCurlHandle(embeddingCurl, EMBED);
  setupCurlHandle(promptCurl, PROMPT);
};

void OpenAPI::setupCurlHandle(CURL *curl, CurlType curlType) {
  curl = curl_easy_init();
  string url;
  if (curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers,
                                ("Authorization: Bearer " + api_key).c_str());

    if (curlType == EMBED) {
      url = "https://api.openai.com/v1/embeddings";
    } else {
      url = "https://api.openai.com/v1/engines/" + promotModel + "/completions";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &OpenAPI::writeFunction);
  } else {
  } // TODO: Handle error
}
void OpenAPI::sendRequest(json &response_json, string paragraph,
                          CurlType curlType) {
  string postFields, responseString;
  CURLcode res;

  if (curlType == CurlType::EMBED) {
    postFields = "{\"input\":\"" + paragraph + "\",\"model\":\"" +
                 embeddingModel + "\"}";
    curl_easy_setopt(embeddingCurl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(embeddingCurl, CURLOPT_WRITEDATA, &responseString);
    res = curl_easy_perform(embeddingCurl);
  } else {
    postFields = "{\"prompt\":\"" + paragraph + "\",\"max_tokens\":2000}";
    curl_easy_setopt(promptCurl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(promptCurl, CURLOPT_WRITEDATA, &responseString);
    res = curl_easy_perform(promptCurl);
  }

  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  response_json = json::parse(responseString);
}

void OpenAPI::sendEmbeddingRequest(string chunk, vector<double> &embedding) {
  json response_json;
  sendRequest(response_json, chunk, EMBED);
  embedding = response_json["data"][0]["embedding"].get<std::vector<double>>();
}
string OpenAPI::sendPromptRequest(string prompt) {
  json response_json;
  sendRequest(response_json, prompt, PROMPT);
  return response_json["choices"][0]["text"];
}

// write a function to find similarity between two vector embeddings
// using cosine similarity
double OpenAPI::findVectorSimilarity(vector<double> &embedding1,
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

void OpenAPI::storeEmbeddings(string &paragraph) {
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
      embedding_data.push_back(
          make_pair((make_pair(chunk, std::move(embedding))), count));
      count = 0;
      chunk = "";
    }
  }
  if (chunk != "") {
    vector<double> embedding;
    sendEmbeddingRequest(chunk, embedding);
    embedding_data.push_back(
        make_pair(make_pair(chunk, std::move(embedding)), count));
  }
}

size_t OpenAPI::writeFunction(void *ptr, size_t size, size_t nmemb,
                              string *data) {
  data->append((char *)ptr, size * nmemb);
  return size * nmemb;
}

string OpenAPI::getParagraphs(vector<double> &embedding) {

  // sorting embeddings based on the given embedding
  sort(embedding_data.begin(), embedding_data.end(), [&](auto &a, auto &b) {
    return findVectorSimilarity(a.first.second, embedding) >
           findVectorSimilarity(b.first.second, embedding);
  });

  // getting the first paragraphs till the token limit is reached
  int count = 0;
  int i = 0;
  string paragraphs = "";
  string cds = "sas";
  while (count < 2 * TOKEN_LIMIT * 0.75) {
    paragraphs += embedding_data[i].first.first + '\n';
    count += embedding_data[i].second;
  }
  return paragraphs;
}

string OpenAPI::entryFunction(string &prompt) {
  vector<double> embedding;
  sendEmbeddingRequest(prompt, embedding);
  string paragraphs = getParagraphs(embedding);

  string updated_prompt =
      fmt::format(Prompts::UPDATED_PROMPT, ".", paragraphs, prompt);
  string response = sendPromptRequest(updated_prompt);
  return response;
}