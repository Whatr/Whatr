#include "whatr_log_funcs.hpp"

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "whatr_download.h"
#include "whatr_html_lexer.h"
#include "whatr_html_yaccer.h"
#include "whatr_css_lexer.h"
#include "whatr_css_yaccer.h"

void update_screen();

int XRES = 500;
int YRES = 500;
int NPTS = 25000;

//Need this array if we're plotting all at once using XDrawPoints
//XPoint pts[NPTS];

Display* dsp;
Window win;
GC gc;

XTextItem testText;

std::string userAgent = std::string("Whatr development version");

//----------------------------
pthread_t downloadThread;
int downloadingPage = 0;
std::string downloadedData;
std::string downloadedHeaders;
std::string downloadedHTML;
//----------------------------

//----------------------------
pthread_t htmlLexThread;
int lexingPage = 0;
std::vector<HTMLTag> HTMLTags;
std::vector<std::string> headerFields;
std::vector<std::string> headerValues;
//----------------------------

//----------------------------
pthread_t htmlYaccThread;
int yaccingPage = 0;
std::vector<HTMLElement*> HTMLElements;
//----------------------------

//----------------------------
pthread_t cssLexThread;
int lexingCSS = 0;
std::vector<CSSToken> CSSTokens;
//----------------------------

//----------------------------
pthread_t cssYaccThread;
int yaccingCSS = 0;
std::vector<CSSClass*> CSSClasses;
//----------------------------

std::string url, host, path;
void printTree(HTMLElement* currentElement, std::string tabs);
int main(int argc, char* argv[])
{
	if (argc!=2)
	{
		ERROR(One URL argument is required!);
		return 0;
	}
	
	///////////////////////////////////
	////// Get URL from arguments
	{
		url = std::string(argv[1]);
	}
	
	///////////////////////////////////
	////// Check URL validity
	{
		if (url.substr(0, 7)!=std::string("http://"))
		{
			ERROR(The URL is not HTTP!);
			return 0;
		}
		if (url.length()<8)
		{
			ERROR(The URL has no host!);
			return 0;
		}
	}
	
	///////////////////////////////////
	////// Convert URL to host and path
	{
		int i;
		for (i=7;i<url.length();i++)
		{
			if (url.at(i)=='/')
			{
				break;
			}
		}
		host = url.substr(7, i-7);
		path = url.substr(i, url.length()-i);
		if (path.length()==0) path = std::string("/");
	}
	
	
	std::cout << "host=" << host << "\n";
	std::cout << "path=" << path << "\n";
	
	///////////////////////////////////
	////// Start thread that downloads the web page
	{
		downloadingPage = 1;
		downloadedData = std::string("");
		downloadedHeaders = std::string("");
		downloadedHTML = std::string("");
		downloadArgs args(&downloadingPage,
						&downloadedData,
						&downloadedHeaders,
						&downloadedHTML,
						&host,
						&path,
						&userAgent);
		if (pthread_create(&downloadThread, NULL, downloadThreadFunc, &args))
		{
			ERROR(Failed to create download thread!);
			return 0;
		}
	}
	
	for (int i=0;i<20000;i++){__asm("nop");} // TODO figure out why removing or reducing this sometimes causes a segmentation fault and fixing it for good
	
	///////////////////////////////////
	////// Start thread that parses the response headers and lexes the HTML tags
	{
		while (downloadingPage){usleep(10000);}
		
		lexingPage = 1;
		htmlLexArgs args(&downloadingPage,
					&lexingPage,
					&HTMLTags,
					&headerFields,
					&headerValues,
					&downloadedHeaders,
					&downloadedHTML);
		if (pthread_create(&htmlLexThread, NULL, htmlLexThreadFunc, &args))
		{
			ERROR(Failed to create HTML lex thread!);
			return 0;
		}
	}
	PRINT(DOWNLOADED HTML:);
	std::cout << downloadedHTML << "\n";
	///////////////////////////////////
	////// Start thread that yaccs the HTML tags
	{
		while (lexingPage){usleep(10000);}
		yaccingPage = 1;
		htmlYaccArgs args(&lexingPage,
						&yaccingPage,
						&HTMLTags,
						&HTMLElements);
		if (pthread_create(&htmlYaccThread, NULL, htmlYaccThreadFunc, &args))
		{
			ERROR(Failed to create HTML yacc thread!);
			return 0;
		}
	}
	
	///////////////////////////////////
	////// Wait until the downloading is done, then print the data
	{
		while (downloadingPage)
		{
			PRINT(Main thread is waiting for downloadingPage to be 0...);
			usleep(100000);
		};
		//PRINT(DOWNLOADED HEADERS:);
		//std::cout << downloadedHeaders << "\n";
		//PRINT(DOWNLOADED HTML:);
		//std::cout << downloadedHTML << "\n";
		//PRINT(DOWNLOADED DATA:);
		//std::cout << downloadedData << "\n";
	}
	///////////////////////////////////
	////// Wait until the lexing is done, then print the headers and tags
	{
		while (lexingPage)
		{
			PRINT(Main thread is waiting for lexingPage to be 0...);
			usleep(100000);
		};
		/*PRINT(The lexer is done! Here are its results:)
		for (int i=0;i<HTMLTags.size();i++)
		{
			HTMLTag tag = HTMLTags[i];
			if (tag.type==0)
			{
				std::cout << GREEN << "Text node: " << NOCLR << tag.text << "\n";
			}
			else if (tag.type==1)
			{
				std::cout << GREEN << "<" << tag.text;
				for (int j=0;j<tag.argNames.size();j++)
				{
					std::cout << " " << tag.argNames[j] << "="
							  << "\"" << tag.argValues[j] << "\"";
				}
				std::cout << ">\n" << NOCLR;
			}
			else if (tag.type==2)
			{
				std::cout << GREEN << "<" << tag.text;
				for (int j=0;j<tag.argNames.size();j++)
				{
					std::cout << " " << tag.argNames[j] << "="
							  << "\"" << tag.argValues[j] << "\"";
				}
				std::cout << "/>\n" << NOCLR;
			}
			else if (tag.type==3)
			{
				std::cout << GREEN << "</" << tag.text << ">\n" << NOCLR;
			}
			else
			{
				std::cout << RED << "ERROR ERROR FATAL ERROR: Tag with unknown type " << tag.type << NOCLR << "\n";
			}
		}*/
	}
	///////////////////////////////////
	////// Wait until the yaccing is done, then print the HTML element tree
	{
		while (yaccingPage)
		{
			PRINT(waiting for yaccingPage);
			usleep(50000);
		};
		for (int i=0;i<HTMLElements.size();i++)
		{
			HTMLElement* currentElement = HTMLElements.at(i);
			printTree(currentElement, std::string("  "));
		}
	}
	
	///////////////////////////////////
	////// Lex the css
	{
		// TODO make it find all styles
		//							<html>			<head>			<style>
		lexingCSS = 1;
		HTMLElement* style = HTMLElements.at(0)->children.at(0)->children.at(0)->children.at(0);
		cssLexArgs args(&lexingCSS, &CSSTokens, &(style->text));
		if (pthread_create(&cssLexThread, NULL, cssLexThreadFunc, &args))
		{
			ERROR(Failed to create CSS lex thread!);
			return 0;
		}
		while(lexingCSS){};
		PRINT(lexingCSS=0! printing CSSTokens...);
		for (int i=0;i<CSSTokens.size();i++)
		{
			CSSToken t = CSSTokens.at(i);
			std::cout << "CSSTokens[" << i << "]={"
					<< t.type << " , " << t.text << "}\n";
		}
	}
	
	///////////////////////////////////
	////// Yacc the css
	{
		yaccingCSS = 1;
		cssYaccArgs args(&yaccingCSS, &CSSTokens);
		if (pthread_create(&cssYaccThread, NULL, cssYaccThreadFunc, &args))
		{
			ERROR(Failed to create CSS yacc thread!);
			return 0;
		}
		while(yaccingCSS){};
		PRINT(yaccingCSS=0!);
	}
	
	///////////////////////////////////
	////// Create window
	
	/*char testString[] = "blablabla";
	
	testText.chars = testString;
	testText.nchars = strlen(testString);
	testText.delta = 0;
	testText.font = 0;
	
	PRINT(test asdfas);
	ERROR(alksdfjlkajsdflkj lkas jdflka jsdflkjs);
	PRINT(9999*#$);
	
	dsp = XOpenDisplay(NULL);
	
	if (!dsp) return 1; // If XOpenDisplay failed, exit the program

	int screen = DefaultScreen(dsp);
	unsigned int white = WhitePixel(dsp, screen);
	unsigned int black = BlackPixel(dsp, screen);

	win = XCreateSimpleWindow	(	dsp,
									DefaultRootWindow(dsp),
									0, 0,		// origin
									XRES, YRES,	// size
									0, black,	// border width/clr
									white		// backgrd clr
								);
	Atom wmDelete = XInternAtom(dsp, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dsp, win, &wmDelete, 1);
	gc = XCreateGC(	dsp, win,
					0,		// mask of values
					NULL	// array of values
					);

	XSetForeground(dsp, gc, black);

	XEvent evt;
	long eventMask = StructureNotifyMask;
	eventMask |= ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask;
	XSelectInput(dsp, win, eventMask);

	KeyCode keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));

	XMapWindow(dsp, win);

	// Wait until the window has been created...
	do
	{
		XNextEvent(dsp,&evt);
	}
	while (evt.type != MapNotify);
	
	// The window has been created!

	srand(time(0));
	update_screen();

	int loop = 1;
	while (loop)
	{
		XNextEvent(dsp, &evt);
		switch (evt.type)
		{
			case (ButtonRelease) :
			{
				update_screen();
			}
			break;
			case (KeyRelease) :
			{
				if (evt.xkey.keycode == keyQ) loop = 0;
				else update_screen();
			}
			break;
			case (ConfigureNotify) :
			{
				// Check if window has been resized
				if (evt.xconfigure.width != XRES || evt.xconfigure.height != YRES)
				{
					XRES = evt.xconfigure.width;
					YRES = evt.xconfigure.height;
					update_screen();
				}
			}
			break;
			case (ClientMessage) :
			{
				if (evt.xclient.data.l[0] == wmDelete) loop = 0;
			}
			break;
			default :

			break;
		}
	} 

	XDestroyWindow(dsp, win);
	XCloseDisplay(dsp);

	return 0;*/
}
void printTree(HTMLElement* currentElement, std::string tabs)
{
	if (currentElement->type==0)
	{
		std::cout << tabs << "TEXT[" << currentElement->text << "]" << "\n";
	}
	else if (currentElement->children.size()==0)
	{
		std::cout << tabs << "<" << currentElement->text;
		for (int i=0;i<currentElement->argNames.size();i++)
		{
			std::cout << " " << currentElement->argNames.at(i) <<
						"=\"" << currentElement->argValues.at(i) << "\"";
		}
		std::cout << "/>\n";
	}
	else
	{
		std::cout << tabs << "<" << currentElement->text;
		for (int i=0;i<currentElement->argNames.size();i++)
		{
			std::cout << " " << currentElement->argNames.at(i) <<
						"=\"" << currentElement->argValues.at(i) << "\"";
		}
		std::cout << ">\n";
		for (int i=0;i<currentElement->children.size();i++)
		{
			printTree(currentElement->children.at(i), tabs+std::string("  "));
		}
		std::cout << tabs << "</" << currentElement->text << ">\n";
	}
}

void update_screen()
{
	XClearWindow(dsp, win);
	XDrawLine(dsp, win, gc, 0, YRES/2, XRES-1, YRES/2); //from-to
	XDrawLine(dsp, win, gc, XRES/2, 0, XRES/2, YRES-1); //from-to
	XDrawText(dsp, win, gc, 30, 30, &testText, 1);
	//XClearWindow(dsp,win);
	//XDrawPoints(dsp, win, gc, pts, NPTS, CoordModeOrigin);
	return;
}
