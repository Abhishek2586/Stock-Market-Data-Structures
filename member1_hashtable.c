/*
 * MEMBER 1: HASH TABLE FOR STOCK DATA STORAGE
 * ============================================
 *
 * Implements: Hash Table with Chaining for collision resolution
 * Purpose: Store and retrieve stock data in O(1) average time
 * Real-world use: Quick lookup of current stock prices by symbol
 */

#include "stock_data.h"

#define TABLE_SIZE 100

typedef struct Stock {
    char symbol[MAX_SYMBOL_LEN];
    double price;
    long volume;
    double change_percent;
    time_t last_update;
    struct Stock* next;
} Stock;

typedef struct {
    Stock* buckets[TABLE_SIZE];
    int total_stocks;
} HashTable;

typedef struct {
    int total_lookups;
    int collisions;
    int successful_searches;
    int failed_searches;
} HT_Stats;

static HT_Stats ht_stats = {0, 0, 0, 0};

unsigned int hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

HashTable* create_hash_table() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    for(int i = 0; i < TABLE_SIZE; i++)
        ht->buckets[i] = NULL;
    ht->total_stocks = 0;
    return ht;
}

void ht_insert(HashTable* ht, const char* symbol, double price, long volume) {
    unsigned int index = hash_function(symbol);
    Stock* current = ht->buckets[index];

    while(current != NULL) {
        if(strcmp(current->symbol, symbol) == 0) {
            double old_price = current->price;
            current->price = price;
            current->volume = volume;
            current->change_percent = ((price - old_price) / old_price) * 100.0;
            current->last_update = time(NULL);
            printf("  [UPDATE] %s: $%.2f -> $%.2f (%.2f%%)\n",
                   symbol, old_price, price, current->change_percent);
            return;
        }
        current = current->next;
    }

    Stock* new_stock = (Stock*)malloc(sizeof(Stock));
    strcpy(new_stock->symbol, symbol);
    new_stock->price = price;
    new_stock->volume = volume;
    new_stock->change_percent = 0.0;
    new_stock->last_update = time(NULL);

    if(ht->buckets[index] != NULL) {
        ht_stats.collisions++;
        printf("  [COLLISION] at index %u for symbol %s\n", index, symbol);
    }

    new_stock->next = ht->buckets[index];
    ht->buckets[index] = new_stock;
    ht->total_stocks++;

    printf("  [INSERT] %s: $%.2f (Index: %u)\n", symbol, price, index);
}

Stock* ht_search(HashTable* ht, const char* symbol) {
    ht_stats.total_lookups++;
    unsigned int index = hash_function(symbol);
    Stock* current = ht->buckets[index];
    int chain_length = 0;

    while(current != NULL) {
        chain_length++;
        if(strcmp(current->symbol, symbol) == 0) {
            ht_stats.successful_searches++;
            printf("  [FOUND] %s at index %u (chain position: %d)\n",
                   symbol, index, chain_length);
            return current;
        }
        current = current->next;
    }

    ht_stats.failed_searches++;
    printf("  [NOT FOUND] %s\n", symbol);
    return NULL;
}

void display_stock(Stock* stock) {
    if(stock == NULL) { printf("  Stock not found!\n"); return; }
    printf("\n  +-----------------------------------------+\n");
    printf("  | Symbol:  %-10s                   |\n", stock->symbol);
    printf("  | Price:   $%-10.2f               |\n", stock->price);
    printf("  | Volume:  %-10ld               |\n", stock->volume);
    printf("  | Change:  %.2f%%                      |\n", stock->change_percent);
    printf("  +-----------------------------------------+\n");
}

void display_all_stocks(HashTable* ht) {
    printf("\n  HASH TABLE CONTENTS:\n");
    printf("  ====================\n");
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(ht->buckets[i] != NULL) {
            printf("  [Index %d] ", i);
            Stock* current = ht->buckets[i];
            while(current != NULL) {
                printf("%s($%.2f)", current->symbol, current->price);
                if(current->next != NULL) printf(" -> ");
                current = current->next;
            }
            printf("\n");
        }
    }
    printf("\n  Total stocks: %d\n", ht->total_stocks);
}

void display_ht_statistics(HashTable* ht) {
    int empty_buckets = 0, max_chain_len = 0, used_buckets = 0;
    double avg_chain_len = 0.0;

    for(int i = 0; i < TABLE_SIZE; i++) {
        if(ht->buckets[i] == NULL) {
            empty_buckets++;
        } else {
            used_buckets++;
            int chain_len = 0;
            Stock* current = ht->buckets[i];
            while(current != NULL) { chain_len++; current = current->next; }
            if(chain_len > max_chain_len) max_chain_len = chain_len;
            avg_chain_len += chain_len;
        }
    }
    if(used_buckets > 0) avg_chain_len /= used_buckets;

    printf("\n  ========================================\n");
    printf("  HASH TABLE PERFORMANCE STATISTICS\n");
    printf("  ========================================\n");
    printf("  Table Size:           %d\n", TABLE_SIZE);
    printf("  Total Stocks:         %d\n", ht->total_stocks);
    printf("  Load Factor:          %.2f\n", (double)ht->total_stocks / TABLE_SIZE);
    printf("  Max Chain Length:     %d\n", max_chain_len);
    printf("  Avg Chain Length:     %.2f\n", avg_chain_len);
    printf("  Collisions:           %d\n", ht_stats.collisions);
    printf("  Successful Searches:  %d\n", ht_stats.successful_searches);
    printf("  ========================================\n");
}

void run_hashtable() {
    ht_stats = (HT_Stats){0, 0, 0, 0};

    printf("\n  ================================================\n");
    printf("  MEMBER 1: HASH TABLE - STOCK DATA STORAGE\n");
    printf("  ================================================\n\n");

    HashTable* stock_table = create_hash_table();

    printf("  Inserting stock data...\n\n");
    for(int i = 0; i < NUM_SAMPLE_STOCKS; i++)
        ht_insert(stock_table, SAMPLE_STOCKS[i].symbol,
                  SAMPLE_STOCKS[i].price, SAMPLE_STOCKS[i].volume);

    printf("\n  Simulating price updates...\n\n");
    ht_insert(stock_table, "AAPL",  180.25, 53000000);
    ht_insert(stock_table, "TSLA",  245.80, 98000000);

    printf("\n  Testing search...\n\n");
    Stock* s = ht_search(stock_table, "GOOGL");
    display_stock(s);
    s = ht_search(stock_table, "NVDA");
    display_stock(s);
    s = ht_search(stock_table, "XYZ");
    display_stock(s);

    display_all_stocks(stock_table);
    display_ht_statistics(stock_table);

    printf("\n  KEY CONCEPTS DEMONSTRATED:\n");
    printf("  - Hash function (djb2 algorithm)\n");
    printf("  - Collision handling with chaining\n");
    printf("  - O(1) average insert/search\n");
    printf("  - Load factor analysis\n\n");
}
