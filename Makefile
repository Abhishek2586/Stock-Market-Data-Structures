# Makefile - Stock Market ADS Project (Unified)
# Run: make && ./stock_market

CC      = gcc
CFLAGS  = -Wall -g
TARGET  = stock_market

SRCS = src/main.c \
       src/hashtable.c \
       src/priority_queue.c \
       src/avl_tree.c \
       src/heap.c \
       src/trie.c

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
