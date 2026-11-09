import asyncio
import sys
import tinker

async def test_inkling_training_async():
    print("[*] Harry Tuttle spinning up Inkling training client with 'base_model'...")
    service_client = tinker.ServiceClient()
    
    try:
        model_id = "thinkingmachines/Inkling-Small"
        print(f"[+] Spawning training client for: {model_id}")
        
        # Use the exact parameter name revealed by introspection: base_model
        training_client = await service_client.create_lora_training_client(
            base_model=model_id,
            rank=8
        )
        print(f"[+] Success! Training client spawned: {training_client}")
        
        # Fetch run info
        info = await training_client.get_info_async()
        print(f"[+] Training Run Info: {info}")
        
    except Exception as e:
        print(f"[-] Caught exception: {e}", file=sys.stderr)

if __name__ == "__main__":
    asyncio.run(test_inkling_training_async())
