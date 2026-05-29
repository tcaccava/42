import socket
import sys
import subprocess
import time

def send_chunked(host, port, path, chunks):
    s = socket.socket()
    s.connect((host, port))
    request = f"POST {path} HTTP/1.1\r\nHost: {host}\r\ntransfer-encoding: chunked\r\n\r\n"
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
    print(resp.decode(errors='ignore'))

if __name__ == "__main__":
    chunks = ["Hello, ", "world"]
    host = "127.0.0.1"
    port = 8080
    path = "/cgi-bin/echo.py"
    subprocess.run(["make"], check=True)
    subprocess.run(["bash", "scripts/setup_test_dirs.sh"], check=True)
    server = subprocess.Popen(["./bin/webserv", "conf/webserv_test.conf"],
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                              text=True)
    time.sleep(1)
    try:
        send_chunked(host, port, path, chunks)
    finally:
        server.terminate()
        out, _ = server.communicate()
        print(out)
        if "Richiesta chunked" not in out:
            sys.exit("Log not found")
