#include "prompts.h"

const std::string Prompts::SUMMARY_PROMPT =
    "You are a researcher helper bot. Now you need to read the summaries of a "
    "research paper. Then you need to reply to ";

const std::string Prompts::READING_PROMPT =
    "You are a researcher helper bot. You can help the user with research "
    "paper reading and summarizing. Now I am going to send you a paper. Note "
    "that there might be a few spacing errors between words and mathematical "
    "errors. "
    "You "
    "need to read it and summarize it for me part by part. When you are "
    "reading, You need to focus on these key points: {}";

const std::string Prompts::BASE_POINTS =
    "1. Who are the authors? 2. What is the process of the proposed method? 3. "
    "What is the performance of the proposed method? Please note down its "
    "performance metrics. 4. What are the baseline models and their "
    "performances? Please note down these baseline methods. 5. What dataset "
    "did "
    "this paper use? ";

const std::string Prompts::SUMMARY_CHAT =
    "Now, I am sending you the chat history between the user and the AI in "
    "context of the research paper. First, here's a summary of older chats "
    "between the user and the AI";

const std::string Prompts::CHAT_PROMPTS =
    "Now, here's the more recent chat history between the user and the "
    "system.";

const std::string Prompts::UPDATED_PROMPT =
    "You are a researcher helper bot. You need to answer a question about a "
    "research paper. {} Here are the relevant parts/paragraphs of the paper "
    "necessary to answer the question. Note: They might be incomplete. {}. Try "
    "your best to only use these parts/paragraphs. Now, here's the question: "
    "{}";

const std::string Prompts::TITLE_PROMPT =
    "\"{}\". What is the title of the paper?";
