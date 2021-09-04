SOURCES=$(shell find src/ -type f -name '*.cpp')
OBJS=$(SOURCES:.cpp=.o)
OBJS:=$(patsubst src/%,build/objs/%,$(OBJS))
OBJS_DBG=$(OBJS:.o=.dbg.o)


CXX=g++
LIBS=-lpthread
FLAGS=-std=c++20 -I./src -Wall
OUTPUT=build/mcpp
OUTPUT_DBG=build/mcpp.dbg

JSON_TAGS=build/tags/*/**/*.json

NBT=dimension.nbt dimension_codec.nbt
NBT_OUT=$(patsubst %,build/nbt/%,$(NBT))

build/objs/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(FLAGS) -c $< -o $@

build/nbt/%.nbt: tools/nbt/generators/%.py
	python3 $<
	mkdir -p build/nbt
	mv tools/nbt/out/*.nbt build/nbt

build/objs/%.dbg.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(FLAGS) -c $< -o $@

$(OUTPUT): $(OBJS) $(JSON_TAGS) $(NBT_OUT)
	$(CXX) $(LIBS) $(OBJS) -o $(OUTPUT)

$(OUTPUT_DBG): $(OBJS_DBG) $(JSON_TAGS) $(NBT_OUT)
	$(CXX) $(LIBS) $(OBJS_DBG) -o $(OUTPUT_DBG)

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
