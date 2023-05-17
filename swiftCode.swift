
import Foundation

@objc class MyCppClassWrapper: NSObject {
    init(string1: String, int1: Int, string2: String, string3: String) {
        _myCppClassWrapper = MyCppClassWrapper(string1: string1, int1: Int, string2: String, string3: String) // api key, tokens, embedding model, prompt model
    }
    
    private let _myCppClassWrapper: MyCppClassWrapper
    
    func storeEmbeddings(string: String) {
        _myCppClassWrapper.storeEmbeddings(string)
    }
    
    func entryFunction(string: String) {
        _myCppClassWrapper.entryFunction(string)
    }
}