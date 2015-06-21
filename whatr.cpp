/* Copyright (C) 2015 Jesse Busman
/* You may not distribute this program or parts of this
/* program in any way, shape or form without written
/* permission from Jesse Busman (born 17 march 1996).
/* This program is distributed in the hope that it will
/* be useful, but WITHOUT ANY WARRANTY; without even the
/* implied warranty of MERCHANTABILITY or FITNESS FOR A
/* PARTICULAR PURPOSE. This message may not be changed
/* or removed. */

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

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
#include "text_and_fonts.h"
#include "settings.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Font* font = NULL;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture()
		{
			//Initialize
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
		//Deallocates memory
		~LTexture()
		{
			//Deallocate
			//free();
		}
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor , TTF_Font* font)
		{
			//Get rid of preexisting texture
			//free();

			//Render text surface
			SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor );
			if( textSurface == NULL )
			{
				printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
			}
			else
			{
				//Create texture from surface pixels
				mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
				if( mTexture == NULL )
				{
					printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
				}
				else
				{
					//Get image dimensions
					mWidth = textSurface->w;
					mHeight = textSurface->h;
				}
				//Get rid of old surface
				SDL_FreeSurface( textSurface );
			}
			//Return success
			return mTexture != NULL;
		}
		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue )
		{
			//Modulate texture rgb
			SDL_SetTextureColorMod( mTexture, red, green, blue );
		}
		//Set blending
		void setBlendMode( SDL_BlendMode blending )
		{
			//Set blending function
			SDL_SetTextureBlendMode( mTexture, blending );
		}
		//Set alpha modulation
		void setAlpha( Uint8 alpha )
		{
			//Modulate texture alpha
			SDL_SetTextureAlphaMod( mTexture, alpha );
		}
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
		{
			//Set rendering space and render to screen
			SDL_Rect renderQuad = { x, y, mWidth, mHeight };
			//Set clip rendering dimensions
			if( clip != NULL )
			{
				renderQuad.w = clip->w;
				renderQuad.h = clip->h;
			}
			//Render to screen
			SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
		}
		//Gets image dimensions
		int getWidth()
		{
			return mWidth;
		}
		int getHeight()
		{
			return mHeight;
		}
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		//Image dimensions
		int mWidth;
		int mHeight;
};

//Rendered texture
LTexture gTextTexture;


void printCSSValue(CSSValue val)
{
	std::cout << "("	<< val.textType << ","
						<< val.lengthType << ","
						<< val.colorType << ","
						<< val.timeType << ","
						<< val.constant << ","
						<< (long)val.font << ")=("
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
HTMLElement document;
std::vector<HTMLElement*> styles;
//----------------------------

//----------------------------
pthread_t cssLexThread;
int lexingCSS = 0;
//----------------------------

//----------------------------
pthread_t cssYaccThread;
int yaccingCSS = 0;
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
	// Initialize SDL_ttf
	//if (TTF_Init()==-1)
	//{
	//	printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	//}
	auto tsAll = std::chrono::high_resolution_clock::now();
	
	//initFontArray();
	
	//font = fonts.at(0);
	
	try{
	
	if (argc!=2)
	{
		ERROR(One URL argument is required!);
		return 0;
	}
	
	auto tsUrl = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Get URL from arguments
	{
		url = std::string(argv[1]);
	}
	
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
		path = url.subString(i);
		if (path.length==0) path = std::string("/");
	}
	
	auto teUrl = std::chrono::high_resolution_clock::now();
	auto tsDownloadThread = std::chrono::high_resolution_clock::now();
	
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
	
	auto teDownloadThread = std::chrono::high_resolution_clock::now();
	auto tsDownload = std::chrono::high_resolution_clock::now();
	
	while (downloadingPage){usleep(10);}
	
	auto teDownload = std::chrono::high_resolution_clock::now();
	auto tsHtmlLexThread = std::chrono::high_resolution_clock::now();
	
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
	
	
	auto teHtmlLexThread = std::chrono::high_resolution_clock::now();
	auto tsHtmlLex = std::chrono::high_resolution_clock::now();
	while (lexingPage){usleep(10);}
	auto teHtmlLex = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Wait until the lexing is done, then print the headers and tags
	{
		/*while (lexingPage){};
		PRINT(The HTML lexer is done! Here are its results:)
		for (int i=0;i<HTMLTags.size();i++)
		{
			HTMLTag tag = HTMLTags[i];
			if (tag.type==0)
			{
				std::cout << GREEN << "Text node: " << NOCLR << '"';
				tag.text.print();
				std::cout << '"' << "\n";
			}
			else if (tag.type==1)
			{
				std::cout << "<" << GREEN;
				tag.text.print();
				for (int j=0;j<tag.argNames.size();j++)
				{
					std::cout << " " << RED;
					tag.argNames[j].print();
					std::cout << NOCLR << "=" << "\"" << GREEN;
					tag.argValues[j].print();
					std::cout << NOCLR << "\"";
				}
				std::cout << ">\n" << NOCLR;
			}
			else if (tag.type==2)
			{
				tag.text.printLine();
				std::cout << GREEN << "<" << tag.text.copy();
				for (int j=0;j<tag.argNames.size();j++)
				{
					std::cout << " " << RED;
					tag.argNames[j].print();
					std::cout << NOCLR << "=\"" << GREEN;
					tag.argValues[j].print();
					std::cout << NOCLR << "\"";
				}
				std::cout << "/>\n" << NOCLR;
			}
			else if (tag.type==3)
			{
				std::cout << GREEN << "</" << tag.text.copy() << ">\n" << NOCLR;
			}
			else
			{
				std::cout << RED << "ERROR ERROR FATAL ERROR: Tag with unknown type " << tag.type << NOCLR << "\n";
			}
		}*/
	}
	
	// Pseudo-element that encapsulates everything
	document.parent = NULL;
	document.text = std::string("document");
	document.type = 1;
	
	auto tsHtmlYaccThread = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Start thread that yaccs the HTML tags
	{
		yaccingPage = 1;
		htmlYaccArgs args(&lexingPage,
						&yaccingPage,
						&HTMLTags,
						&document,
						&styles);
		if (pthread_create(&htmlYaccThread, NULL, htmlYaccThreadFunc, &args))
		{
			ERROR(Failed to create HTML yacc thread!);
			return 0;
		}
	}
	
	auto teHtmlYaccThread = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Wait until the yaccing is done, then print the HTML element tree
	auto tsHtmlYacc = std::chrono::high_resolution_clock::now();
	while (yaccingPage){usleep(10);};
	auto teHtmlYacc = std::chrono::high_resolution_clock::now();
	{
		for (int i=0;i<document.children.size();i++)
		{
			HTMLElement* currentElement = document.children.at(i);
			printTree(currentElement, std::string("  "));
		}
	}
	
	std::chrono::duration<double> lexCssTime, yaccCssTime, applyCssTime;
	
	std::vector<CSSToken>** CSSTokens = new std::vector<CSSToken>*[styles.size()];
	std::vector<CSSClass>** CSSClasses = new std::vector<CSSClass>*[styles.size()];
	for (int i=0;i<styles.size();i++)
	{
		CSSTokens[i] = new std::vector<CSSToken>;
		CSSClasses[i] = new std::vector<CSSClass>;
		
		///////////////////////////////////
		////// Lex the css
		{
			auto tsCssLex = std::chrono::high_resolution_clock::now();
			lexingCSS = 1;
			cssLexArgs args(&lexingCSS, CSSTokens[i], styles[i]->children[0]->text);
			if (pthread_create(&cssLexThread, NULL, cssLexThreadFunc, &args))
			{
				ERROR(Failed to create CSS lex thread!);
				return 0;
			}
			while(lexingCSS){usleep(10);};
			auto teCssLex = std::chrono::high_resolution_clock::now();
			lexCssTime += teCssLex - tsCssLex;
			PRINT(lexingCSS=0! printing CSSTokens...);
			/*for (int j=0;j<CSSTokens[i]->size();j++)
			{
				CSSToken t = CSSTokens[i]->at(j);
				std::cout << "CSSTokens["<<i<<"][" << j << "]={"
						<< t.type << " , ";
				t.text.print();
				std::cout << "}\n";
			}*/
		}
	
		///////////////////////////////////
		////// Yacc the css
		{
			auto tsCssYacc = std::chrono::high_resolution_clock::now();
			yaccingCSS = 1;
			cssYaccArgs args(&yaccingCSS, CSSTokens[i], CSSClasses[i]);
			if (pthread_create(&cssYaccThread, NULL, cssYaccThreadFunc, &args))
			{
				ERROR(Failed to create CSS yacc thread!);
				return 0;
			}
			while(yaccingCSS){usleep(10);};
			usleep(10000);
			auto teCssYacc = std::chrono::high_resolution_clock::now();
			yaccCssTime += teCssYacc - tsCssYacc;
			std::cout<<"Yacc css this: "<<std::chrono::duration_cast<std::chrono::microseconds>(yaccCssTime).count()<<"us\n";
			std::cout<<"Yacc css so far: "<<std::chrono::duration_cast<std::chrono::microseconds>(teCssYacc - tsCssYacc).count()<<"us\n";
			PRINT(yaccingCSS=0! printing CSS classes...);
			for (int k=0;k<CSSClasses[i]->size();k++)
			{
				std::cout << "--- Class selector:\n";
				for (int j=0;j<CSSClasses[i]->at(k).selector.subSelectors.size();j++)
				{
					CSSSubSelector ss = CSSClasses[i]->at(k).selector.subSelectors.at(j);
					std::cout << ss.str1 << " " << ss.str2 << " " << ss.type << "\n";
				}
				//std::cout << "--- Class rules:\n";
				std::cout << CSSClasses[i]->at(k).ruleProperties.size() << '-';
				std::cout << CSSClasses[i]->at(k).ruleValues.size();
				std::cout << "--- Class rules:\n";
				for (int j=0;j<CSSClasses[i]->at(k).ruleProperties.size();j++)
				{
					std::cout << CSSClasses[i]->at(k).ruleProperties.at(j) << ": ";
					printCSSValue(CSSClasses[i]->at(k).ruleValues.at(j));
					std::cout << "\n";
				}
			}
		}
	
		///////////////////////////////////
		////// Apply the css
		{
			auto tsCssApply = std::chrono::high_resolution_clock::now();
			applyingCSS = 1;
			cssApplyArgs args(&applyingCSS, CSSClasses[i], &document);
			/*if (styles[i]->parent==NULL ||
				styles[i]->parent->tag==TAG_HEAD ||
				styles[i]->parent->tag==TAG_BODY ||
				styles[i]->parent->tag==TAG_HTML)
			{
				args.destination = &document;
			}
			else
			{
				args.destination = styles[i]->parent;
			}*/
			if (pthread_create(&cssApplyThread, NULL, cssApplyThreadFunc, &args))
			{
				ERROR(Failed to create CSS yacc thread!);
				return 0;
			}
			while(applyingCSS){usleep(10);};
			auto teCssApply = std::chrono::high_resolution_clock::now();
			applyCssTime += teCssApply - tsCssApply;
			printTree(&document, std::string("  "));
		}
	
	}
	
	// Initialize SDL_ttf
	if (TTF_Init()==-1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	Font* font = loadFont("arial.ttf");
	
	///////////////////////////////////
	////// Renderer 1: HTML Transform
	document.availableWidth  = document.width  = SCREEN_WIDTH ;
	document.availableHeight = document.height = SCREEN_HEIGHT;
	auto tsRenderer1Thread = std::chrono::high_resolution_clock::now();
	{
		rendering1 = 1;
		renderer1Args args(&rendering1, &document);
		if (pthread_create(&renderer1Thread, NULL, renderer1ThreadFunc, &args))
		{
			ERROR(Failed to create renderer 1 thread!);
			return 0;
		}
	}
	auto teRenderer1Thread = std::chrono::high_resolution_clock::now();
	
	auto tsRenderer1 = std::chrono::high_resolution_clock::now();
	while(rendering1){usleep(10);};
	auto teRenderer1 = std::chrono::high_resolution_clock::now();
	//printTree(&document, std::string("  "));
	
	auto teAll = std::chrono::high_resolution_clock::now();
	
	auto timeUrl = teUrl - tsUrl;
	auto timeDownloadThread = teDownloadThread - tsDownloadThread;
	auto timeDownload = teDownload - tsDownload;
	auto timeHtmlLexThread = teHtmlLexThread - tsHtmlLexThread;
	auto timeHtmlLex = teHtmlLex - tsHtmlLex;
	auto timeHtmlYaccThread = teHtmlYaccThread - tsHtmlYaccThread;
	auto timeHtmlYacc = teHtmlYacc - tsHtmlYacc;
	auto timeRenderer1Thread = teRenderer1Thread - tsRenderer1Thread;
	auto timeRenderer1 = teRenderer1 - tsRenderer1;
	
	std::cout << "\n\n##### Slowness report:\n";
	std::cout <<"Parse URL: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeUrl).count()<<"us\n";
	std::cout<<"Start download thread: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeDownloadThread).count()<<"us\n";
	std::cout<<"Download page: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeDownload).count()<<"us\n";
	std::cout<<"Start lex thread: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeHtmlLexThread).count()<<"us\n";
	std::cout<<"Lex html: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeHtmlLex).count()<<"us\n";
	std::cout<<"Start yacc html thread: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeHtmlYaccThread).count()<<"us\n";
	std::cout<<"Yacc html: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeHtmlYacc).count()<<"us\n";
	std::cout<<"Lex css: "<<std::chrono::duration_cast<std::chrono::microseconds>(lexCssTime).count()<<"us\n";
	std::cout<<"Yacc css: "<<std::chrono::duration_cast<std::chrono::microseconds>(yaccCssTime).count()<<"us\n";
	std::cout<<"Apply css: "<<std::chrono::duration_cast<std::chrono::microseconds>(applyCssTime).count()<<"us\n";
	std::cout<<"Start renderer1 thread: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeRenderer1Thread).count()<<"us\n";
	std::cout<<"Renderer 1: "<<std::chrono::duration_cast<std::chrono::microseconds>(timeRenderer1).count()<<"us\n";
	
	auto total = teAll-tsAll;
	
	std::cout << "##### Total time taken: "<<std::chrono::duration_cast<std::chrono::microseconds>(total).count()<<"us\n";
	std::cout << "##### Total time taken excluding download: "<<std::chrono::duration_cast<std::chrono::microseconds>(total-timeDownload).count()<<"us\n";
	
	///////////////////////////////////
	////// Create window
	
	// Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO)<0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		// Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags)&imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				}
			}
		}
	}
	if (font->gFont==NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		// Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!gTextTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor, font->gFont))
		{
			printf("Failed to render text texture!\n");
		}
	}
	
	// Main loop flag
	bool quit = false;
	// Event handler
	SDL_Event e;
	// While application is running
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e)!=0)
		{
			// User requests quit
			if (e.type==SDL_QUIT) quit = true;
		}
		// Clear screen
		SDL_SetRenderDrawColor (gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear (gRenderer);
		// Render current frame
		
		// Draw text
		gTextTexture.render((SCREEN_WIDTH-gTextTexture.getWidth())/2, (SCREEN_HEIGHT-gTextTexture.getHeight())/2);
		
		// Draw line
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);
		
		// Draw rect
		SDL_Rect outlineRect =
		{
			SCREEN_WIDTH / 6,
			SCREEN_HEIGHT / 6,
			SCREEN_WIDTH * 2 / 3,
			SCREEN_HEIGHT * 2 / 3
		};
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &outlineRect);
		
		// Draw dots
		for (int x=0; x<SCREEN_WIDTH/2; x++)
		{
			for (int y=0; y<SCREEN_HEIGHT/2; y++)
			{
				SDL_SetRenderDrawColor(gRenderer, 0x22*((x%7)&(y%11)), 0x22*((x%17)&(y%13)), 0x11*((x%23)&(y%29)), 0x88);
				SDL_RenderDrawPoint(gRenderer, x, y);
			}
		}
		
		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	
	// Free loaded images
	//gTextTexture.free();
	// Free global font
	TTF_CloseFont(font->gFont);
	font->gFont = NULL;
	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	// Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	
	}
	catch(int exex)
	{
		printf("exception=%i=%s\n", exex, (char*)(&exex));
	}
	
	return 0;
}
void recursiveRenderer2(HTMLElement* el, bool insideBody)
{
	std::cout << "recursiveRenderer2 is at a <" << el->text;
	if (insideBody) std::cout << " insideBody";
	std::cout << "...>\n";
	if (el->type==0) // Text
	{
		if (!insideBody) return;
	}
	else // HTML element
	{
		for (	std::vector<HTMLElement*>::iterator h=el->children.begin();
				h!=el->children.end();
				h++)
		{
			if (insideBody)
			{
			}
			recursiveRenderer2(*h, (el->tag == TAG_BODY) || insideBody);
		}
	}
}
void printTree(HTMLElement* currentElement, std::string tabs)
{
	if (currentElement->type==0)
	{
		std::cout << tabs << "TEXT[";
		currentElement->text.trim(' ', '\t', '\n', '\r').print();
		std::cout << "]" << "\n";
	}
	else if (currentElement->children.size()==0)
	{
		std::cout << tabs << "<";
		currentElement->text.print();
		for (int i=0;i<currentElement->argNames.size();i++)
		{
			std::cout << " " << currentElement->argNames.at(i);
			std::cout << "=\"" << currentElement->argValues.at(i);
			std::cout << "\"";
		}
		std::cout << " computedStyle=\"";
		for (int i=0;i<currentElement->styleFields.size();i++)
		{
			std::cout << currentElement->styleFields.at(i);
			std::cout << ":";
			printCSSValue(currentElement->styleValues.at(i));
			std::cout << ";";
		}
		std::cout << "\"/>\n";
	}
	else
	{
		std::cout << tabs << "<";
		currentElement->text.print();
		for (int i=0;i<currentElement->argNames.size();i++)
		{
			std::cout << " " << currentElement->argNames.at(i);
			std::cout << "=\"" << currentElement->argValues.at(i);
			std::cout << "\"";
		}
		std::cout << " computedStyle=\"";
		for (int i=0;i<currentElement->styleFields.size();i++)
		{
			std::cout << currentElement->styleFields.at(i);
			std::cout << ":";
			printCSSValue(currentElement->styleValues.at(i));
			std::cout << ";";
		}
		std::cout << "\">\n";
		for (int i=0;i<currentElement->children.size();i++)
		{
			printTree(currentElement->children.at(i), tabs+std::string("  "));
		}
		std::cout << tabs << "</" << currentElement->text << ">\n";
	}
}
