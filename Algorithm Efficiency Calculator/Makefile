.PHONY: all dir build clean

CC = g++
CFLAGS = -std=c++11 -Wall

ODIR = bin
DEPS = $(wildcard *.h)
SRCS = $(wildcard *.cpp)
_OBJS = $(SRCS:.cpp=.o)
OBJS = $(patsubst %,${ODIR}/%,${_OBJS})
_TARGET = aec
TARGET=$(patsubst %,${ODIR}/%,${_TARGET})

all: dir build

dir:
	@mkdir -p ${ODIR}

build: ${TARGET}

${TARGET}: ${OBJS}
	${CC} -o $@ $^ ${CFLAGS}

${ODIR}/%.o: %.cpp ${DEPS}
	${CC} -c -o $@ $< ${CFLAGS}

clean:
	rm ${TARGET} ${OBJS}
