#include "header/embedding.h"

void *initialize(const char *key, int token_limit, const char *embedding_model,
                 const char *propmpt_model) {
  return new OpenAPI(key, token_limit, embedding_model, propmpt_model);
}

void storeEmbeddings(void *object, char *paragraph) {
  OpenAPI *o = (OpenAPI *)object;
  o->storeEmbeddings(paragraph);
}

void entryFunction(void *object, char *prompt) {
  OpenAPI *o = (OpenAPI *)object;
  o->entryFunction(prompt);
}

// create a header file