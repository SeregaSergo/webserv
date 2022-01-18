#!/usr/local/bin/python3

import os

print("Content-type: text/html\r\n\r\n")

try:
    cookie = os.environ['HTTP_COOKIE']
except:
    print("<h1>I didn't get cookie<\h1>")
    exit()

cookie = cookie[4:]
text = None
root = os.environ.get("DOCUMENT_ROOT")
if root is None:
    print("<h1>I need root of your cookie, but i didn't get it, sorry<\h1>")
    exit()
elif os.environ.get("REQUEST_METHOD") == "GET":
    with open(root + ".sessions/" + cookie) as f:
        text = f.read()
elif os.environ.get("REQUEST_METHOD") == "POST":
    with open(root + ".sessions/" + cookie, 'w') as f:
        text = input()[5:].replace('%0D%0A', '\n')
        f.write(text)
else:
    print("<h1>I only process GET and POST, but you want <\h1>")
    exit()

print(f"""
<form method="post">
    <p><b>Your note:</b></p>
    <p><textarea rows="10" cols="45" name="text">{text}</textarea></p>
    <p><input type="submit" value="Submit"></p>
  </form>
""")
