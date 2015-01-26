class downloadArgs
{
public:
	int* downloadingPage;
	std::string* downloadedData;
	std::string* downloadedHeaders;
	std::string* downloadedHTML;
	std::string* host;
	std::string* path;
	std::string* userAgent;
	downloadArgs(int* downloadingPage,
				std::string* downloadedData,
				std::string* downloadedHeaders,
				std::string* downloadedHTML,
				std::string* host,
				std::string* path,
				std::string* userAgent):
		downloadingPage(downloadingPage),
		downloadedData(downloadedData),
		downloadedHeaders(downloadedHeaders),
		downloadedHTML(downloadedHTML),
		host(host),
		path(path),
		userAgent(userAgent){};
};
void* downloadThreadFunc(void* voidStar);
