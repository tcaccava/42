#!/usr/bin/env python3
import os
import sys

method  = os.environ.get("REQUEST_METHOD", "")
query   = os.environ.get("QUERY_STRING", "")
c_len   = int(os.environ.get("CONTENT_LENGTH", 0) or 0)
c_type  = os.environ.get("CONTENT_TYPE", "")

body_in = sys.stdin.read(c_len) if c_len > 0 else ""

print("Content-Type: text/plain")
print()
print("CGI_OK")
print(f"REQUEST_METHOD={method}")
print(f"QUERY_STRING={query}")
print(f"CONTENT_TYPE={c_type}")
print(f"CONTENT_LENGTH={c_len}")
if body_in:
    print(f"BODY={body_in}")
