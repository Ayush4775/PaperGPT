#include <curl/curl.h>
#include <iostream>
#include <json/json.h>
#include <string>
#include <vector>

std::string data;

size_t writeCallback(char *buf, size_t size, size_t nmemb, void *up) {
  for (int c = 0; c < size * nmemb; c++)
    data.push_back(buf[c]);
  return size * nmemb;
}

int main() {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  std::vector<std::string> history;

  curl = curl_easy_init();
  if (curl) {
    std::string url = "https://api.openai.com/v1/engines/davinci/completions";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers =
        curl_slist_append(headers, "Authorization: Bearer YOUR_API_KEY_HERE");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    int n = 0;
    while (true) {
      // concatenate history
      std::string historyStr;
      for (const auto &str : history) {
        historyStr += str + "\n";
      }

      // send new prompt
      std::string prompt = "YOUR_PROMPT_HERE";
      history.push_back(prompt);
      std::string postData =
          "{\"prompt\": \"" + historyStr + prompt + "\", \"max_tokens\": 100}";
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

      res = curl_easy_perform(curl);
      if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

      // parse response
      Json::Value jsonData;
      Json::Reader jsonReader;
      if (jsonReader.parse(data, jsonData)) {
        n += jsonData["n"].asInt();
        std::cout << "Number of tokens used: " << n << std::endl;
        std::string completion = jsonData["choices"][0]["text"].asString();
        history.push_back(completion);

        // check if token limit has been reached
        if (n >= 4000) {
          // send request to generate summary
          int tokensToSummarize = n - 3500;
          prompt = "summarize the first " + std::to_string(tokensToSummarize) +
                   " tokens: " + historyStr;
          postData = "{\"prompt\": \"" + prompt + "\", \"max_tokens\": 100}";
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
          res = curl_easy_perform(curl);
          if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
          if (jsonReader.parse(data, jsonData)) {
            n += jsonData["n"].asInt();
            std::cout << "Number of tokens used: " << n << std::endl;
            std::string summary = jsonData["choices"][0]["text"].asString();
            std::cout << summary << std::endl;

            // update history with summary
            history.clear();
            history.push_back(summary);
          }
        }
      }
    }

    curl_easy_cleanup(curl);
  }
  return 0;
}

// g++  -L /opt/local/lib -lpodofo read_pdf.cpp