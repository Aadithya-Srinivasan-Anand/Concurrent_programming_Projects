CFLAGS = -Wall -g -std=c++11
CXX = g++

all: skiplist

skiplist:
	$(CXX) main.cpp headers.cpp skip_list.cpp -o skiplist -pthread $(CFLAGS)
	$(CXX) High_content.cpp headers.cpp skip_list.cpp -o High_content -pthread  $(CFLAGS)
	$(CXX) Low_content.cpp headers.cpp skip_list.cpp -o Low_content -pthread  $(CFLAGS)
	$(CXX) unit_test_1.cpp headers.cpp skip_list.cpp -o unit_test_1 -pthread  $(CFLAGS)
	$(CXX) unit_test_2.cpp headers.cpp skip_list.cpp -o unit_test_2 -pthread  $(CFLAGS)

clean:
	rm skiplist High_content Low_content unit_test_1 unit_test_2