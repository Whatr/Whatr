#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

std::string* httpGET(std::string url, std::vector<std::string>& headersOut);
std::string* httpGET(std::string host, std::string path, std::string userAgent, std::vector<std::string>& headersOut);
std::string userAgent("Whatr 0.0 development version");


int isDigit(char c)
{
	return	c=='0'||
			c=='1'||
			c=='2'||
			c=='3'||
			c=='4'||
			c=='5'||
			c=='6'||
			c=='7'||
			c=='8'||
			c=='9';
}
int main(int argc, char* argv[])
{
	std::vector<std::string> headers;
	std::string* page = httpGET(argv[1], headers);
	for (int i=0;i<headers.size();i++)
	{
		std::cout << headers.at(i) << "\n";
	}
	if (page==NULL) std::cout << "Error in httpGET(..)\n";
	printf("%s", page->c_str());
    return 0;
}
std::string* httpGET(std::string url, std::vector<std::string>& headersOut)
{
	if (url.substr(0, 7)!=std::string("http://")) return NULL;
	if (url.length()<8) return NULL;
	
	int i;
	for (i=7;i<url.length();i++)
	{
		if (url.at(i)=='/')
		{
			break;
		}
	}
	return httpGET(url.substr(7, i-7), url.substr(i, url.length()-i), userAgent, headersOut);
}
std::string* httpGET(std::string host, std::string path, std::string userAgent, std::vector<std::string>& headersOut)
{
	if (path.length()==0) path = std::string("/");
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buffer[256];
    portno = 80;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0)
    {
    	error("Error creating socket");
    }
    else
    {
    	std::cout << "Socket created\n";
    }
    server = gethostbyname(host.c_str());
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        exit(0);
    }
    else
    {
    	std::cout << "Host found\n";
    }
    std::cout << "Zeroing something...\n";
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::cout << "Copying shit to something\n";
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    std::cout << "Connecting...\n";
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error connecting!");
    }
    else std::cout << "Connected!\n";
    std::cout << "Forming HTTP request...\n";
    const char* HTTPgetRequest = (std::string("GET ")+path+std::string(" HTTP/1.1\r\nHost: ")+host+std::string("\r\nUser-Agent: ")+userAgent+std::string("\r\nDNT: 1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nConnection: keep-alive\r\n\r\n")).c_str();
    std::cout << HTTPgetRequest << "\n\n";
    //bzero(buffer, 256);
    //fgets(buffer, 255, stdin);
    std::cout << "Sending HTTP request...\n";
    n = write(sockfd, HTTPgetRequest, strlen(HTTPgetRequest)); //buffer, strlen(buffer));
    std::cout << "HTTP request sent.\n";
    if (n<0)
    {
    	error("ERROR writing to socket");
   	}
   	std::cout << "Zeroing buffer...\n";
    bzero(buffer, 256);
    std::string response;
    
    // Info about response
    int content_length_header = -1;
    int header_length = -1;
    
    std::cout << "Reading response...\n";
    while
    (
    	( // Stop reading when according to the responses's Content-Length all
    	  // bytes have been read
    		content_length_header==-1
    		||
    		response.size()<(content_length_header+header_length)
    	)
    	&&
    	( // Stop reading when read returns 0
    		n=read(sockfd, buffer, 255)
    	)	>0
    )
    {
    	std::cout << n << " bytes read...\n";
    	if (header_length==-1)
    	{
			if (content_length_header==-1)
			{
				std::size_t found = response.find(std::string("Content-Length: "));
				if (found!=std::string::npos)
				{
					std::string tempContentLength = response.substr(found+16, 10);
					for (int i=0;i<tempContentLength.size();i++)
					{
						if (isDigit(tempContentLength.at(i))) continue;
						else tempContentLength = tempContentLength.substr(0, i);
					}
					std::cout << "Content length =" << tempContentLength << "=\n\n";
					content_length_header = atoi(tempContentLength.c_str());
				}
			}
			std::size_t found = response.find(std::string("\r\n\r\n"));
			if (found!=std::string::npos)
			{
				header_length = found+4;
				std::cout << "Header length =" << header_length << "=\n\n";
			}
    	}
    	if (n<255)
    	{
    		buffer[n] = 0;
    	}
    	response.append(buffer);
    	std::cout << n << " bytes written to buffer!\n";
    	//printf("n=%i\n%s\n", n, buffer);
    }
    //headerLengthOut = header_length;
    
    int i = 0;
    std::string line = std::string("");
    while (i<header_length)
    {
    	if (response.at(i+0)=='\r' &&
    		response.at(i+1)=='\n' &&
    		response.at(i+2)=='\r' &&
    		response.at(i+3)=='\n')
    	{
    		headersOut.push_back(line);
    		break;
    	}
    	if (response.at(i+0)=='\r' &&
    		response.at(i+1)=='\n')
    	{
    		if (headersOut.size()>0) headersOut.push_back(line);
    		line = std::string("");
    		i += 2;
    	}
    	line.append(response.substr(i, 1));
    	i++;
    }
    
    std::cout << "Reading done.\n";
    close(sockfd);
    std::cout << "Socket closed.\n";
    std::cout << response.size() << " bytes response.\n";
    return new std::string(response);
}

