CXX      = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS  = -lcurl

SRCS   = main.cpp connection/binance_client.cpp
TARGET = binance_test

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
