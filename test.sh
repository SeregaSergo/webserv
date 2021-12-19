make client
# method uri num_req sleep_time buf_size var val ...
# ./tests/cl GET http://127.0.0.1:8080/images/light_side.jpg 1 0 1024 name bswag
# ./tests/cl POST http://127.0.0.1:8080/123 1 0 1024 name bswag < tests/files/simple_body.txt
# ./tests/cl POST_chunked http://127.0.0.1:8080/123 1 0 10 name bswag < tests/files/simple_body.txt
# curl -F yo=@./tests/files/simple_body.txt "http://127.0.0.1:8080/dir/" # multipart/form-data
# curl -F yo=@./tests/files/simple_body.txt "http://127.0.0.1:8080/file" # multipart/form-data
# curl --data "param1=test1&param2=test2" http://localhost:8080/dir/   # application/x-www-form-urlencoded
# curl --data "param1=test1&param2=test2" http://localhost:8080/upload/test_file.txt   # application/x-www-form-urlencoded
# curl  -H 'Content-Type: application/json' --data '{"param1":"test1","param2":"test2"}' http://bswag.com:8080/ # application/json
# curl -H "Content-Disposition: attachment, filename=\"a.txt\"" -H "X-Session-ID: simple_body.txt" -F yo=@simple_body.txt "http://127.0.0.1:8080/upload" # multipart/form-data
# curl -X 'DELETE' http://bswag.com:8080/upload/test_file.txt  # different methods
# curl -X 'PUT' -d '{"param1":"test1","param2":"test3"}' http://bswag.com:8080/upload/test_file.txt  # different methods
# curl -w "%{time_total}\n" -o /dev/null -s bswag.com:8080    # time tracing
curl --data "param1=test1&param2=test2" "http://127.0.0.1:8080/python_script.py"