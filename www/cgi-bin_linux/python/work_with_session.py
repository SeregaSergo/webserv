#!/usr/bin/python3

import os

print("Content-Type: text/html\r\n")

print("""
<!DOCTYPE html>
<html>
<body>
""")

try:
    cookie = os.environ['HTTP_COOKIE']
except:
    print("<h1>I didn't get cookie<\h1></body></html>")
    exit()

cookie = cookie[4:]
text = None
root = os.environ.get("DOCUMENT_ROOT")
if root is None:
    print("<h1>I need root of your cookie, but i didn't get it, sorry<\h1></body></html>")
    exit()
elif os.environ.get("REQUEST_METHOD") == "GET":
    with open(root + ".sessions/" + cookie) as f:
        text = f.read()
elif os.environ.get("REQUEST_METHOD") == "POST":
    with open(root + ".sessions/" + cookie, 'w') as f:
        text = input()[5:].replace('%0D%0A', '\n')
        f.write(text)
else:
    print("<h1>I only process GET and POST, but you want <\h1></body></html>")
    exit()

print(f"""
<head>
<meta charset="UTF-8">
<title>Simple form</title>
</head>
<form method="post">
    <p><b>Your note:</b></p>
    <p><textarea rows="10" cols="45" name="text">{text}</textarea></p>
    <p><input type="submit" value="Submit"></p>
  </form>
</body>
</html>
""")

