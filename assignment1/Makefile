CC = g++
CFLAGS = -std=c++11
OMP_CFLAGS = -std=c++11 -fopenmp

SRCS = partsp.cpp seqtsp.cpp
OBJS = $(SRCS:.cpp=.o)

PARTSP_TARGET = partsp
SEQTSP_TARGET = seqtsp

all: $(PARTSP_TARGET) $(SEQTSP_TARGET)

$(PARTSP_TARGET): partsp.o
	$(CC) $(OMP_CFLAGS) -o $@ partsp.o

$(SEQTSP_TARGET): seqtsp.o
	$(CC) $(CFLAGS) -o $@ seqtsp.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

partsp.o: partsp.cpp
	$(CC) $(OMP_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(PARTSP_TARGET) $(SEQTSP_TARGET)

