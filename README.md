# webserv
HTTP web server (I/O multiplexing, CGI, cookies, sessions)

This project was completed as assignment during learning in school_21
	
Server has only one thread. It uses select syscall for greater performance.
There were implemented only GET, POST, DELETE HTTP methods.
Sessions were represented with session IDs(SID) and temporary files assosiated with it.
More info about config file you can read in file "config_description".

## How to build
```bash
make
```
```bash
make display #with additional info in console
```

## How to run
```bash
./server /path/linux_config
```
