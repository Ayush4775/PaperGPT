#import "header/wrapper.h"
#import "header/embedding.h"

@implementation MyCppClassWrapper {
  OpenAPI _myCppClass;
}

- (instancetype)initWithapiKey:(NSString *)apiKey
                    tokenLimit:(int)tokenLimit
                embeddingModel:(NSString *)embeddingModel
                   promptModel:(NSString *)promptModel {
  self = [super init];
  if (self) {
    std::string api_key = [apiKey UTF8String];
    std::string embedding_model = [embeddingModel UTF8String];
    std::string propmpt_model = [promptModel UTF8String];
    _myCppClass = OpenAPI(api_key, tokenLimit, embeddingModel, promptModel);
  }
  return self;
}

- (void)storeEmbeddings:(NSString *)paragraph {
  std::string paragraph = [paragraph UTF8String];

  _myCppClass.storeEmbeddings(paragraph);
}
- (void)entryFunction:(NSString *)prompt {
  std::string prompt = [prompt UTF8String];

  _myCppClass.entryFunction(prompt);
}

@end

// create a header file