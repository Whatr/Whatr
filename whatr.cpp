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

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>

#include "const_str.h"
#include "log_funcs.hpp"
#include "download.h"
#include "html_lexer.h"
#include "html_yaccer.h"
#include "css_lexer.h"
#include "css_yaccer.h"
#include "css_applyer.h"
#include "css_selector.h"
#include "renderer_1.h"
#include "css_values.h"
#include "const_str.h"
#include "settings.h"

void update_screen();

void printCSSValue(CSSValue val)
{
	std::cout << "("	<< val.text << ","
						<< val.length << ","
						<< val.color << ","
						<< val.time << ")=("
						<< val.textValue << ","
						<< val.lengthValue << ","
						<< std::setw(8)
						<< std::hex
						<< std::setfill('0')
						<< val.colorValue << ","
						<< std::setw(0)
						<< std::dec
						<< std::setfill(' ')
						<< val.timeValue << ")";
}

int XRES = 500;
int YRES = 500;
int NPTS = 25000;

//Need this array if we're plotting all at once using XDrawPoints
//XPoint pts[NPTS];

Display* dsp;
Window win;
GC gc;

XTextItem testText;

ConstStr userAgent("Whatr development version");

//----------------------------
pthread_t downloadThread;
int downloadingPage = 0;
int bytesDownloaded = 0;
char* blocks[MAX_BLOCKS];
ConstStr downloadedData;
ConstStr downloadedHeaders;
ConstStr downloadedHTML;
//----------------------------

//----------------------------
pthread_t htmlLexThread;
int lexingPage = 0;
std::vector<HTMLTag> HTMLTags;
std::vector<ConstStr> headerFields;
std::vector<ConstStr> headerValues;
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
std::vector<CSSClass> CSSClasses;
//----------------------------

//----------------------------
pthread_t cssApplyThread;
int applyingCSS = 0;
//----------------------------

//----------------------------
pthread_t renderer1Thread;
int rendering1 = 0;
//----------------------------

ConstStr url, host, path;
void printTree(HTMLElement* currentElement, std::string tabs);
int main(int argc, char* argv[])
{
	try{
	
	printf("main():0\n");
	
	if (argc!=2)
	{
		ERROR(One URL argument is required!);
		return 0;
	}
	
	auto time_1 = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Get URL from arguments
	{
		url = std::string(argv[1]);
	}
	
	printf("main():1\n");
	
	///////////////////////////////////
	////// Check URL validity
	{
		if (url.subString(0, 7)!=std::string("http://"))
		{
			ERROR(The URL is not HTTP!);
			return 0;
		}
		if (url.length<8)
		{
			ERROR(The URL has no host!);
			return 0;
		}
	}
	
	printf("main():2\n");
	
	///////////////////////////////////
	////// Convert URL to host and path
	{
		int i;
		for (i=7;i<url.length;i++)
		{
			if (url[i]=='/')
			{
				break;
			}
		}
		host = url.subString(7, i-7);
		path = url.subString(i);//, url.length-i);
		if (path.length==0) path = std::string("/");
	}
	
	//host.printLine();
	
	printf("main():3\n");
	
	auto time_2 = std::chrono::high_resolution_clock::now();
	
	int aaa = 0;
	
	///////////////////////////////////
	////// Start thread that downloads the web page
	{
		downloadingPage = 1;
		
		downloadedData.startBlock = &blocks[0];
		downloadedData.startChar = NULL;
		downloadedData.length = 0;
		
		downloadArgs* args = new downloadArgs
								(&downloadingPage,
								&downloadedData,
								&downloadedHeaders,
								&downloadedHTML,
								&host,
								&path,
								&userAgent);
		
		if (pthread_create(&downloadThread, NULL, downloadThreadFunc, args))
		{
			ERROR(Failed to create download thread!);
			return 0;
		}
	}
	
	auto time_3 = std::chrono::high_resolution_clock::now();
	while (downloadingPage){usleep(100);}
	
	//PRINT(DOWNLOADED HTML:);
	//downloadedHTML.printLine();
	
	/*auto time_3b = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Start thread that parses the response headers and lexes the HTML tags
	{
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
	
	auto time_4 = std::chrono::high_resolution_clock::now();
	while (lexingPage){}
	usleep(100000);
	auto time_4b = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Wait until the lexing is done, then print the headers and tags
	{
		while (lexingPage){};
		PRINT(The HTML lexer is done! Here are its results:)
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
		}
	}
	///////////////////////////////////
	////// Start thread that yaccs the HTML tags
	{
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
	
	auto time_5 = std::chrono::high_resolution_clock::now();
	///////////////////////////////////
	////// Wait until the yaccing is done, then print the HTML element tree
	{
		while (yaccingPage){};
		for (int i=0;i<HTMLElements.size();i++)
		{
			HTMLElement* currentElement = HTMLElements.at(i);
			printTree(currentElement, std::string("  "));
		}
	}
	
	auto time_6 = std::chrono::high_resolution_clock::now();
	
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
	
	auto time_7 = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Yacc the css
	{
		yaccingCSS = 1;
		cssYaccArgs args(&yaccingCSS, &CSSTokens, &CSSClasses);
		if (pthread_create(&cssYaccThread, NULL, cssYaccThreadFunc, &args))
		{
			ERROR(Failed to create CSS yacc thread!);
			return 0;
		}
		while(yaccingCSS){};
		PRINT(yaccingCSS=0! printing CSS classes...);
		for (int i=0;i<CSSClasses.size();i++)
		{
			std::cout << "--- Class selector:\n";
			for (int j=0;j<CSSClasses.at(i).selector.subSelectors.size();j++)
			{
				CSSSubSelector ss = CSSClasses.at(i).selector.subSelectors.at(j);
				std::cout << ss.str1 << " " << ss.str2 << " " << ss.type << "\n";
			}
			std::cout << "--- Class rules:\n";
			for (int j=0;j<CSSClasses.at(i).ruleNames.size();j++)
			{
				std::cout << CSSClasses.at(i).ruleNames.at(j) << ": ";
				printCSSValue(CSSClasses.at(i).ruleValues.at(j));
				std::cout << "\n";
			}
		}
	}
	
	auto time_8 = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Apply the css
	{
		for (int i=0;i<HTMLElements.size();i++)
		{
			applyingCSS = 1;
			cssApplyArgs args(&applyingCSS, &CSSClasses, HTMLElements.at(i));
			if (pthread_create(&cssApplyThread, NULL, cssApplyThreadFunc, &args))
			{
				ERROR(Failed to create CSS yacc thread!);
				return 0;
			}
			while(applyingCSS){};
			printTree(HTMLElements.at(i), std::string("  "));
		}
	}
	
	auto time_9 = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Renderer 1: HTML Transform
	{
		rendering1 = 1;
		renderer1Args args(&rendering1, &HTMLElements);
		if (pthread_create(&renderer1Thread, NULL, renderer1ThreadFunc, &args))
		{
			ERROR(Failed to create renderer 1 thread!);
			return 0;
		}
		while(rendering1){};
		printTree(HTMLElements.at(0), std::string("  "));
	}
	
	auto time_10 = std::chrono::high_resolution_clock::now();
	
	auto time1 = time_2 - time_1;
	auto time2 = time_3 - time_2;
	auto time3a = time_3b - time_3;
	auto time3b = time_4 - time_3b;
	auto time4a = time_4b - time_4;
	auto time4b = time_5 - time_4b;
	auto time5 = time_6 - time_5;
	auto time6 = time_7 - time_6;
	auto time7 = time_8 - time_7;
	auto time8 = time_9 - time_8;
	auto time9 = time_10 - time_9;
	
	std::cout << "\n\n##### Slowness report:\n";
	std::cout <<"Parse URL: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time1).count()<<"us\n";
	std::cout<<"Start download thread: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time2).count()<<"us\n";
	std::cout<<"Download page: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time3a).count()<<"us\n";
	std::cout<<"Start lex thread: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time3b).count()<<"us\n";
	std::cout<<"Lex html: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time4a).count()<<"us\n";
	std::cout<<"Start yacc html thread: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time4b).count()<<"us\n";
	std::cout<<"Yacc html: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time5).count()<<"us\n";
	std::cout<<"Lex css: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time6).count()<<"us\n";
	std::cout<<"Yacc css: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time7).count()<<"us\n";
	std::cout<<"Apply css: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time8).count()<<"us\n";
	std::cout<<"Renderer 1: "
	<<std::chrono::duration_cast<std::chrono::microseconds>(time9).count()<<"us\n";
	
	auto total = time1+time2+time3a+time3b+time4a+time4b+time5+time6+time7+time8+time9;
	
	std::cout << "##### Total time taken: "<<std::chrono::duration_cast<std::chrono::microseconds>(total).count()<<"us\n";
	std::cout << "##### Total time taken excluding download: "<<std::chrono::duration_cast<std::chrono::microseconds>(total-time2-time3a).count()<<"us\n";
	*/
	
	///////////////////////////////////
	////// Create window
	
	/*
	char testString[] = "blablabla";
	
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
	*/
	
	}
	catch(int exex)
	{
		printf("exception=%i=%s\n", exex, (char*)(&exex));
	}
	
	return 0;
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
		std::cout << " computedStyle=\"";
		for (int i=0;i<currentElement->styleFields.size();i++)
		{
			std::cout << currentElement->styleFields.at(i) << ":";
			printCSSValue(currentElement->styleValues.at(i));
			std::cout << ";";
		}
		std::cout << "\"";
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
		std::cout << " computedStyle=\"";
		for (int i=0;i<currentElement->styleFields.size();i++)
		{
			std::cout << currentElement->styleFields.at(i) << ":";
			printCSSValue(currentElement->styleValues.at(i));
			std::cout << ";";
		}
		std::cout << "\"";
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
