/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#ifndef downloader_included
#define downloader_included yes

#include "const_str.h"

class downloadArgs
{
public:
	int* downloadingPage;
	ConstStr* downloadedData;
	ConstStr* downloadedHeaders;
	ConstStr* downloadedHTML;
	ConstStr* host;
	ConstStr* path;
	ConstStr* userAgent;
	downloadArgs(int* downloadingPage,
				ConstStr* downloadedData,
				ConstStr* downloadedHeaders,
				ConstStr* downloadedHTML,
				ConstStr* host,
				ConstStr* path,
				ConstStr* userAgent):
		downloadingPage(downloadingPage),
		downloadedData(downloadedData),
		downloadedHeaders(downloadedHeaders),
		downloadedHTML(downloadedHTML),
		host(host),
		path(path),
		userAgent(userAgent){};
};
void* downloadThreadFunc(void* voidStar);

#endif
