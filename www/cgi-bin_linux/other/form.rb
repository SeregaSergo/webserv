#!/usr/bin/env ruby
#
# 3.get.rb
# Copyright (C) 2015 uralbash <root@uralbash.ru>
#
# Distributed under terms of the MIT license.
#

require 'cgi'

cgi = CGI.new

# Get data from fields
first_name = cgi['first_name']
last_name = cgi['last_name']

print "Content-type:text/html\r\n\r\n"
print "<!DOCTYPE html>"
print "<html lang=\"en\">"
print "<html>"
print "<head>"
print "<title>CGI form response</title>"
print "</head>"
print "<body>"
print "<h2>Hello %s %s</h2>" % [first_name, last_name]
print "</body>"
print "</html>"