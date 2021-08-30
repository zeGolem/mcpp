#!/usr/bin/env python3

import requests
import os

# Server URL for version 1.17.1
SERVER_URL = "https://launcher.mojang.com/v1/objects/a16d67e5807f57fc4e550299cf20226194497dc2/server.jar"
DOWNLOAD_CHUNK_SIZE = 4096

if os.path.exists("server/server.jar"):
    exit()

server = requests.get(SERVER_URL, stream=True)
with open("server/server.jar", 'wb') as fp:
    for data in server.iter_content(DOWNLOAD_CHUNK_SIZE):
        fp.write(data)

