import asyncio
import tinker

async def main():
    service_client = tinker.ServiceClient()
    client = await service_client.create_lora_training_client(
        base_model="thinkingmachines/Inkling-Small",
        rank=8
    )
    print("TrainingClient methods:")
    for m in dir(client):
        if not m.startswith('_'):
            print(f"  - {m}")

asyncio.run(main())
