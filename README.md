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
