/*
 * MEMBER 3: AVL TREE FOR HISTORICAL STOCK PRICES
 * ===============================================
 *
 * Implements: Self-balancing AVL tree
 * Purpose: Store time-series stock data in sorted order
 * Real-world use: Query historical prices by timestamp efficiently
 */

#include "stock_data.h"
#include <math.h>

typedef struct PriceNode {
    long timestamp;
    double price;
    long volume;
    int height;
    struct PriceNode* left;
    struct PriceNode* right;
} PriceNode;

typedef struct {
    char symbol[MAX_SYMBOL_LEN];
    PriceNode* price_history;
    int total_records;
} StockHistory;

static int avl_rotations = 0;

static int avl_height(PriceNode* n) { return n ? n->height : 0; }
static int avl_max(int a, int b)    { return (a > b) ? a : b; }
static int avl_balance(PriceNode* n){ return n ? avl_height(n->left) - avl_height(n->right) : 0; }

static PriceNode* avl_new_node(long ts, double price, long vol) {
    PriceNode* n = (PriceNode*)malloc(sizeof(PriceNode));
    n->timestamp = ts; n->price = price; n->volume = vol;
    n->height = 1; n->left = n->right = NULL;
    return n;
}

static PriceNode* rotate_right(PriceNode* y) {
    PriceNode* x = y->left, *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    avl_rotations++;
    printf("    [AVL] Right rotation\n");
    return x;
}

static PriceNode* rotate_left(PriceNode* x) {
    PriceNode* y = x->right, *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    avl_rotations++;
    printf("    [AVL] Left rotation\n");
    return y;
}

static PriceNode* avl_insert(PriceNode* node, long ts, double price, long vol) {
    if(!node) return avl_new_node(ts, price, vol);
    if(ts < node->timestamp)      node->left  = avl_insert(node->left,  ts, price, vol);
    else if(ts > node->timestamp) node->right = avl_insert(node->right, ts, price, vol);
    else { node->price = price; node->volume = vol; return node; }

    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
    int bal = avl_balance(node);

    if(bal > 1  && ts < node->left->timestamp)               return rotate_right(node);
    if(bal < -1 && ts > node->right->timestamp)              return rotate_left(node);
    if(bal > 1  && ts > node->left->timestamp)  { node->left  = rotate_left(node->left);  return rotate_right(node); }
    if(bal < -1 && ts < node->right->timestamp) { node->right = rotate_right(node->right); return rotate_left(node);  }
    return node;
}

static void inorder(PriceNode* root) {
    if(!root) return;
    inorder(root->left);
    time_t t = root->timestamp;
    struct tm* tm_info = localtime(&t);
    char buf[20]; strftime(buf, 20, "%H:%M:%S", tm_info);
    printf("    %s | $%-8.2f | Vol: %ld\n", buf, root->price, root->volume);
    inorder(root->right);
}

static PriceNode* find_min_price(PriceNode* r) {
    if(!r) return NULL;
    PriceNode* best = r;
    PriceNode* l = find_min_price(r->left);
    PriceNode* rv = find_min_price(r->right);
    if(l  && l->price  < best->price) best = l;
    if(rv && rv->price < best->price) best = rv;
    return best;
}

static PriceNode* find_max_price(PriceNode* r) {
    if(!r) return NULL;
    PriceNode* best = r;
    PriceNode* l = find_max_price(r->left);
    PriceNode* rv = find_max_price(r->right);
    if(l  && l->price  > best->price) best = l;
    if(rv && rv->price > best->price) best = rv;
    return best;
}

static void calc_stats(PriceNode* r, int* cnt, int* leaves, double* total) {
    if(!r) return;
    (*cnt)++; *total += r->price;
    if(!r->left && !r->right) (*leaves)++;
    calc_stats(r->left,  cnt, leaves, total);
    calc_stats(r->right, cnt, leaves, total);
}

void run_avltree() {
    avl_rotations = 0;
    srand((unsigned)time(NULL));

    printf("\n  ================================================\n");
    printf("  MEMBER 3: AVL TREE - HISTORICAL PRICE DATA\n");
    printf("  ================================================\n\n");

    StockHistory stock;
    strcpy(stock.symbol, "AAPL");
    stock.price_history = NULL;
    stock.total_records = 0;

    printf("  Building price history for %s...\n\n", stock.symbol);

    time_t current_time = time(NULL);
    double base_price = 175.0;

    for(int i = 0; i < 20; i++) {
        long ts = current_time - (3600 - i * 180);
        double price = base_price + ((rand() % 200) - 100) / 100.0;
        long vol = (rand() % 50000 + 10000) * 100L;

        stock.price_history = avl_insert(stock.price_history, ts, price, vol);
        stock.total_records++;

        time_t t = ts; struct tm* ti = localtime(&t);
        char buf[20]; strftime(buf, 20, "%H:%M:%S", ti);
        printf("  [INSERT] %s | $%.2f | Vol: %ld\n", buf, price, vol);
    }

    printf("\n  --- Price History (time-sorted inorder) ---\n\n");
    inorder(stock.price_history);

    printf("\n  --- Price Range Analysis ---\n\n");
    PriceNode* mn = find_min_price(stock.price_history);
    PriceNode* mx = find_max_price(stock.price_history);
    if(mn) {
        time_t t = mn->timestamp; struct tm* ti = localtime(&t);
        char buf[20]; strftime(buf, 20, "%H:%M:%S", ti);
        printf("  Lowest:  $%.2f at %s\n", mn->price, buf);
    }
    if(mx) {
        time_t t = mx->timestamp; struct tm* ti = localtime(&t);
        char buf[20]; strftime(buf, 20, "%H:%M:%S", ti);
        printf("  Highest: $%.2f at %s\n", mx->price, buf);
    }

    int cnt = 0, leaves = 0; double total = 0.0;
    calc_stats(stock.price_history, &cnt, &leaves, &total);

    printf("\n  ========================================\n");
    printf("  AVL TREE STATISTICS\n");
    printf("  ========================================\n");
    printf("  Symbol:              %s\n", stock.symbol);
    printf("  Total Records:       %d\n", cnt);
    printf("  Tree Height:         %d\n", avl_height(stock.price_history));
    printf("  Leaf Nodes:          %d\n", leaves);
    printf("  Average Price:       $%.2f\n", total / cnt);
    printf("  Rotations Performed: %d\n", avl_rotations);
    printf("  Balance Factor:      %d\n", avl_balance(stock.price_history));
    printf("  ========================================\n");

    printf("\n  KEY CONCEPTS DEMONSTRATED:\n");
    printf("  - Self-balancing (AVL) rotations\n");
    printf("  - LL, RR, LR, RL cases\n");
    printf("  - O(log n) insert and search\n");
    printf("  - Time-series sorted storage\n\n");
}
