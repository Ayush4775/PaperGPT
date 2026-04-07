
import Foundation

/// Swift-facing wrapper around the Objective-C++ MyCppClassWrapper bridge.
/// Requires a bridging header that imports "wrapper.h".
class PaperGPTWrapper {
    private let wrapper: MyCppClassWrapper // ObjC++ class visible via bridging header

    init(apiKey: String, tokenLimit: Int, embeddingModel: String, promptModel: String) {
        wrapper = MyCppClassWrapper(
            apiKey: apiKey,
            tokenLimit: Int32(tokenLimit),
            embeddingModel: embeddingModel,
            promptModel: promptModel
        )
    }

    /// Chunk and embed the paper text for later similarity search.
    func storeEmbeddings(_ text: String) {
        wrapper.storeEmbeddings(text)
    }

    /// Ask a question about the stored paper and return the AI-generated answer.
    func ask(_ prompt: String) -> String {
        return wrapper.entryFunction(prompt)
    }
}