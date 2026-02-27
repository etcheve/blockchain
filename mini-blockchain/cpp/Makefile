CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 \
            -I/opt/homebrew/include
LDFLAGS  := -L/opt/homebrew/lib -lssl -lcrypto

TARGET  := blockchain
SRCS    := main.cpp block.cpp blockchain.cpp
OBJS    := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
