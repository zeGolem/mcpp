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

$(OUTPUT): $(SOURCES)
	$(CXX) $(FLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)

$(OUTPUT_DBG): $(SOURCES)
	$(CXX) -g3 $(FLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT_DBG)
