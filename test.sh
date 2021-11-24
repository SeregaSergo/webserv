make client
# method uri num_req sleep_time buf_size var val ...
# ./tests/cl GET http://127.0.0.1:8080/123 1 0 1024 name bswag
# ./tests/cl POST http://127.0.0.1:8080/123 1 0 1024 name bswag < tests/files/simple_body.txt
./tests/cl POST_chunked http://127.0.0.1:8080/123 1 0 10 name bswag < tests/files/simple_body.txt