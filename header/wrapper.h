#import <Foundation/Foundation.h>

@interface MyCppClassWrapper : NSObject
- (instancetype)initWithapiKey:(NSString *)apiKey
                    tokenLimit:(int)tokenLimit
                embeddingModel:(NSString *)embeddingModel
                   promptModel:(NSString *)promptModel;
- (void)storeEmbeddings:(NSString *)paragraph;
- (void)entryFunction:(NSString *)prompt;
@end
