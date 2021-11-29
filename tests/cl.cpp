#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sstream>
#include <iterator>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

char http_version[] = {"HTTP/1.1"};

unsigned long long buf_size_g  = 1024;
unsigned long time_to_sleep_g = 0;

std::string getProtocol( std::string url )
{
    std::string protocol = "";
    unsigned long i = 0;

    for(i = 0; i < url.size(); i++)
    {
        if ( url[i] != '/' || url[i+1] != '/'  )
            protocol += url[i];
        else
        {
            protocol += "//";
            break;
        }
    }
    return protocol;
}

std::string getHost( std::string url )
{
    std::string host = "";

    url.replace(0, getProtocol(url).size(), "");

    unsigned long i = 0;
    for(i = 0; i < url.size(); i++)
    {
        if( url[i] != '/' && url[i] != ':')
            host += url[i];
        else
            break;
    }

    return host;
}

std::string getPort( std::string url )
{
    std::string port = "";

    url.replace(0, getProtocol(url).size()+getHost(url).size(), "");

    unsigned long i;
    if (url[0] == ':')
        i = 1;
    else
        i = 0;
    for(; i < url.size(); i++)
    {
        if( url[i] != '/' )
            port += url[i];
        else
            break;
    }

    if (port.empty())
        port = "80";

    return port;
}

std::string getAction( std::string url )
{
    std::string parm = "";
    std::string port = getPort(url);
    int port_size = port.empty() ? 0 : port.size() + 1;
    url.replace(0, getProtocol(url).size()+getHost(url).size()+ port_size, "");

    unsigned long i = 0;
    for(i = 0; i < url.size(); i++)
    {
        if( url[i] != '?' && url[i] != '#' )
            parm += url[i];
        else
            break;
    }

    return parm;
}

std::string getParams( std::vector< std::pair< std::string, std::string> > requestData )
{
    std::string parm = "";

    std::vector< std::pair< std::string, std::string> >::iterator itr = requestData.begin();

    for( ; itr != requestData.end(); ++itr )
    {
        if( parm.size() < 1 )
            parm += "";
        else
            parm += "&";
        parm += itr->first + "=" + itr->second;
    }
    if (!parm.empty())
        parm = "?" + parm;
    return parm;
}

void GET(std::string url, std::vector< std::pair< std::string, std::string> > requestData, int num_req)
{
    std::string http = getProtocol(url);
    std::string host = getHost(url);
    std::string port = getPort(url);
    std::string script = getAction(url);
    std::string parm = getParams(requestData);
    std::string header = "";
    char buf[buf_size_g];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    struct hostent* raw_host = gethostbyname( host.c_str() );
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port.c_str()));

    if (raw_host == NULL) {
        std::cout << "ERROR, no such host: " << host.c_str();
        exit(0);
    }
    bcopy( (char*)raw_host->h_addr, (char*)&addr.sin_addr, raw_host->h_length );
    if (connect( sock, (struct sockaddr *)&addr, sizeof(addr) ) < 0) {
        std::cerr << "connect error" <<std::endl;
        exit(2);
    }
    
    header += "GET ";
    header += script;
    header += (std::string)" "+ http_version + "\r\n";
    header += (std::string)"Host: " + http + host + "/" + "\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    header += (std::string)"Accept-Charset: utf-8;q=0.7,*;q=0.7" + "\r\n";
    if (num_req == 1)
        header += (std::string)"Connection: close " + "\r\n";
    else
        header += (std::string)"Connection: keep-alive " + "\r\n";
    header += "\r\n";

    std::cout << "\nRequest:\n" << header << std::endl;

    for (; num_req > 0; --num_req)
    {
        int size_to_send;
        std::string massage(header);
        while (massage.size())
        {
            size_to_send = massage.size() > buf_size_g ? buf_size_g : massage.size();
            send(sock, &massage[0], size_to_send, 0);
            massage.erase(0, size_to_send);
            usleep(time_to_sleep_g);
        }

        std::cout << "\nResponse(" << num_req << "):" << std::endl;
        int ret = 1;
        while (ret > 0)
        {
            ret = recv(sock, buf, sizeof(buf), 0);
            std::cout << std::string(buf, &buf[ret]) << std::endl;
        }
    }
}

void POST(std::string url, int num_req, bool chunked)
{
    std::string http = getProtocol(url);
    std::string host = getHost(url);
    std::string port = getPort(url);
    std::string script = getAction(url);
    std::string header = "";
    std::cin >> std::noskipws;
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string body(it, end);
    char buf[buf_size_g];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    struct hostent* raw_host = gethostbyname( host.c_str() );
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port.c_str()));
    int ret = snprintf(buf, sizeof(buf), "%lu", body.size());

    if (raw_host == NULL) {
        std::cout << "ERROR, no such host: " << host.c_str();
        exit(0);
    }
    bcopy( (char*)raw_host->h_addr, (char*)&addr.sin_addr, raw_host->h_length );
    if (connect( sock, (struct sockaddr *)&addr, sizeof(addr) ) < 0) {
        std::cerr << "connect error" <<std::endl;
        exit(2);
    }
    
    header += "POST ";
    header += script;
    header += (std::string)" "+ http_version + "\r\n";
    header += (std::string)"Host: " + http + host + "/" + "\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    header += (std::string)"Accept-Charset: utf-8;q=0.7,*;q=0.7" + "\r\n";
    if (num_req == 1)
        header += (std::string)"Connection: close " + "\r\n";
    else
        header += (std::string)"Connection: keep-alive " + "\r\n";
    if (chunked)
    {
        header += (std::string)"Transfer-Encoding: chunked\r\n\r\n";
        int size_to_send;
        while (true)
        {
            size_to_send = body.size() > buf_size_g ? buf_size_g : body.size();
            std::stringstream ss;
            ss << std::hex << size_to_send;
            if (size_to_send)
            {
                header += ss.str() + "\r\n" + std::string(&body[0], &body[size_to_send]) + "\r\n";
                body.erase(0, size_to_send);
            }
            else
            {
                header += ss.str() + "\r\n";
                break;
            }
        }
    }
    else
    {
        header += (std::string)"Content-Length: " + std::string(buf, &buf[ret]) + "\r\n\r\n";
        header += body;
    }
    
    std::cout << "\nRequest:\n" << header << std::endl;

    for (; num_req > 0; --num_req)
    {
        while (header.size())
        {
            int size_to_send = header.size() > buf_size_g ? buf_size_g : header.size();
            send(sock, &header[0], size_to_send, 0);
            header.erase(0, size_to_send);
            usleep(time_to_sleep_g);
        }

        std::cout << "\nResponse(" << num_req << "):" << std::endl;
        ret = 1;

        fcntl(sock, F_SETFL, O_NONBLOCK);
        int num_empty = 0;
        while (ret != 0)
        {
            ret = recv(sock, buf, sizeof(buf), 0);
            if (ret == -1)
            {
                if (errno == EWOULDBLOCK) {
                    ++num_empty;
                    if (num_empty == 6)
                        break;
                    usleep(30000);
                    continue;
                }
                else
                    break;
            }
            num_empty = 0;
            std::cout << std::string(buf, &buf[ret]);
        }
        std::cout << std::endl;
    }
}

int main(int argc, char ** argv)
{
    if (argc < 6)
    {
        std::cerr << "Need 5 arguments at least:\n \
        method uri num_req time_to_send buf_size" << std::endl;
        return (1);
    }
    if (argc % 2 != 0)
    {
        std::cerr << "Need even number of arguments \
            (url query_var query_val ... query_var query_val" << std::endl;
    }
    std::vector<std::pair< std::string, std::string> > params;
    for (int i = 6; i != argc; i += 2)
        params.push_back(std::make_pair(argv[i], argv[i+1]));
    
    int num_req = atoi(argv[3]);
    time_to_sleep_g = (unsigned int)atoi(argv[4]);
    buf_size_g = (unsigned long long)atoll(argv[5]);

    if (!strcmp(argv[1],"GET"))
        GET(argv[2], params, num_req);
    if (!strcmp(argv[1],"POST_chunked"))
        POST(argv[2], num_req, true);
    if (!strcmp(argv[1],"POST"))
        POST(argv[2], num_req, false);
    // if (!strcmp(argv[1],"DELETE")
        // DELETE(argv[2], )
    return (0);
}