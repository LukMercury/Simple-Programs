CC = g++
CFLAGS = -std=c++11 -Wall
SRCS = compose_function.cpp functions.cpp line_data.cpp parse.cpp aec.cpp
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp %.hcm
	$(CC) -c -o $@ $< $(CFLAGS)

aec: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm aec *.o
