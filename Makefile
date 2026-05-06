# Makefile - Stock Market ADS Project (Unified)
# Run: make && ./stock_market

CC      = gcc
CFLAGS  = -Wall -g
TARGET  = stock_market

SRCS = main.c \
       member1_hashtable.c \
       member2_priority_queue.c \
       member3_avltree.c \
       member4_heap.c \
       member5_trie.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)
	@echo ""
	@echo "  =================================="
	@echo "  Build successful!"
	@echo "  Run with: ./stock_market"
	@echo "  =================================="

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c stock_data.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "  Cleaned."

.PHONY: all clean
