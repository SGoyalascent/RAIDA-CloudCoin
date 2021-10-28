
import websockets
import asyncio

async def listen():
    url = "wss://127.0.0.1:3000"

    async with websockets.connect(url, ping_interval=None) as ws:
        await ws.send("Heelo SHUBHAM")
        while True:
            msg = await ws.recv()
            print(msg)


asyncio.get_event_loop().run_until_complete(listen())
