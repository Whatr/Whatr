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
#include "const_str.h"
#include "download.h"
#include "settings.h"
#include <unistd.h>
#include <fcntl.h>

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
	ConstStr* downloadedData = d->downloadedData;
	ConstStr* downloadedHeaders = d->downloadedHeaders;
	ConstStr* downloadedHTML = d->downloadedHTML;
	ConstStr* host = d->host;
	ConstStr* path = d->path;
	ConstStr* userAgent = d->userAgent;
	
	PRINT(downloadThreadFunc start);
	
	*downloadingPage = 1;
	
	PRINT(downloader thread has set downloadingPage=1);
	
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent* server;
	char buffer[256];
	portno = 80;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
	host->printLine();
	char* hostStr = host->copy();
	PRINT(checkey);
	server = gethostbyname(hostStr);
	if (server == NULL)
	{
	    ERROR(No such host!);
		pthread_exit(NULL);
		exit(0);
		return NULL;
	}
	else
	{
		std::cout << GREEN << "Host ";
		host->print();
		std::cout << " found\n" << NOCLR;
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
		std::string("GET ")+std::string(path->copy())+std::string(" HTTP/1.1\r\n")+
		std::string("Host: ")+std::string(host->copy())+std::string("\r\n")+
		std::string("User-Agent: ")+userAgent->copy()+std::string("\r\n")+
		std::string("DNT: 1\r\n")+
		std::string("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n")+
		std::string("Connection: keep-alive\r\n\r\n")
	).c_str();
	PRINT(downloader thread Checkpoint 003);
	n = write(sockfd, HTTPgetRequest, strlen(HTTPgetRequest));
	if (n<0)
	{
		ERROR(Could not write to socket!);
		pthread_exit(NULL);
		return NULL;
   	}
	bzero(buffer, 256);
	
	char** currentBlock = downloadedData->startBlock;
	
	// Info about response
	int content_length_header = -1;
	int header_length = -1;
	PRINT(downloader thread Checkpoint 004);
	
	int downloadedHTMLStartPos = 0;
	
	while
	(
		( // Stop reading when according to the responses's Content-Length header
		  // all bytes have been read
			content_length_header==-1
			||
			header_length==-1
			||
			downloadedData->length < (content_length_header+header_length)
		)
		&&
		(
			( // Stop reading when read returns 0
				n
				=
				recv
				(
					sockfd
					,
					currentBlock++[0]=(char*)malloc(BLOCK_SIZE) // lol
					,
					BLOCK_SIZE
					,
					0
				)
			)
			>
			0
		) // https://www.youtube.com/watch?v=JKQwgpaLR6o
	)
	{
		int nn = -1;
		if (n>0 && n<BLOCK_SIZE) // If recv returns a partial block
		{
			readAgain:
			usleep(100); // Wait 0.1 ms

			// Try to read the rest:

			// Test if the socket is in non-blocking mode:
			if (fcntl(sockfd, F_GETFL) & O_NONBLOCK)
			{
				// socket is non-blocking
			}

			// Put the socket in non-blocking mode:
			if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK) < 0)
			{
				std::cout << RED << "handle error\n" << NOCLR;
			}
			char buf;
			try
			{
				int err = recv(sockfd, &buf, 1, MSG_PEEK);
				if (err <= -1) break;
			}
			catch (int eee)
			{
				printf("Exception=%i\n", eee);
			}
			try
			{
				nn = recv(sockfd, (currentBlock-1)[0]+n, BLOCK_SIZE-n, MSG_WAITALL);
			}
			catch (int eee)
			{
				printf("Exception=%i\n", eee);
			}
			if (nn==BLOCK_SIZE-n) // Success :)
			{
				n = BLOCK_SIZE; // Pretend as if there was no trouble at all
			}
			else if (nn!=0) // Partial success :(
			{
				n += nn; // Pretend as if this amount had been read in the first place
				goto readAgain; // Try again
			}
		}
		if (downloadedData->length==0)
		{
			downloadedData->startChar = currentBlock[-1];
		}
		downloadedData->length += n;
		int appendedHTML = 0;
		std::cout << n << " bytes read (total=" << downloadedData->length << ") Content-Length: " << content_length_header << "\n";
		if (header_length==-1)
		{
			if (content_length_header==-1)
			{
				std::size_t found = downloadedData->findReverse("Content-Length: ");
				if (found!=std::string::npos)
				{
					char* tempContentLength = downloadedData->subString(found+16, 10).copy();
					for (int i=0;tempContentLength[i]!=0;i++)
					{
						if (isDigit(tempContentLength[i])) continue;
						else tempContentLength[i] = 0;
					}
					content_length_header = atoi(tempContentLength);
					
				}
			}
			std::size_t found = downloadedData->findReverse("\r\n\r\n");
			if (found!=std::string::npos)
			{
				header_length = found+4;
				*downloadedHeaders = downloadedData->subString(0, header_length);
				downloadedHTMLStartPos = found+4;
				appendedHTML = 1;
			}
		}
		std::cout << n << " bytes written to buffer!\n";
	}
	std::cout << "Reading done.\n";
	close(sockfd);
	std::cout << "Socket closed.\n";
	PRINT(-------------------------------------------------------);
	std::cout << downloadedData->length << " bytes response:\n";
	downloadedData->printLine();
	PRINT(-------------------------------------------------------);
	std::cout << downloadedHeaders->length << " bytes of header:\n";
	downloadedHeaders->printLine();
	PRINT(-------------------------------------------------------);
	*downloadedHTML = downloadedData->subString(downloadedHeaders->length);
	std::cout << downloadedHTML->length << " bytes of HTML:\n";
	downloadedHTML->printLine();
	PRINT(-------------------------------------------------------);
	PRINT(downloadThreadFunc end);
	*downloadingPage = 0;
	pthread_exit(NULL);
}

