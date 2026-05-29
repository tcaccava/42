#!/bin/bash

set -e

echo "🧹 Pulizia directory precedente..."
rm -rf var/www

echo "📁 Creazione delle directory..."
mkdir -p var/www/html
mkdir -p var/www/autoindex
mkdir -p var/www/upload
mkdir -p var/www/cgi
mkdir -p var/www/cgi-bin
mkdir -p var/www/api/v1

echo "📝 Creazione file HTML e JSON..."
# Pagina di errore generica
cat > var/www/404.html <<EOF
<h1>404 Not Found</h1>
EOF
# Homepage
cat > var/www/html/index.html <<EOF
<!DOCTYPE html>
<html><body><h1>Benvenuto su www.local!</h1></body></html>
EOF

# Pagina errore 404 per il sito principale
cat > var/www/html/404.html <<EOF
<!DOCTYPE html>
<html><body><h1>404 - Pagina non trovata (custom)</h1></body></html>
EOF

# Pagina errore 500 per il sito principale
cat > var/www/html/500.html <<EOF
<!DOCTYPE html>
<html><body><h1>500 - Errore interno (custom)</h1></body></html>
EOF

# Pagina errore 404 API
cat > var/www/api/api404.html <<EOF
<!DOCTYPE html>
<html><body><h1>404 - API Not Found</h1></body></html>
EOF

# Pagina di destinazione del redirect
cat > var/www/html/newpage.html <<EOF
<!DOCTYPE html>
<html><body><h1>Sei stato reindirizzato a /newpage!</h1></body></html>
EOF

# Form per test di upload
cat > var/www/upload_form.html <<EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>File Upload Test</title>
</head>
<body>
    <h1>Upload a file</h1>
    <form action="/upload" method="post" enctype="multipart/form-data">
        <input type="file" name="file">
        <button type="submit">Upload</button>
    </form>
</body>
</html>
EOF

# Autoindex file
echo "Questa directory serve per testare l'autoindex." > var/www/autoindex/readme.txt

# File JSON per API
echo '{"message": "Benvenuto su api.local/v1"}' > var/www/api/v1/index.json

echo "🐍 Script CGI: base"
# CGI base
cat > var/www/cgi/test.py <<EOF
#!/usr/bin/env python3
print("Content-Type: text/html\n")
print("<html><body><h1>CGI Python OK!</h1></body></html>")
EOF
chmod +x var/www/cgi/test.py

# CGI echo
cat > var/www/cgi/echo.py <<EOF
#!/usr/bin/env python3
import sys
body = sys.stdin.read()
print("Content-Type: text/plain")
print("")
print(body)
EOF
chmod +x var/www/cgi/echo.py

# CGI nella cartella cgi-bin
cat > var/www/cgi-bin/test.py <<EOF
#!/usr/bin/env python3

print("Content-Type: text/html")
print("")
print("<html><body>")
print("<h1>Hello from CGI!</h1>")
print("</body></html>")
EOF
chmod +x var/www/cgi-bin/test.py

# CGI echo nella cartella cgi-bin (per test chunked)
cat > var/www/cgi-bin/echo.py <<EOF
#!/usr/bin/env python3
import sys
body = sys.stdin.read()
print("Content-Type: text/plain")
print("")
print(body)
EOF
chmod +x var/www/cgi-bin/echo.py

# CGI che genera un errore per test 500
cat > var/www/cgi-bin/error.py <<EOF
#!/usr/bin/env python3
import sys
sys.exit(1)
EOF
chmod +x var/www/cgi-bin/error.py

echo "📦 File per test DELETE"
echo "Questo file verrà usato per testare DELETE." > var/www/upload/delete_me.txt


# Copia del sito statico di test
cp -r testsite var/www/html/

echo "✅ Setup completato!"

