# webserv
HTTP(S) web server (I/O multiplexing, CGI, cookies, sessions)

### Config file description (example)

> Path to error log file (default: "/dev/null") <

`error_log /logs/error.log;`

* Switch on daemon mode (default: non-daemon mode)

`daemon_mode;`

If after this time the client does not respond,
then it is deleted (default: 10 sec)

`idle_timeout 5;`

Timeout for keep-alive connections. Server will close connections after this time
and after responding to the current request (default: 86400 sec)

`keepalive_timeout 30;`

> Keep-alive parameters. Optional.
> You can use one of them (default settings of your OS).
> https://tldp.org/HOWTO/TCP-Keepalive-HOWTO/overview.html
tcp_keepalive_time 300;        > after this time TCP will check connection (sec)
tcp_keepalive_probes 3;        > how many probes will be sended to check connection
tcp_keepalive_intvl 20;        > time iterval between two probes (sec)

> URI limit -> 414 Request-URI Too Long (default: 1024 bytes)
limit_uri_length  1024;

> Request headers limit -> 413 Request Entity Too Large (default: 4096)
limit_request_entity 4096;

> MIME types depending on file extentions.
> If there is no extention: "application/octet-stream".
types {
  text/html                             html htm;
  text/css                              css;
  image/jpeg                            jpeg jpg;
  application/zip                       zip;
  application/octet-stream              deb bin exe dll dmg eot iso img;
  audio/mpeg                            mp3;
  video/mpeg                            mpeg mpg mov avi;
};

> Virtual server configuration.
> There may be several of these, at least one.
server {
  > "Host" http header field (optional)
	server_name suzumaki.org www.suzumaki.org;
  
  > IP is optional (default: ANY), port is mandatory
  listen 127.0.0.0:8080;
  
  > Path to access log file (default: "/dev/null") 
  access_log  logs/access.log;
  
  > HTTP request body limit (default: 100 Mb)
  > You can use B(bytes), K(kbs), M, G
  client_max_body_size 5M;
  
  > Default root for every location (optional)
  root /Users/bswag/Desktop/;
  
  > Error pages uri
  error_page 500 503 504 /custom_50x.html;
  error_page 404 /custom_404.html;
  
  > Locations for resourse serching, could be several, at least one.
  > There are 3 types of location with different priorities (from high to low):
  > 1. = means an exact match of uri and location path
  > 2. ~ search by extension
  > 3.   partial match of path (searching starts from begining)
  location / {
      root      /data/www/;                   # works as alias in nginx
      index     index.html index.php;         # gives one of these files if location ends with "/"
  };

  location /fetch/ {
      allowed_methods   GET;                  # allowed methods (as default: allow everything)
      autoindex     on;                       # https://www.keycdn.com/support/nginx-directory-index
                                              # do something only if uri ends on '/' and can't find index file
  };

  location = /index.php {
      root /data/www/suzumaki/index.php;
  };
  
  location ~ php {
      root /home/www/cgi-bin/;
      cgi_interpreter /usr/bin/php-cgi;       # Path to cgi interpreter
      cgi_timeout 5;                          # Stop process after this time (default: 5 sec)
  };
    
  location ~ html/jpg {
      root /data/www/suzumaki/static/;
      allowed_methods   GET;
  };
  
};

server {

  listen 127.0.0.0:8080;
  server_name bswag.ru www.bswag.ru;
  
  location / {
        redirect 301 http://suzumaki.org$uri;    # redirections with parsing
  };
};
