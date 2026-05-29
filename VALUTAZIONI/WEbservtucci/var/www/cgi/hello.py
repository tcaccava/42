#!/usr/bin/env python3
import time

time.sleep(5)
print("Content-Type: text/html\n")
print("""<!DOCTYPE html>
<html>
<head>
    <title>CGI Redirect Test</title>
    <style>
        body {
            background: linear-gradient(to right, #00b894, #00cec9);
            color: white;
            font-family: Arial, sans-serif;
            padding: 40px;
            text-align: center;
        }
        .container {
            max-width: 600px;
            margin: 0 auto;
            background: rgba(255,255,255,0.1);
            padding: 30px;
            border-radius: 12px;
        }
        .redirect-info {
            background: rgba(255,255,255,0.2);
            padding: 15px;
            border-radius: 8px;
            margin: 20px 0;
        }
        a {
            color: #ddd;
            text-decoration: none;
            padding: 10px 20px;
            background: rgba(0,0,0,0.2);
            border-radius: 5px;
            display: inline-block;
            margin: 10px;
        }
        a:hover {
            background: rgba(0,0,0,0.4);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🐍 Hello from CGI!</h1>
        <div class="redirect-info">
            <p><strong>✅ CGI Redirect Test Successful!</strong></p>
            <p>This CGI script was called via HTTP redirect.</p>
            <p>Both <code>/old-api</code> (307) and <code>/api-v1</code> (308) redirect here.</p>
        </div>
        
        <p>This demonstrates that redirects work correctly with CGI endpoints.</p>
        <p><strong>Method preservation:</strong> 307 and 308 redirects preserve the original HTTP method.</p>
        
        <a href="/">← Back to Home</a>
        <a href="/test">Test Other Features</a>
    </div>
</body>
</html>""")
