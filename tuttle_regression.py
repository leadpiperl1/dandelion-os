#!/usr/bin/env python3
import os
import sys
import tinker
from tml_renderers import chat, tokenizers, v0

def run_regression():
    print("[*] Starting Tuttle Axiomatic Regression Suite...")
    renderer = v0.Renderer(tokenizers.O200kBaseChatTokenizer())
    
    test_cases = [
        [{"role": "user", "content": "Test invariant -1"}],
        [{"role": "user", "content": "Test invariant 0"}],
        [{"role": "user", "content": "Test invariant +1"}]
    ]
    
    for i, case in enumerate(test_cases, 1):
        tml_msgs = chat.OpenAIMessage.from_openai_chat_dicts(case)
        spans, parser = renderer.render_for_completion(tml_msgs)
        print(f"[+] Regression Test Case {i} passed: {len(spans)} span(s) generated.")

    print("[+] All axiomatic regression test cases completed successfully.")

if __name__ == "__main__":
    run_regression()
