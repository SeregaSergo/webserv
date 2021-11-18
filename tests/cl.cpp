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

#define BUF_SIZE    1024
char http_version[] = {"HTTP/1.1"};

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

void GET( std::string url, std::vector< std::pair< std::string, std::string> > requestData )
{
    std::string http = getProtocol(url);
    std::string host = getHost(url);
    std::string port = getPort(url);
    std::string script = getAction(url);
    std::string parm = getParams(requestData);
    std::string header = "";
    char buf[BUF_SIZE];
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
    header += http + host + script + parm;
    header += (std::string)" "+ http_version + "\r\n";
    header += (std::string)"Host: " + http + host + "/" + "\r\n";
    header += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    header += (std::string)"Accept: text/html" + "\r\n";
    header += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    header += (std::string)"Accept-Charset: utf-8;q=0.7,*;q=0.7" + "\r\n";
    header += (std::string)"Connection: keep-alive " + "\r\n";
    header += "\r\n";

    send(sock, &header[0], header.size(), 0);
    int ret = recv(sock, buf, sizeof(buf), 0);

    std::cout << "\nRequest:\n" << header << std::endl;
    std::cout << "\nResponse:\n" << std::string(buf, &buf[ret]) << std::endl;
}

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        std::cerr << "Need two arguments at least" << std::endl;
        return (1);
    }
    if (argc % 2 != 0)
    {
        std::cerr << "Need even number of arguments \
            (url query_var query_val ... query_var query_val" << std::endl;
    }
    std::vector<std::pair< std::string, std::string> > params;
    for (int i = 2; i != argc; i += 2)
        params.push_back(std::make_pair(argv[i], argv[i+1]));
    
    GET(argv[1], params);
    
    return (0);
}