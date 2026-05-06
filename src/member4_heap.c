/*
 * MEMBER 4: HEAP FOR TOP GAINERS/LOSERS RANKING
 * ==============================================
 *
 * Implements: Max Heap (gainers) and Min Heap (losers)
 * Purpose: Efficiently track top performing stocks
 * Real-world use: Display market movers in real-time
 */

#include "stock_data.h"

#define MAX_HEAP_SIZE 100

typedef struct {
    char symbol[MAX_SYMBOL_LEN];
    double price;
    double change_percent;
    long volume;
} HeapStock;

typedef struct {
    HeapStock stocks[MAX_HEAP_SIZE];
    int size;
    int is_max_heap;
} Heap;

static Heap* create_heap(int is_max) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->size = 0; h->is_max_heap = is_max;
    return h;
}

static void swap_s(HeapStock* a, HeapStock* b) { HeapStock t = *a; *a = *b; *b = t; }

static void heapify_up(Heap* h, int i) {
    if(i == 0) return;
    int p = (i - 1) / 2;
    int should_swap = h->is_max_heap
        ? (h->stocks[p].change_percent < h->stocks[i].change_percent)
        : (h->stocks[p].change_percent > h->stocks[i].change_percent);
    if(should_swap) { swap_s(&h->stocks[p], &h->stocks[i]); heapify_up(h, p); }
}

static void heapify_down(Heap* h, int i) {
    int target = i;
    int l = 2*i+1, r = 2*i+2;
    if(h->is_max_heap) {
        if(l < h->size && h->stocks[l].change_percent > h->stocks[target].change_percent) target = l;
        if(r < h->size && h->stocks[r].change_percent > h->stocks[target].change_percent) target = r;
    } else {
        if(l < h->size && h->stocks[l].change_percent < h->stocks[target].change_percent) target = l;
        if(r < h->size && h->stocks[r].change_percent < h->stocks[target].change_percent) target = r;
    }
    if(target != i) { swap_s(&h->stocks[i], &h->stocks[target]); heapify_down(h, target); }
}

static void heap_insert(Heap* h, const char* sym, double price, double change, long vol) {
    if(h->size >= MAX_HEAP_SIZE) { printf("  [ERROR] Heap full!\n"); return; }
    strcpy(h->stocks[h->size].symbol, sym);
    h->stocks[h->size].price = price;
    h->stocks[h->size].change_percent = change;
    h->stocks[h->size].volume = vol;
    printf("  [INSERT] %-8s %+6.2f%%  ($%.2f)\n", sym, change, price);
    heapify_up(h, h->size++);
}

static HeapStock heap_extract(Heap* h) {
    HeapStock empty = {"", 0, 0, 0};
    if(h->size == 0) return empty;
    HeapStock top = h->stocks[0];
    h->stocks[0] = h->stocks[--h->size];
    if(h->size > 0) heapify_down(h, 0);
    return top;
}

static void display_top_n(Heap* h, int n, const char* title) {
    printf("\n  %s (Top %d)\n", title, n);
    printf("  ========================================\n");
    if(h->size == 0) { printf("  No data\n"); return; }
    Heap temp = *h;
    int count = (n < temp.size) ? n : temp.size;
    for(int i = 0; i < count; i++) {
        HeapStock s = heap_extract(&temp);
        printf("  %d. %-8s %+7.2f%%  ($%.2f, Vol: %ld)\n",
               i+1, s.symbol, s.change_percent, s.price, s.volume);
    }
}

void run_heap() {
    printf("\n  ================================================\n");
    printf("  MEMBER 4: HEAP - TOP GAINERS/LOSERS\n");
    printf("  ================================================\n\n");

    Heap* gainers = create_heap(1);
    Heap* losers  = create_heap(0);

    printf("  Processing market data...\n\n");
    for(int i = 0; i < NUM_SAMPLE_STOCKS; i++) {
        const StockRecord* s = &SAMPLE_STOCKS[i];
        if(s->change_percent > 0)
            heap_insert(gainers, s->symbol, s->price, s->change_percent, s->volume);
        else
            heap_insert(losers,  s->symbol, s->price, s->change_percent, s->volume);
    }

    display_top_n(gainers, 5, "TOP GAINERS");
    display_top_n(losers,  5, "TOP LOSERS");

    printf("\n  ========================================\n");
    printf("  HEAP STATISTICS\n");
    printf("  ========================================\n");
    printf("  Total Gainers:  %d\n", gainers->size);
    printf("  Total Losers:   %d\n", losers->size);
    if(gainers->size > 0) printf("  Best:  %s (+%.2f%%)\n", gainers->stocks[0].symbol, gainers->stocks[0].change_percent);
    if(losers->size  > 0) printf("  Worst: %s (%.2f%%)\n",  losers->stocks[0].symbol,  losers->stocks[0].change_percent);
    printf("  ========================================\n");

    printf("\n  KEY CONCEPTS DEMONSTRATED:\n");
    printf("  - Max heap for gainers, min heap for losers\n");
    printf("  - heapify_up on insert, heapify_down on extract\n");
    printf("  - O(log n) insert, O(1) peek at top\n\n");

    free(gainers); free(losers);
}
