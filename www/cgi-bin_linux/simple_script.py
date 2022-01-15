#!/usr/bin/python3
"""Very basic Python script that outputs static HTML content."""

import cgi
import sys
import os
import time

def template():
    """Returns the a formatted templated. This can be placed in an HTML file."""
    templ = """
<html>
<title>{title}</title>
<body>
  <p>Hello, {username}!
  I read {size} symbols from stdin!</p>
  <h4>Environment variables:</h4>
  {env}
</body>
</html>
            """
    return templ


def render(**kwargs):
    content = template().format(**kwargs)
    print(content)

def listToString(l): 
    
    # initialize an empty string
    str1 = "" 
    
    # traverse in the string  
    for tuple in l: 
        str1 += tuple[0] + " = " + tuple[1] + "<br>"
    
    # return string  
    return str1 

def run():
    username = 'Bswag'
    title = 'My first tinyPy.cgi'
    input_str = sys.stdin.read(-1)
    size = len(input_str)
    env = listToString(list(os.environ.items()))
    render(**locals())


if __name__ == '__main__':
    try:
        # Tell the browser it's an HTML page.
        print('Content-Type: text/html')
        # Blank line to indicate end of headers.
        print('')
        # Main program.
        run()
    except:
        cgi.print_exception()
