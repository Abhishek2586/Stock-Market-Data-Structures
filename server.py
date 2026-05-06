#!/usr/bin/env python3
"""
server.py - Stock Market ADS Web Server
========================================
Run this from inside ADS_unified/ folder:
    python3 server.py

Then open: http://localhost:8000
This server compiles your C code and runs it live when you click buttons.
"""

import http.server
import subprocess
import os
import json
import threading

PORT = 8000
# server.py lives in ADS_unified/, so C files are right here
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
BINARY   = os.path.join(BASE_DIR, "stock_market")

# ── Compile on startup ──────────────────────────────────────────
def compile_c():
    c_files = [
        "main.c",
        "member1_hashtable.c",
        "member2_priority_queue.c",
        "member3_avltree.c",
        "member4_heap.c",
        "member5_trie.c",
    ]
    sources = [os.path.join(BASE_DIR, f) for f in c_files]
    result = subprocess.run(
        ["gcc", "-o", BINARY] + sources + ["-lm"],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        print("[ERROR] Compile error:\n", result.stderr)
        return False
    print("[OK] Compiled successfully ->", BINARY)
    return True

# ── Run a module by sending choice to stdin ─────────────────────
def run_module(choice: str) -> str:
    try:
        result = subprocess.run(
            [BINARY],
            input=f"{choice}\n0\n",
            capture_output=True, text=True, timeout=15
        )
        output = result.stdout
        # Strip menu noise, keep only module section
        lines = output.split('\n')
        start, end = 0, len(lines)
        for i, l in enumerate(lines):
            if '================================================' in l and i+1 < len(lines) and 'MEMBER' in lines[i+1]:
                start = i
                break
        for i in range(len(lines)-1, start, -1):
            if lines[i].strip() and '╔' not in lines[i] and '╚' not in lines[i] and 'Enter choice' not in lines[i]:
                end = i + 1
                break
        return '\n'.join(lines[start:end]).rstrip()
    except subprocess.TimeoutExpired:
        return "ERROR: Program timed out"
    except Exception as e:
        return f"ERROR: {e}"

# ── HTTP Handler ─────────────────────────────────────────────────
class Handler(http.server.BaseHTTPRequestHandler):

    def log_message(self, fmt, *args):
        print(f"  [{self.address_string()}] {fmt % args}")

    def send_cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_cors()
        self.end_headers()

    def do_GET(self):
        # Serve the HTML file
        if self.path in ('/', '/index.html'):
            html_path = os.path.join(BASE_DIR, "stock_market_ads.html")
            if not os.path.exists(html_path):
                self.send_error(404, "stock_market_ads.html not found")
                return
            with open(html_path, 'rb') as f:
                data = f.read()
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_cors()
            self.end_headers()
            self.wfile.write(data)

        # API: run a module live
        # e.g. GET /run?module=1
        elif self.path.startswith('/run'):
            from urllib.parse import urlparse, parse_qs
            qs = parse_qs(urlparse(self.path).query)
            module = qs.get('module', ['6'])[0]
            output = run_module(module)
            resp = json.dumps({"output": output, "module": module}).encode()
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_cors()
            self.end_headers()
            self.wfile.write(resp)

        # API: recompile
        elif self.path == '/compile':
            ok = compile_c()
            resp = json.dumps({"ok": ok}).encode()
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_cors()
            self.end_headers()
            self.wfile.write(resp)

        else:
            self.send_error(404)

if __name__ == "__main__":
    print("\n  Stock Market ADS — Web Server")
    print("  ================================")
    print(f"  Folder : {BASE_DIR}")
    print()

    ok = compile_c()
    if not ok:
        print("\n  [WARN] Could not compile. Make sure gcc is installed.")
        print("  Continuing anyway — you can recompile via /compile\n")

    print(f"\n  Open in browser: http://localhost:{PORT}")
    print("  Press Ctrl+C to stop.\n")

    server = http.server.HTTPServer(("", PORT), Handler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n  Server stopped.")
