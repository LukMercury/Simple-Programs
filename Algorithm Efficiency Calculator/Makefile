CC = g++
CFLAGS = -std=c++11 -Wall
ODIR = bin
DEPS = compose_function.h functions.h line_data.h parse.h \
	std_lib_facilities.h
SRCS = compose_function.cpp functions.cpp line_data.cpp parse.cpp aec.cpp
_OBJS = $(SRCS:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: all dir build clean

all: dir build

dir:
	mkdir -p $(ODIR);

build: $(ODIR)/aec

$(ODIR)/aec: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(ODIR)/aec $(ODIR)/*.o
