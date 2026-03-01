CXX      = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS  = -lcurl

TARGET = aggregator

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET)
