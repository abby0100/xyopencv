#include <iostream>
#include <string>
#include "highgui.h"
#include "../0-multiple-windows/TimeUtil.hpp"
//#include "opencv2/highgui.hpp"

const std::string WINDOW_NAME = "Example Window XY";

void draw_picture(char* path) {

	std::cout << "[-] draw_picture" << std::endl;
	IplImage* img = cvLoadImage( path );
	cvNamedWindow( WINDOW_NAME.c_str(), CV_WINDOW_AUTOSIZE );
	cvShowImage( WINDOW_NAME.c_str(), img );
	cvWaitKey(0);

	cvReleaseImage( &img );
	cvDestroyWindow( WINDOW_NAME.c_str() );
}

void draw_video(char* path) {

	int count = 0;
	IplImage* frame;
	CvCapture* capture = cvCreateFileCapture( path );
	cvNamedWindow( WINDOW_NAME.c_str(), CV_WINDOW_AUTOSIZE );

	TimeUtil* tu = new TimeUtil();
	tu->StartCount();

	while(1) {
		frame = cvQueryFrame( capture );
		if( !frame ) break;
		cvShowImage( WINDOW_NAME.c_str(), frame );
		char c = cvWaitKey(25);
		if( c== 27 ) break;

		count++;
		if( count % 30 == 0 ) {
			tu->StopCount("read 30 frames");
			tu->Report();
			tu->StartCount();
		}
	}

	cvReleaseCapture( &capture );
	cvDestroyWindow( WINDOW_NAME.c_str() );
}

int main(int argc, char **argv) {

	std::cout << "[+] " << __FUNCTION__ << std::endl;

	char* path = argv[1];
	std::cout << "path\t" << path << std::endl;


	//draw_picture( path );
	draw_video( path );


	return 0;
}
