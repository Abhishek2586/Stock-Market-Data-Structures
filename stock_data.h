/*
 * stock_data.h - Shared data structures and constants
 * Stock Market Advanced Data Structures Project
 */

#ifndef STOCK_DATA_H
#define STOCK_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SYMBOL_LEN 10

/* Common stock record used across all modules */
typedef struct {
    char symbol[MAX_SYMBOL_LEN];
    double price;
    double change_percent;
    long volume;
} StockRecord;

/* Shared sample stock data used by all modules */
static const StockRecord SAMPLE_STOCKS[] = {
    {"AAPL",  178.50,  1.5,  52000000},
    {"GOOGL", 142.30, -5.2,  21000000},
    {"MSFT",  410.20,  3.2,  28000000},
    {"AMZN",  173.80, -1.8,  38000000},
    {"TSLA",  248.50, 12.3,  95000000},
    {"META",  485.60, -2.1,  15000000},
    {"NVDA",  875.30,  8.5,  42000000},
    {"NFLX",  625.40,  6.7,   8000000},
    {"AMD",   195.30,  9.1,  31000000},
    {"INTC",   42.50, -4.5,  45000000},
    {"ORCL",  118.20,  2.3,  12000000},
    {"IBM",   183.40, -0.8,   8500000}
};

#define NUM_SAMPLE_STOCKS 12

/* Function declarations for each module */
void run_hashtable();
void run_priority_queue();
void run_avltree();
void run_heap();
void run_trie();

#endif
