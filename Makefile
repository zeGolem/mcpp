SOURCES=src/main.cpp                       \
		src/minecraft_client.cpp           \
		src/minecraft_server.cpp           \
		src/network/tcp_server.cpp         \
		src/network/tcp_connection.cpp     \
		src/protocol/packet_parser.cpp     \
		src/protocol/packet_serializer.cpp \
		src/protocol/types.cpp             \
		src/protocol/packets/packet.cpp    \


CXX=g++
LIBS=-lpthread
FLAGS=-std=c++20 -I./src -Wall
OUTPUT=build/mcpp
OUTPUT_DBG=build/mcpp.dbg

JSON_TAGS=build/tags/*/**/*.json

$(OUTPUT): $(SOURCES) $(NBT_DATA) $(JSON_TAGS)
	$(CXX) $(FLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)

$(OUTPUT_DBG): $(SOURCES) $(NBT_DATA) $(JSON_TAGS)
	$(CXX) -g3 $(FLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT_DBG)

$(JSON_TAGS): tools/server/extracted/
	cp -r tools/server/extracted/data/minecraft/tags build/

tools/server/extracted/: tools/server/server.jar
	mkdir tools/server/extracted
	7z x tools/server/server.jar -otools/server/extracted
	rm tools/server/extracted/*.class

tools/server/server.jar:
	mkdir -p tools/server
	# 1.17.1 server
	curl "https://launcher.mojang.com/v1/objects/a16d67e5807f57fc4e550299cf20226194497dc2/server.jar" > tools/server/server.jar
