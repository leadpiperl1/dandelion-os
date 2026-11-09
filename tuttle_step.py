#!/usr/bin/env python3
import os
import sys
import tinker
from tml_renderers import chat, tokenizers, v0

def main():
    api_key = os.getenv("TINKER_API_KEY")
    if not api_key:
        print("[-] Error: TINKER_API_KEY environment variable not set.", file=sys.stderr)
        sys.exit(1)

    print("[*] Initializing Tinker ServiceClient for Inkling-Small...")
    client = tinker.ServiceClient(api_key=api_key)

    print("[*] Initializing tml-renderers O200kBaseChatTokenizer and Renderer...")
    renderer = v0.Renderer(tokenizers.O200kBaseChatTokenizer())

    messages = [
        {
            "role": "system", 
            "content": "You are an axiomatic assistant adhering to Landauer thermodynamic invariants and ternary logic."
        },
        {
            "role": "user", 
            "content": "Evaluate signal propagation across the boundary layer using ternary states (-1, 0, +1)."
        }
    ]

    print("[*] Converting OpenAI chat dicts and rendering for completion...")
    tml_messages = chat.OpenAIMessage.from_openai_chat_dicts(messages)
    spans, parser = renderer.render_for_completion(tml_messages)
    
    print("[+] Rendered prompt successfully generated.")
    print(f"[+] Spans count: {len(spans)}")
    print("[+] Tuttle step rendering complete.")

if __name__ == "__main__":
    main()
