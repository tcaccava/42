#!/usr/bin/env python3
import subprocess
import time
import socket
import sys


def curl(args):
    return subprocess.run(["curl"] + args, capture_output=True, text=True)

def curl_status(url, extra=None):
    args = ["-o", "/dev/null", "-s", "-w", "%{http_code}"]
    if extra:
        args += extra
    args.append(url)
    res = curl(args)
    try:
        return int(res.stdout.strip())
    except ValueError:
        return 0

def curl_body(url, extra=None):
    args = ["-s"]
    if extra:
        args += extra
    args.append(url)
    res = curl(args)
    return res.stdout

def curl_status_body(url, extra=None):
    args = ["-s", "-w", "\n%{http_code}"]
    if extra:
        args += extra
    args.append(url)
    res = curl(args)
    lines = res.stdout.splitlines()
    code = 0
    if lines:
        try:
            code = int(lines[-1])
        except ValueError:
            code = 0
    body = "\n".join(lines[:-1])
    return code, body

def send_chunked(host, port, path, chunks):
    s = socket.socket()
    s.connect((host, port))
    request = f"POST {path} HTTP/1.1\r\nHost: {host}\r\nTransfer-Encoding: chunked\r\n\r\n"
    s.sendall(request.encode())
    for chunk in chunks:
        size = f"{len(chunk):X}\r\n"
        s.sendall(size.encode())
        s.sendall(chunk.encode())
        s.sendall(b"\r\n")
    s.sendall(b"0\r\n\r\n")
    resp = b""
    while True:
        data = s.recv(4096)
        if not data:
            break
        resp += data
    s.close()
    return resp

def main():
    subprocess.run(["make"], check=True)
    subprocess.run(["bash", "scripts/setup_test_dirs.sh"], check=True)
    server = subprocess.Popen(["./bin/webserv", "conf/webserv_test_complete.conf"])
    time.sleep(1)
    ok = True

    def check(desc, cond):
        nonlocal ok
        if cond:
            print(f"✅ {desc}")
        else:
            print(f"❌ {desc}")
            ok = False

    try:
        code = curl_status("http://localhost:8080/")
        body = curl_body("http://localhost:8080/")
        check("GET /", code == 200 and "Benvenuto su www.local" in body)

        code = curl_status("http://localhost:8080/autoindex/")
        body = curl_body("http://localhost:8080/autoindex/")
        check("Autoindex", code == 200 and "readme.txt" in body)

        code = curl_status("http://localhost:8080/testsite/")
        body = curl_body("http://localhost:8080/testsite/")
        check("Serves test site", code == 200 and "Webserv Test Site" in body)


        code = curl_status("http://localhost:8080/upload", ["-F", "file=@tests/upload.txt"])
        check("POST upload", code in (200, 201))

        code = curl_status("http://localhost:8080/upload/upload.txt", ["-X", "DELETE"])
        check("DELETE uploaded file", code in (200, 204))

        code = curl_status("http://localhost:8080/upload/upload.txt", ["-X", "DELETE"])
        check("DELETE missing file returns 404", code == 404)

        code = curl_status("http://localhost:8080/cgi-bin/test.py")
        body = curl_body("http://localhost:8080/cgi-bin/test.py")
        check("CGI script", code == 200 and "Hello from CGI" in body)

        res = curl(["-s", "-D", "-", "-o", "/dev/null", "http://localhost:8080/oldpage"])
        headers = res.stdout.splitlines()
        status_line = headers[0] if headers else ""
        location = ""
        for h in headers:
            if h.lower().startswith("location:"):
                location = h.split(":", 1)[1].strip()
                break
        check("Redirect /oldpage", status_line.startswith("HTTP/1.1 301") and location == "/newpage")

        code = curl_status("http://localhost:8080/oldpage", ["-L"])
        body = curl_body("http://localhost:8080/oldpage", ["-L"])
        check("Redirect resolves to /newpage", code == 200 and "Sei stato reindirizzato a /newpage" in body)

        code, body = curl_status_body("http://localhost:8080/questa-non-esiste")
        check("404 page", code == 404 and "Pagina non trovata" in body)

        code, body = curl_status_body("http://localhost:8080/cgi-bin/error.py")

        check("500 error page", code == 500 and "Errore interno" in body)

        code, body = curl_status_body("http://localhost:8080/v1/", ["-H", "Host: api.local"])
        check("Host routing", code == 200 and "Benvenuto su api.local" in body)

        code, body = curl_status_body("http://localhost:8080/missing", ["-H", "Host: api.local"])

        check("API 404 page", code == 404 and "API Not Found" in body)

        resp = send_chunked("127.0.0.1", 8080, "/cgi-bin/echo.py", ["chunk1", "chunk2"])
        check("Chunked POST to CGI", b"chunk1chunk2" in resp)

    finally:
        server.terminate()
        server.wait()

    if not ok:
        sys.exit(1)

if __name__ == "__main__":
    main()
