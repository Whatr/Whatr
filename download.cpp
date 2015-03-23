/* Copyright (C) 2015 Jesse Busman
/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* (at your option) any later version.
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/* GNU General Public License for more details.
/* You should have received a copy of the GNU General Public License
/* along with this program.  If not, see <http://www.gnu.org/licenses/>  */

#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "log_funcs.hpp"
#include "download.h"

int isDigit(char c)
{
	return	c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||
			c=='5'||c=='6'||c=='7'||c=='8'||c=='9';
}
void* downloadThreadFunc(void* args)
{
	PRINT(downloadThreadFunc start);
	downloadArgs* d = (downloadArgs*)args;
	
	int* downloadingPage = d->downloadingPage;
	std::string* downloadedData = d->downloadedData;
	std::string* downloadedHeaders = d->downloadedHeaders;
	std::string* downloadedHTML = d->downloadedHTML;
	std::string* host = d->host;
	std::string* path = d->path;
	std::string* userAgent = d->userAgent;
	
    PRINT(downloader thread Checkpoint 000);
	
	*downloadingPage = 1;
	
	//std::cout << downloadingPage << "\n\n";
		
	PRINT(downloader thread has set downloadingPage=1);
	std::cout << "downloadedData=\"" << *downloadedData << "\"\n";
	*downloadedData = std::string("");
    PRINT(downloader thread Checkpoint 000.01);
	*downloadedHeaders = std::string("");
    PRINT(downloader thread Checkpoint 000.02);
	*downloadedHTML = std::string("");
    PRINT(downloader thread Checkpoint 000.1);
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buffer[256];
    PRINT(downloader thread Checkpoint 000.2);
    portno = 80;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    PRINT(downloader thread Checkpoint 001);
    if (sockfd<0)
    {
    	ERROR(Error creating socket!);
    	pthread_exit(NULL);
    	return NULL;
    }
    else
    {
    	PRINT(Socket created);
    }
    server = gethostbyname(host->c_str());
    if (server == NULL)
    {
        ERROR(No such host!);
    	pthread_exit(NULL);
    	return NULL;
    }
    else
    {
    	std::cout << GREEN << "Host " << *host << " found\n" << NOCLR;
    }
    PRINT(downloader thread Checkpoint 002);
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        ERROR(Could not connect!);
    	pthread_exit(NULL);
    	return NULL;
    }
    const char* HTTPgetRequest =
    (
		std::string("GET ")+*path+std::string(" HTTP/1.1\r\n")+
		std::string("Host: ")+*host+std::string("\r\n")+
		std::string("User-Agent: ")+*userAgent+std::string("\r\n")+
		std::string("DNT: 1\r\n")+
		std::string("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n")+
		std::string("Connection: keep-alive\r\n\r\n")
    ).c_str();
    PRINT(downloader thread Checkpoint 003);
    //std::cout << "Sending HTTP request:\n" << HTTPgetRequest << "\n\n";
    n = write(sockfd, HTTPgetRequest, strlen(HTTPgetRequest)); //buffer, strlen(buffer));
    //std::cout << "HTTP request sent.\n";
    if (n<0)
    {
    	ERROR(Could not write to socket!);
    	pthread_exit(NULL);
    	return NULL;
   	}
   	//std::cout << "Zeroing buffer...\n";
    bzero(buffer, 256);
    
    // Info about response
    int content_length_header = -1;
    int header_length = -1;
    PRINT(downloader thread Checkpoint 004);
    
    //std::cout << "Reading response...\n";
    while
    (
    	( // Stop reading when according to the responses's Content-Length header
    	  // all bytes have been read
    		content_length_header==-1
    		||
    		header_length==-1
    		||
    		downloadedData->size()<(content_length_header+header_length)
    	)
    	&&
    	(
    		( // Stop reading when read returns 0
    			n=read(sockfd, buffer, 255)
    		)
    		>
    		0
    	)
    )
    {
    	int appendedHTML = 0;
    	std::cout << n << " bytes read (total=" << downloadedData->size() << ") Content-Length: " << content_length_header << "\n";
    	if (n<255)
    	{
    		buffer[n] = 0;
    	}
    	downloadedData->append(buffer);
    	if (header_length==-1)
    	{
			if (content_length_header==-1)
			{
				std::size_t found = downloadedData->find(std::string("Content-Length: "));
				if (found!=std::string::npos)
				{
					std::string tempContentLength = downloadedData->substr(found+16, 10);
					for (int i=0;i<tempContentLength.size();i++)
					{
						if (isDigit(tempContentLength.at(i))) continue;
						else tempContentLength = tempContentLength.substr(0, i);
					}
					//std::cout << "Content length =" << tempContentLength << "=\n\n";
					content_length_header = atoi(tempContentLength.c_str());
				}
			}
			std::size_t found = downloadedData->find(std::string("\r\n\r\n"));
			if (found!=std::string::npos)
			{
				header_length = found+4;
				//std::cout << "Header length =" << header_length << "=\n\n";
    			*downloadedHeaders = downloadedData->substr(0, header_length);
    			downloadedHTML->append(downloadedData->substr(header_length));
    			appendedHTML = 1;
			}
			//else std::cout << "Could not find header length.\n";
    	}
    	if (header_length!=-1 && !appendedHTML)
    	{
    		downloadedHTML->append(buffer);
		} 
    	std::cout << n << " bytes written to buffer!\n";
    }
    
    //std::cout << "Reading done.\n";
    close(sockfd);
    //std::cout << "Socket closed.\n";
    //std::cout << downloadedData->size() << " bytes response.\n";
    
    *downloadingPage = 0;
	PRINT(downloadThreadFunc end);
	pthread_exit(NULL);
}

