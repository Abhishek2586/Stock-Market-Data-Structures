/*
 * main.c - Unified Stock Market ADS Project
 * ==========================================
 *
 * Single entry point that connects all 5 data structures.
 * Run: ./stock_market
 */

#include "stock_data.h"

void print_banner() {
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════╗\n");
    printf("  ║     STOCK MARKET - ADVANCED DATA STRUCTURES      ║\n");
    printf("  ║                                                  ║\n");
    printf("  ║  1. Hash Table       (Member 1 - Stock Lookup)   ║\n");
    printf("  ║  2. Priority Queue   (Member 2 - Alert Feed)     ║\n");
    printf("  ║  3. AVL Tree         (Member 3 - Price History)  ║\n");
    printf("  ║  4. Heap             (Member 4 - Gainers/Losers) ║\n");
    printf("  ║  5. Trie             (Member 5 - Auto-Complete)  ║\n");
    printf("  ║  6. Run ALL          (Full demo)                 ║\n");
    printf("  ║  0. Exit                                         ║\n");
    printf("  ╚══════════════════════════════════════════════════╝\n");
    printf("\n  Enter choice: ");
}

int main() {
    int choice;

    printf("\n  Welcome to the Stock Market ADS Project!\n");

    while(1) {
        print_banner();
        if(scanf("%d", &choice) != 1) break;

        switch(choice) {
            case 1: run_hashtable();       break;
            case 2: run_priority_queue();  break;
            case 3: run_avltree();         break;
            case 4: run_heap();            break;
            case 5: run_trie();            break;
            case 6:
                printf("\n  *** RUNNING ALL DATA STRUCTURES ***\n");
                run_hashtable();
                run_priority_queue();
                run_avltree();
                run_heap();
                run_trie();
                printf("\n  *** ALL DEMOS COMPLETE ***\n\n");
                break;
            case 0:
                printf("\n  Goodbye!\n\n");
                return 0;
            default:
                printf("\n  Invalid choice. Try again.\n");
        }

        printf("\n  Press Enter to return to menu...");
        getchar(); getchar();
    }

    return 0;
}
