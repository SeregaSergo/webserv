#!/usr/local/bin/python3
"""Very basic Python script that outputs static HTML content."""

import cgi
import sys
import os
import time

if __name__ == '__main__':
    try:
        # Tell the browser it's an HTML page.
        print('Location: /images/light_side.jpg')
        # Blank line to indicate end of headers.
        print('')
    except:
        cgi.print_exception()