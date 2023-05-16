#import "header/wrapper.h"
#import "header/embedding.h"

@implementation MyCppClassWrapper {
  OpenAPI *_myCppClass;
}

- (instancetype)initWithapiKey:(NSString *)apiKey
                    tokenLimit:(int)tokenLimit
                embeddingModel:(NSString *)embeddingModel
                   promptModel:(NSString *)promptModel {
  self = [super init];
  if (self) {
    std::string api_key = [apiKey UTF8String];
    std::string embedding_model = [embeddingModel UTF8String];
    std::string prompt_model = [promptModel UTF8String];
    _myCppClass =
        new OpenAPI(api_key, tokenLimit, embedding_model, prompt_model);
  }
  return self;
}

- (void)storeEmbeddings:(NSString *)paragraph {
  std::string par = [paragraph UTF8String];

  _myCppClass->storeEmbeddings(par);
}
- (void)entryFunction:(NSString *)prompt {
  std::string promptString = [prompt UTF8String];

  _myCppClass->entryFunction(promptString);
}

@end

// create a header file