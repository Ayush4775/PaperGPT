# PaperGPT

A C++ backend for an AI-powered research paper reader and Q&A assistant. PaperGPT ingests a research paper's text, generates vector embeddings via the OpenAI API, and uses semantic similarity search to answer user questions grounded in the paper's content.

## How It Works

1. **Ingest** — The user provides raw paper text. The text is chunked into segments (≈75 % of the token limit) and each chunk is sent to the OpenAI Embeddings API. The resulting embedding vectors are stored alongside their source text.
2. **Query** — When the user asks a question, the question itself is embedded. Stored chunks are ranked by cosine similarity to the question embedding, and the most relevant chunks are assembled into a context window.
3. **Answer** — A prompt is constructed from a template (see `Prompts::UPDATED_PROMPT`) that combines the retrieved context and the user question, then sent to the OpenAI Completions API. The model's response is returned to the caller.

## Architecture

| File | Role |
|---|---|
| `embedding.cpp / embedding.h` | Core `OpenAPI` class — networking, embedding storage, similarity search, prompt dispatch |
| `prompts.cpp / prompts.h` | Prompt templates stored as compile-time constants in a `Prompts` namespace |
| `wrapper.mm / wrapper.h` | Objective-C++ bridge that wraps the C++ `OpenAPI` class for use from Swift / Apple frameworks |
| `swiftCode.swift` | Thin Swift wrapper around the Objective-C++ bridge |
| `main.cpp` | Minimal entry point for standalone testing |
| `CMakeLists.txt` | Build configuration (CMake) |

## C++ Techniques & Features

### C++20 Standard
The project targets **C++20** (`CMAKE_CXX_STANDARD 20`), enabling modern language features throughout the codebase.

### Object-Oriented Design
All API interaction logic is encapsulated in a single `OpenAPI` class with clear public/private boundaries.  
- **Constructor with default arguments** — `OpenAPI(string api_key, int token_limit = 1000, string embeddingModel = "text-davinci-002", string promotModel = "davinci")` lets callers override only the parameters they need.  
- **Member initializer lists** — the constructor uses an initializer list (`: api_key(api_key), TOKEN_LIMIT(token_limit), …`) for efficient, direct member initialization.

### Enumerations for Type Safety
A private `enum CurlType { EMBED, PROMPT }` distinguishes request types at compile time, avoiding error-prone magic numbers or strings.

### STL Algorithms & Containers
- **`std::vector`** as the primary container for embedding data.
- **`std::inner_product`** (from `<numeric>`) computes dot products and magnitudes for cosine similarity — a concise, iterator-based numeric algorithm.
- **`std::sort` with a lambda comparator** ranks chunks by similarity in-place:
  ```cpp
  sort(embedding_data.begin(), embedding_data.end(), [&](auto &a, auto &b) {
      return findVectorSimilarity(a.first.second, embedding) >
             findVectorSimilarity(b.first.second, embedding);
  });
  ```
- **`std::pair` and nested pairs** are used as lightweight data tuples (`pair<pair<string, vector<double>>, int>`) with a readable type alias (`EmbeddingDataType`).
- **`std::stringstream`** tokenises input text word-by-word for chunking.
- **`std::move`** is applied to embedding vectors during storage to avoid unnecessary copies of large data.

### Type Aliases
```cpp
using json = nlohmann::json;
using EmbeddingDataType = vector<pair<pair<string, vector<double>>, int>>;
```
Aliases keep complex template types readable and maintainable.

### Lambda Expressions
Lambdas with capture-by-reference (`[&]`) are used inside `std::sort` to provide inline, context-aware comparison logic.

### Namespaces for Prompt Management
Prompt templates live in the `Prompts` namespace with `extern const std::string` declarations in the header and definitions in the source file — a clean separation of data from logic.

### Static Callback (libcurl Integration)
`OpenAPI::writeFunction` serves as a C-style callback for libcurl's `CURLOPT_WRITEFUNCTION`, bridging C and C++ idioms by casting `void*` data into a `std::string*`.

### Objective-C++ Interoperability
`wrapper.mm` is an **Objective-C++** file that bridges the C++ `OpenAPI` class to Apple's Objective-C runtime. It:
- Wraps a raw C++ pointer (`OpenAPI *_myCppClass`) inside an `NSObject` subclass.
- Converts `NSString *` to `std::string` (via `UTF8String`) at the boundary.
- Enables downstream consumption from **Swift** through a standard Objective-C interface.

## Build

```bash
mkdir -p build && cd build
cmake ..
make
```

### Dependencies

| Library | Purpose |
|---|---|
| [libcurl](https://curl.se/libcurl/) | HTTP requests to the OpenAI API |
| [nlohmann/json](https://github.com/nlohmann/json) | JSON parsing and serialisation |
| [fmt](https://github.com/fmtlib/fmt) | String formatting for prompt construction |

All three are resolved via CMake's `find_package`.
