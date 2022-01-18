#!/usr/local/bin/python3
"""Very basic Python script that outputs static HTML content."""

import cgi
import sys
import os
import time

if __name__ == '__main__':
    try:
        # Tell the browser it's an HTML page.
        print('Status: 301 Moved Permanently')
        print('Location: https://datatracker.ietf.org/doc/html/rfc3875#section-6')
        # Blank line to indicate end of headers.
        print('')
    except:
        cgi.print_exception()