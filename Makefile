# Makefile - Stock Market ADS Project (Unified)
# Run: make && ./stock_market

CC      = gcc
CFLAGS  = -Wall -g
TARGET  = stock_market

SRCS = src/main.c \
       src/member1_hashtable.c \
       src/member2_priority_queue.c \
       src/member3_avltree.c \
       src/member4_heap.c \
       src/member5_trie.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)
	@echo ""
	@echo "  =================================="
	@echo "  Build successful!"
	@echo "  Run with: ./stock_market"
	@echo "  =================================="

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

src/%.o: src/%.c src/stock_data.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
	@echo "  Cleaned."

.PHONY: all clean
