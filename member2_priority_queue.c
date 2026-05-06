/*
 * MEMBER 2: PRIORITY QUEUE FOR STOCK ALERT PROCESSING
 * =====================================================
 *
 * Implements: Priority Queue using a Min-Max Heap
 * Purpose: Process trade alerts by urgency, not just arrival order
 * Real-world use: Critical market alerts (circuit breakers, halts)
 *                 are processed before routine price updates
 *
 * Priority levels:
 *   1 = CRITICAL  (circuit breaker, trading halt)
 *   2 = HIGH      (large volume spike, index move)
 *   3 = MEDIUM    (earnings update, analyst rating)
 *   4 = LOW       (routine price tick)
 */

#include "stock_data.h"
#include <unistd.h>

#define PQ_CAPACITY 100

/* Alert structure stored in the priority queue */
typedef struct {
    char symbol[MAX_SYMBOL_LEN];
    double price;
    long volume;
    int priority;         /* 1=CRITICAL, 2=HIGH, 3=MEDIUM, 4=LOW */
    char alert_type[30];
    char timestamp[20];
} StockAlert;

/* Priority Queue (min-heap on priority field) */
typedef struct {
    StockAlert data[PQ_CAPACITY];
    int size;
    int total_inserted;
    int total_processed;
} PriorityQueue;

/* ---- Helper functions ---- */

static int pq_parent(int i)      { return (i - 1) / 2; }
static int pq_left(int i)        { return 2 * i + 1; }
static int pq_right(int i)       { return 2 * i + 2; }

static void pq_swap(StockAlert* a, StockAlert* b) {
    StockAlert tmp = *a; *a = *b; *b = tmp;
}

static const char* priority_label(int p) {
    switch(p) {
        case 1: return "CRITICAL";
        case 2: return "HIGH    ";
        case 3: return "MEDIUM  ";
        default:return "LOW     ";
    }
}

/* Bubble up after insert to restore heap property */
static void heapify_up(PriorityQueue* pq, int idx) {
    while(idx > 0) {
        int par = pq_parent(idx);
        /* Lower priority number = higher urgency = should be at top */
        if(pq->data[par].priority > pq->data[idx].priority) {
            pq_swap(&pq->data[par], &pq->data[idx]);
            idx = par;
        } else {
            break;
        }
    }
}

/* Push down after extract to restore heap property */
static void heapify_down(PriorityQueue* pq, int idx) {
    int smallest = idx;
    int left  = pq_left(idx);
    int right = pq_right(idx);

    if(left  < pq->size && pq->data[left].priority  < pq->data[smallest].priority)
        smallest = left;
    if(right < pq->size && pq->data[right].priority < pq->data[smallest].priority)
        smallest = right;

    if(smallest != idx) {
        pq_swap(&pq->data[idx], &pq->data[smallest]);
        heapify_down(pq, smallest);
    }
}

/* ---- Public API ---- */

PriorityQueue* pq_create() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    pq->total_inserted = 0;
    pq->total_processed = 0;
    return pq;
}

int pq_is_empty(PriorityQueue* pq) { return pq->size == 0; }
int pq_is_full(PriorityQueue* pq)  { return pq->size == PQ_CAPACITY; }

void pq_insert(PriorityQueue* pq, const char* symbol, double price,
               long volume, int priority, const char* alert_type) {
    if(pq_is_full(pq)) {
        printf("  [WARNING] Queue full! Dropping alert for %s\n", symbol);
        return;
    }

    StockAlert* a = &pq->data[pq->size];
    strncpy(a->symbol, symbol, MAX_SYMBOL_LEN - 1);
    a->price    = price;
    a->volume   = volume;
    a->priority = priority;
    strncpy(a->alert_type, alert_type, 29);

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    sprintf(a->timestamp, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

    pq->size++;
    pq->total_inserted++;
    heapify_up(pq, pq->size - 1);

    printf("  [ENQUEUE] [%s] %-8s $%.2f  (%s)\n",
           priority_label(priority), symbol, price, alert_type);
}

StockAlert pq_extract(PriorityQueue* pq) {
    StockAlert empty = {"", 0, 0, 0, "", ""};
    if(pq_is_empty(pq)) {
        printf("  [ERROR] Queue empty!\n");
        return empty;
    }

    StockAlert top = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    if(pq->size > 0) heapify_down(pq, 0);
    pq->total_processed++;
    return top;
}

void pq_peek(PriorityQueue* pq) {
    if(pq_is_empty(pq)) { printf("  Queue is empty.\n"); return; }
    StockAlert* a = &pq->data[0];
    printf("  [PEEK] Next to process: [%s] %s - %s\n",
           priority_label(a->priority), a->symbol, a->alert_type);
}

void pq_display(PriorityQueue* pq) {
    printf("\n  PRIORITY QUEUE STATE (size: %d)\n", pq->size);
    printf("  %-10s %-8s %-8s %s\n", "PRIORITY", "SYMBOL", "PRICE", "ALERT TYPE");
    printf("  --------------------------------------------------\n");
    for(int i = 0; i < pq->size; i++) {
        StockAlert* a = &pq->data[i];
        printf("  [%s] %-8s $%-8.2f %s\n",
               priority_label(a->priority), a->symbol, a->price, a->alert_type);
    }
    printf("\n");
}

void process_alert(StockAlert* a) {
    printf("  >>> PROCESSING ALERT <<<\n");
    printf("      Priority:  [%s]\n", priority_label(a->priority));
    printf("      Symbol:    %s\n",   a->symbol);
    printf("      Price:     $%.2f\n", a->price);
    printf("      Volume:    %ld\n",  a->volume);
    printf("      Type:      %s\n",   a->alert_type);
    printf("      Time:      %s\n",   a->timestamp);
}

/* ---- Main demo ---- */

void run_priority_queue() {
    printf("\n  ================================================\n");
    printf("  MEMBER 2: PRIORITY QUEUE - ALERT PROCESSING\n");
    printf("  ================================================\n\n");

    PriorityQueue* pq = pq_create();

    printf("  --- Phase 1: Incoming alerts (mixed priorities) ---\n\n");

    /* Simulate alerts arriving in random priority order */
    pq_insert(pq, "MSFT",  410.20, 28000000, 4, "Routine price tick");
    pq_insert(pq, "AAPL",  178.50, 52000000, 3, "Earnings update");
    pq_insert(pq, "TSLA",  248.50, 95000000, 1, "Trading HALT issued");
    pq_insert(pq, "NVDA",  875.30, 42000000, 2, "Volume spike +300%");
    pq_insert(pq, "AMZN",  173.80, 38000000, 4, "Routine price tick");
    pq_insert(pq, "META",  485.60, 15000000, 3, "Analyst downgrade");
    pq_insert(pq, "GOOGL", 142.30, 21000000, 1, "Circuit breaker -7%");
    pq_insert(pq, "NFLX",  625.40,  8000000, 2, "Index rebalance");
    pq_insert(pq, "AMD",   195.30, 31000000, 4, "Routine price tick");
    pq_insert(pq, "IBM",   183.40,  8500000, 3, "Dividend announced");

    printf("\n  Queue after loading all alerts:\n");
    pq_display(pq);
    pq_peek(pq);

    printf("\n  --- Phase 2: Processing by priority (urgent first) ---\n\n");

    /* Extract all alerts — critical ones come out first regardless of insertion order */
    int count = 0;
    while(!pq_is_empty(pq)) {
        StockAlert alert = pq_extract(pq);
        printf("\n  [%d] Extracted:\n", ++count);
        process_alert(&alert);
        usleep(100000);
    }

    printf("\n  ========================================\n");
    printf("  PRIORITY QUEUE STATISTICS\n");
    printf("  ========================================\n");
    printf("  Capacity:         %d\n", PQ_CAPACITY);
    printf("  Total Inserted:   %d\n", pq->total_inserted);
    printf("  Total Processed:  %d\n", pq->total_processed);
    printf("  ========================================\n");

    printf("\n  KEY CONCEPTS DEMONSTRATED:\n");
    printf("  - Min-heap enforces priority ordering\n");
    printf("  - O(log n) insert via heapify_up\n");
    printf("  - O(log n) extract via heapify_down\n");
    printf("  - O(1) peek at highest-priority alert\n");
    printf("  - Critical alerts processed before LOW ones\n");
    printf("    regardless of arrival order\n\n");

    free(pq);
}
