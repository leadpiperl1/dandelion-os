#!/usr/bin/env python3
"""
Test harness for Shingari v4.4.0 LSP server.
Validates JSON-RPC handshake, Kintsugi 251 diagnostics, and zero-allocation hover responses.
"""
import subprocess
import json
import sys

def send_lsp(proc, msg):
    content = json.dumps(msg)
    payload = f"Content-Length: {len(content)}\r\n\r\n{content}"
    proc.stdin.write(payload.encode('utf-8'))
    proc.stdin.flush()

def read_lsp(proc):
    header = proc.stdout.readline().decode('utf-8')
    if not header or not header.startswith("Content-Length:"):
        return None
    length = int(header.split(":")[1].strip())
    proc.stdout.readline() # empty line
    content = proc.stdout.read(length).decode('utf-8')
    return json.loads(content)

def main():
    print("[TEST] Launching Shingari LSP server...")
    proc = subprocess.Popen(
        ["python3", "scripts/v4.4.0/shingari_lsp_v4_4_0.py"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    # 1. Initialize Handshake
    init_msg = {
        "jsonrpc": "2.0",
        "id": 1,
        "method": "initialize",
        "params": {"capabilities": {}}
    }
    send_lsp(proc, init_msg)
    resp = read_lsp(proc)
    print("[TEST] Initialize Response:", json.dumps(resp, indent=2))
    assert resp["result"]["capabilities"]["hoverProvider"] is True, "Hover provider not enabled!"

    # 2. Test Document Open & Kintsugi 251 Diagnostic Linting
    open_msg = {
        "jsonrpc": "2.0",
        "method": "textDocument/didOpen",
        "params": {
            "textDocument": {
                "uri": "file:///test.shg",
                "languageId": "shingari",
                "version": 1,
                "text": "intent test_intent\n    tau > 0.045\nend"
            }
        }
    }
    send_lsp(proc, open_msg)
    diag_resp = read_lsp(proc)
    print("[TEST] Diagnostic Notification:", json.dumps(diag_resp, indent=2))
    assert diag_resp["method"] == "textDocument/publishDiagnostics", "Expected diagnostics notification"
    assert len(diag_resp["params"]["diagnostics"]) == 1, "Expected 1 Kintsugi 251 warning"
    assert diag_resp["params"]["diagnostics"][0]["code"] == "KINTSUGI-251", "Expected Kintsugi-251 error code"

    # 3. Test Hover Request
    hover_msg = {
        "jsonrpc": "2.0",
        "id": 2,
        "method": "textDocument/hover",
        "params": {
            "textDocument": {"uri": "file:///test.shg"},
            "position": {"line": 1, "character": 4}
        }
    }
    send_lsp(proc, hover_msg)
    hover_resp = read_lsp(proc)
    print("[TEST] Hover Response:", json.dumps(hover_resp, indent=2))
    assert "0 bytes" in hover_resp["result"]["contents"]["value"], "Expected zero allocation metric in hover"

    proc.terminate()
    print("[SUCCESS] All Shingari LSP integration tests passed perfectly!")

if __name__ == "__main__":
    main()
