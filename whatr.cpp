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

//Globally used font
TTF_Font *gFont = NULL;

int fontCharWidth[256];
int fontCharHeight[256];

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
			free();
		}
		//Loads image at specified path
		bool loadFromFile( std::string path )
		{
			//Get rid of preexisting texture
			free();
			//The final texture
			SDL_Texture* newTexture = NULL;
			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
			if( loadedSurface == NULL )
			{
				printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
			}
			else
			{
				//Color key image
				SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
				//Create texture from surface pixels
				newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
				if( newTexture == NULL )
				{
					printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
				}
				else
				{
					//Get image dimensions
					mWidth = loadedSurface->w;
					mHeight = loadedSurface->h;
				}
				//Get rid of old loaded surface
				SDL_FreeSurface( loadedSurface );
			}
			//Return success
			mTexture = newTexture;
			return mTexture != NULL;
		}
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor )
		{
			//Get rid of preexisting texture
			free();

			//Render text surface
			SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
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
		//Deallocates texture
		void free()
		{
			//Free texture if it exists
			if( mTexture != NULL )
			{
				SDL_DestroyTexture( mTexture );
				mTexture = NULL;
				mWidth = 0;
				mHeight = 0;
			}
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
						<< val.timeType << ")=("
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
	
	auto time_3b = std::chrono::high_resolution_clock::now();
	
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
		}
	}
	
	// Pseudo-element that encapsulates everything
	document.parent = NULL;
	document.text = std::string("document");
	document.type = 1;
	
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
	
	auto time_5 = std::chrono::high_resolution_clock::now();
	///////////////////////////////////
	////// Wait until the yaccing is done, then print the HTML element tree
	{
		while (yaccingPage){};
		for (int i=0;i<document.children.size();i++)
		{
			HTMLElement* currentElement = document.children.at(i);
			printTree(currentElement, std::string("  "));
		}
	}
	
	auto time_6 = std::chrono::high_resolution_clock::now();
	
	std::vector<CSSToken>** CSSTokens = new std::vector<CSSToken>*[styles.size()];
	std::vector<CSSClass>** CSSClasses = new std::vector<CSSClass>*[styles.size()];
	for (int i=0;i<styles.size();i++)
	{
		CSSTokens[i] = new std::vector<CSSToken>;
		CSSClasses[i] = new std::vector<CSSClass>;
		
		///////////////////////////////////
		////// Lex the css
		{
			lexingCSS = 1;
			cssLexArgs args(&lexingCSS, CSSTokens[i], styles[i]->children[0]->text);
			if (pthread_create(&cssLexThread, NULL, cssLexThreadFunc, &args))
			{
				ERROR(Failed to create CSS lex thread!);
				return 0;
			}
			while(lexingCSS){};
			PRINT(lexingCSS=0! printing CSSTokens...);
			for (int j=0;j<CSSTokens[i]->size();j++)
			{
				CSSToken t = CSSTokens[i]->at(j);
				std::cout << "CSSTokens["<<i<<"][" << j << "]={"
						<< t.type << " , ";
				t.text.print();
				std::cout << "}\n";
			}
		}
	
		///////////////////////////////////
		////// Yacc the css
		{
			yaccingCSS = 1;
			cssYaccArgs args(&yaccingCSS, CSSTokens[i], CSSClasses[i]);
			if (pthread_create(&cssYaccThread, NULL, cssYaccThreadFunc, &args))
			{
				ERROR(Failed to create CSS yacc thread!);
				return 0;
			}
			while(yaccingCSS){};
			PRINT(yaccingCSS=0! printing CSS classes...);
			for (int k=0;k<CSSClasses[i]->size();k++)
			{
				std::cout << "--- Class selector:\n";
				for (int j=0;j<CSSClasses[i]->at(k).selector.subSelectors.size();j++)
				{
					CSSSubSelector ss = CSSClasses[i]->at(k).selector.subSelectors.at(j);
					std::cout << ss.str1 << " " << ss.str2 << " " << ss.type << "\n";
				}
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
			while(applyingCSS){};
			printTree(&document, std::string("  "));
		}
	
	}
		
	auto time_7 = std::chrono::high_resolution_clock::now();

	auto time_8 = std::chrono::high_resolution_clock::now();
	
	auto time_9 = std::chrono::high_resolution_clock::now();
	
	///////////////////////////////////
	////// Renderer 1: HTML Transform
	/*
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
	*/
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
	
	///////////////////////////////////
	////// Create window
	

	
	
	
	
	
	
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
				}
			}
		}
	}
	
	
	
	
	
	
	gFont = TTF_OpenFont( "arial.ttf", 28 );
	
	char allChars[512];
	for (int i=0;i<256;i++)
	{
		allChars[i*2] = (char)i;
		allChars[i*2+1] = 0;
	}
	
	char* c = &allChars[0];
	for (int i=0;i<256;i++,c+=2)
	{
		if (isprint(*c))
		{
			TTF_SizeText(gFont, c, &fontCharWidth[i], &fontCharHeight[i]);
		}
		else
		{
			fontCharWidth[i] = fontCharHeight[i] = 0;
		}
	}
	
	ConstStr testStr("The lazy fox jumped over the big brown dog. This was a nice rare occurrence of a visual message.");
	int ww, hh;
	calculateTextSize(testStr, 300, fontCharWidth, fontCharHeight, ww, hh);
	
	
	
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
		}
	}
	
	
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		//Render current frame
		gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );

		//Update screen
		SDL_RenderPresent( gRenderer );
	}
	
	//Free loaded images
	gTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
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
