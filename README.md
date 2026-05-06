# Stock Market Analysis Data Structures (C)

A high-performance C application implementing advanced data structures (Hash Table, Priority Queue, AVL Tree, Min/Max Heap, and Trie) to process and analyze stock market data efficiently.

## 📁 Repository Structure

```
StockMarket-ADS/
├── src/                # C Source files and headers
│   ├── main.c          # Entry point and menu system
│   ├── hashtable.c     # Hash Table (O(1) Stock Lookups)
│   ├── priority_queue.c# Priority Queue (Real-Time Alert Feed)
│   ├── avl_tree.c      # AVL Tree (Chronological Price History)
│   ├── heap.c          # Min/Max Heap (Top Gainers and Losers)
│   ├── trie.c          # Trie (Auto-Complete Engine)
│   └── stock_data.h    # Shared data structures and definitions
├── docs/               # Documentation and Reports
│   ├── Research_Paper.pdf
│   └── Research_Paper.docx
├── web/                # Local web interface for interactive demo
│   ├── server.py       # Python web server
│   └── stock_market_ads.html
├── Makefile            # Build configuration
└── .gitignore
```

## 🚀 Getting Started

### Prerequisites
- GCC Compiler (for C code)
- Python 3.x (optional, for web UI)
- Make (optional, for building)

### Build Instructions

**Using Make:**
```bash
make
./stock_market
```

**Manual Compilation:**
```bash
gcc -o stock_market src/*.c -lm
./stock_market
```

### Web Interface

You can run the web-based visual interface for this project:

```bash
cd web
python server.py
```
Then navigate to `http://localhost:8000` in your web browser.

## 📝 Features

- **Hash Tables**: O(1) stock lookup by ticker.
- **Priority Queues**: Volume-based ranking.
- **AVL Trees**: Fast, balanced chronological querying.
- **Heaps**: Extracting min/max price extremes.
- **Tries**: Autocomplete engine for stock tickers.

## 📄 License
This project is for educational and research purposes.
