import os
import sys

def test_tinker_connection():
    try:
        import tinker
        print(f"[+] Successfully loaded Tinker SDK version: {tinker.__version__}")
        
        client = tinker.Client()
        print("[+] Pinging Tinker remote worker endpoint...")
        response = client.complete(
            prompt="System integrity check: Report operational status and token overhead.",
            max_tokens=64
        )
        
        print("[+] Response received from Tinker backend:")
        print("-" * 50)
        print(response)
        print("-" * 50)
        
    except Exception as e:
        print(f"[-] API boundary caught an exception or authentication wall: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    test_tinker_connection()
