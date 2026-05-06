/*
 * MEMBER 5: TRIE FOR STOCK SYMBOL AUTO-COMPLETE
 * ==============================================
 *
 * Implements: Trie (Prefix Tree)
 * Purpose: Fast prefix-based search for stock symbols
 * Real-world use: Search suggestions as user types
 */

#include "stock_data.h"
#include <ctype.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int is_end;
    char symbol[MAX_SYMBOL_LEN];
    double last_price;
} TrieNode;

static int trie_nodes = 0;
static int trie_words = 0;

static TrieNode* trie_new_node() {
    TrieNode* n = (TrieNode*)calloc(1, sizeof(TrieNode));
    trie_nodes++;
    return n;
}

static int char_idx(char c) { return toupper(c) - 'A'; }

static void trie_insert(TrieNode* root, const char* symbol, double price) {
    TrieNode* cur = root;
    printf("  [INSERT] %s ($%.2f)\n", symbol, price);
    for(int i = 0; symbol[i]; i++) {
        int idx = char_idx(symbol[i]);
        if(idx < 0 || idx >= ALPHABET_SIZE) continue;
        if(!cur->children[idx]) cur->children[idx] = trie_new_node();
        cur = cur->children[idx];
    }
    cur->is_end = 1;
    strncpy(cur->symbol, symbol, MAX_SYMBOL_LEN - 1);
    cur->last_price = price;
    trie_words++;
}

static int trie_search(TrieNode* root, const char* symbol) {
    TrieNode* cur = root;
    for(int i = 0; symbol[i]; i++) {
        int idx = char_idx(symbol[i]);
        if(idx < 0 || idx >= ALPHABET_SIZE || !cur->children[idx]) return 0;
        cur = cur->children[idx];
    }
    return cur && cur->is_end;
}

static void collect(TrieNode* node, char suggestions[][MAX_SYMBOL_LEN], int* cnt, int max) {
    if(*cnt >= max) return;
    if(node->is_end) strcpy(suggestions[(*cnt)++], node->symbol);
    for(int i = 0; i < ALPHABET_SIZE; i++)
        if(node->children[i]) collect(node->children[i], suggestions, cnt, max);
}

static int get_suggestions(TrieNode* root, const char* prefix,
                            char suggestions[][MAX_SYMBOL_LEN], int max) {
    TrieNode* cur = root;
    for(int i = 0; prefix[i]; i++) {
        int idx = char_idx(prefix[i]);
        if(idx < 0 || idx >= ALPHABET_SIZE || !cur->children[idx]) return 0;
        cur = cur->children[idx];
    }
    int cnt = 0;
    collect(cur, suggestions, &cnt, max);
    return cnt;
}

void run_trie() {
    trie_nodes = 0; trie_words = 0;

    printf("\n  ================================================\n");
    printf("  MEMBER 5: TRIE - STOCK SYMBOL AUTO-COMPLETE\n");
    printf("  ================================================\n\n");

    TrieNode* root = trie_new_node();

    struct { char symbol[MAX_SYMBOL_LEN]; double price; } stocks[] = {
        {"AAPL", 178.50}, {"AMD",  195.30}, {"AMZN", 173.80},
        {"GOOGL",142.30}, {"GOOG", 143.10}, {"META", 485.60},
        {"MSFT", 410.20}, {"NVDA", 875.30}, {"NFLX", 625.40},
        {"TSLA", 248.50}, {"IBM",  183.40}, {"INTC",  42.50},
        {"ORCL", 118.20}, {"ADBE", 562.30}, {"CSCO",  51.20},
        {"AVGO",1295.60}, {"TXN",  188.90}, {"QCOM", 172.40}
    };
    int n = sizeof(stocks)/sizeof(stocks[0]);

    printf("  Building symbol trie...\n\n");
    for(int i = 0; i < n; i++)
        trie_insert(root, stocks[i].symbol, stocks[i].price);

    printf("\n  --- Exact Search ---\n\n");
    const char* tests[] = {"AAPL", "GOOGL", "XYZ", "MSFT"};
    for(int i = 0; i < 4; i++)
        printf("  Search '%-6s': %s\n", tests[i],
               trie_search(root, tests[i]) ? "FOUND" : "NOT FOUND");

    printf("\n  --- Auto-Complete ---\n\n");
    const char* prefixes[] = {"A", "G", "M", "NV"};
    for(int i = 0; i < 4; i++) {
        char sugg[20][MAX_SYMBOL_LEN];
        int cnt = get_suggestions(root, prefixes[i], sugg, 20);
        printf("  Prefix '%s' -> ", prefixes[i]);
        for(int j = 0; j < cnt; j++) printf("%s%s", sugg[j], j<cnt-1?", ":"");
        printf("\n");
    }

    printf("\n  --- Simulating user typing 'AAPL' ---\n\n");
    const char* typing[] = {"A", "AA", "AAP", "AAPL"};
    for(int i = 0; i < 4; i++) {
        char sugg[20][MAX_SYMBOL_LEN];
        int cnt = get_suggestions(root, typing[i], sugg, 10);
        printf("  '%s' -> ", typing[i]);
        for(int j = 0; j < cnt; j++) printf("%s ", sugg[j]);
        printf("\n");
    }

    printf("\n  ========================================\n");
    printf("  TRIE STATISTICS\n");
    printf("  ========================================\n");
    printf("  Total Symbols:  %d\n", trie_words);
    printf("  Total Nodes:    %d\n", trie_nodes);
    printf("  Memory Usage:   ~%d bytes\n", trie_nodes * (int)sizeof(TrieNode));
    printf("  ========================================\n");

    printf("\n  KEY CONCEPTS DEMONSTRATED:\n");
    printf("  - Prefix tree (Trie) structure\n");
    printf("  - O(m) search (m = key length)\n");
    printf("  - Auto-complete in O(m + results)\n");
    printf("  - Space-time trade-off\n\n");
}
