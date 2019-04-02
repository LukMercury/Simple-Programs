CC = g++
CFLAGS = -std=c++11 -Wall
ODIR = bin
DEPS = compose_function.h functions.h line_data.h parse.h
SRCS = compose_function.cpp functions.cpp line_data.cpp parse.cpp aec.cpp
_OBJS = $(SRCS:.cpp=.o)
OBJS =  $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: build dir clean

build: dir $(ODIR)/aec

dir:
	mkdir -p $(ODIR);

$(ODIR)/aec: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(ODIR)/aec $(ODIR)/*.o
