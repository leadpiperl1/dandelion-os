import os
import sys
import tinker

def unleash_tuttle():
    print("[*] Harry Tuttle dropping back into the ventilation system with a valid guest pass...")
    try:
        # Use the recommended RestClient and APIFuture pattern
        rest_client = tinker.RestClient()
        print("[+] RestClient initialized.")
        
        # Query server capabilities asynchronously / via future result
        print("[+] Querying server capabilities...")
        caps_future = rest_client.get_server_capabilities()
        caps = caps_future.result()
        print(f"    Server Capabilities: {caps}")
        
        # Verify identity using whoami
        print("[+] Verifying session identity (whoami)...")
        who_future = rest_client.whoami()
        user_info = who_future.result()
        print(f"    Session Identity: {user_info}")
        
        print("\n[+] Bulkheads cleared! Harry Tuttle is live inside the cluster.")
        
    except Exception as e:
        print(f"[-] Tuttle encountered a bulkhead error: {e}", file=sys.stderr)

if __name__ == "__main__":
    unleash_tuttle()
