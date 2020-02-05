SRCS=main.cpp client.cpp server.cpp

OBJS=${SRCS:.cpp=.o}

miproxy: $(OBJS)
	$(CXX) -o $@ $^

clean:
	rm *.o
