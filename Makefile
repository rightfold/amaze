LD=g++
LDFLAGS=$(shell pkg-config --libs gl glfw3)

CXX=g++
CXXFLAGS=-std=c++14 -Wall -Wextra -Wpedantic

CXX_SOURCES=$(shell find src -type f -name '*.cpp')
CXX_HEADERS=$(shell find src -type f -name '*.hpp')
CXX_OBJECTS=$(patsubst %.cpp,build/%.cpp.o,${CXX_SOURCES})

TARGET=build/amaze

all: ${TARGET}

.PHONY: clean
clean:
	rm -rf build

${TARGET}: ${CXX_OBJECTS}
	mkdir -p $(dir $@)
	${LD} ${LDFLAGS} -o $@ $^

build/%.cpp.o: %.cpp | ${CXX_HEADERS}
	mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -c -o $@ $^
