#!/usr/bin/env python3
import subprocess
import time
import os


def log(msg):
    print(f"\n{msg}")


def run_cmd(desc, cmd):
    log(f"🔹 {desc}")
    print(f"$ {cmd}")
    res = subprocess.run(cmd, shell=True, text=True, capture_output=True)
    if res.stdout:
        print(res.stdout.strip())
    if res.stderr:
        # stampiamo solo stderr “vero”, non la barra progress
        stderr_clean = res.stderr.strip()
        if stderr_clean:
            print(stderr_clean)
    time.sleep(1)  # piccola pausa per leggibilità
    return res.returncode


def main():
    log("🚀 Avvio dei test completi... preparati!")

    # Compila e prepara le directory
    log("🛠️  Compilazione in corso (make)...")
    subprocess.run(["make"], check=True)
    time.sleep(1)

    log("📂 Creo la directory per la pagina 500...")
    os.makedirs("var/www/html", exist_ok=True)
    with open("var/www/html/500.html", "w") as f:
        f.write("<h1>500 - Errore interno</h1>")
    time.sleep(1)

    log("🚀 Avvio del server web...")
    server = subprocess.Popen(["./bin/webserv", "conf/webserv_test_complete.conf"])
    time.sleep(2)

    try:
        log("🌐 Avvio dei test delle rotte...")
        run_cmd("✅ Test homepage", "curl -s -i http://localhost:8080/")
        run_cmd("📁 Test autoindex", "curl -s -i http://localhost:8080/autoindex/")
        run_cmd("⬆️ Upload di un file", "curl -s -i -X POST -F \"file=@tests/upload.txt\" http://localhost:8080/upload")
        run_cmd("🗑️ Delete file", "curl -s -i -X PUT http://localhost:8080/upload/upload.txt")
        run_cmd("🗑️ Delete file che non esiste", "curl -s -i -X DELETE http://localhost:8080/upload/upload2.txt")
        run_cmd("🐍 Esecuzione CGI script", "curl -s -i http://localhost:8080/cgi-bin/test.py")
        run_cmd("🔀 Test redirect", "curl -s -i -L http://localhost:8080/oldpage")
        run_cmd("❌ Test 404 personalizzata", "curl -s -i http://localhost:8080/questa-non-esiste")
        run_cmd("🌍 Test API host", "curl -s -i -H \"Host: api.local\" http://localhost:8080/v1/")
    finally:
        log("🛑 Fermando il server...")
        server.terminate()
        server.wait()
        time.sleep(1)

    log("🎉 Tutti i test sono stati completati con successo! Grazie 🙌")


if __name__ == "__main__":
    main()
