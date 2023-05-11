
import Foundation

@objc class MyCppClassWrapper: NSObject {
    init(string1: String, string2: String, int1: Int, int2: Int) {
        _myCppClassWrapper = MyCppClassWrapper(string1: string1, string2: string2, int1: int1, int2: int2)
    }
    
    private let _myCppClassWrapper: MyCppClassWrapper
    
    func storeEmbeddings(string: String) {
        _myCppClassWrapper.storeEmbeddings(string)
    }
    
    func entryFunction(string: String) {
        _myCppClassWrapper.entryFunction(string)
    }
}