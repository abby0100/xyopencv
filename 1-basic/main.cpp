#include <iostream>
#include "../999-utils/LogUtil.hpp"
#include "../999-utils/TimeUtil.hpp"
#include "opencv2/opencv.hpp"
#include "unistd.h"
//#include "opencv2/highgui.hpp"

int g_flag_run			= 1;
int g_slider_position		= 0;
cv::VideoCapture g_cap;
const std::string WINDOW_NAME		= "Window XY in";
const std::string WINDOW_NAME_OUT	= "Window XY out";

std::string mystring( int number ) {
	return std::to_string(number);
}

void init() {

}

void show_image(std::string path) {

	cv::namedWindow( WINDOW_NAME, cv::WINDOW_AUTOSIZE );

	cv::Mat img = cv::imread( path, -1 );
	if ( img.empty() ) return;

	cv::imshow( WINDOW_NAME, img );
	cv::waitKey( 0 );
	cv::destroyWindow( WINDOW_NAME );
}

void onTrackbarSlide( int pos, void * ) {

	g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
}

void show_video(std::string path) {

	cv::namedWindow( WINDOW_NAME, cv::WINDOW_AUTOSIZE );

	int count = 0;
	TimeUtil tu;
	cv::Mat frame;
	g_cap.open(path);

	int frames = g_cap.get(cv::CAP_PROP_FRAME_COUNT);
	int width = g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height= g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	LogUtil::d( __FUNCTION__, "Video: " + mystring(width) + "*" + mystring(height) + " " + mystring(frames) + " frames");

	char c = 0;
	int current_pos = 0;
	std::string POSTION = "Position";
	cv::createTrackbar(POSTION, WINDOW_NAME, &g_slider_position, frames, onTrackbarSlide);

	tu.StartCount();
	while(true) {
		if(g_flag_run) {
			g_cap >> frame;
			if( frame.empty() ) break;

			//tu.StartCount();
			current_pos = g_cap.get(cv::CAP_PROP_POS_FRAMES);
			//cv::setTrackbarPos(POSTION, WINDOW_NAME, current_pos);
			//tu.StopCount("cv::setTrackbarPos");
			//tu.Report();

			cv::imshow( WINDOW_NAME, frame );
			if(++count % 30 == 0) {
				tu.StopCount("30 frames");
				tu.Report();
				tu.StartCount();
			}
		}

		//tu.StopCount("cv::imshow");
		//tu.Report();
		//tu.StartCount();

		c = cv::waitKey(25);
		if (c == 's') {
			g_flag_run = 0;
			std::cout << "Single step, run = " << g_flag_run << std::endl;
		} else if (c == 'c') {
			g_flag_run = 1;
			std::cout << "Continue..., run = " << g_flag_run << std::endl;
		} else if (c == 27) {
			break;
		}

		//tu.StopCount("cv::waitKey");
		//tu.Report();
		//tu.StartCount();
	}

	cv::destroyWindow( WINDOW_NAME );
}

void smooth_image(std::string path) {

	cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(WINDOW_NAME_OUT, cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(path);

	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height= cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	LogUtil::d(__FUNCTION__, "Video: " + mystring(width) + "*" + mystring(height));
	sleep(1);

	cv::Mat frame;
	cv::Mat out;

	TimeUtil tu;

	while(g_flag_run) {

		tu.StartCount();

		cap >> frame;

		tu.StopCount("read frame");
		tu.StartCount();

//---Gaussian Blur
//		frame.copyTo(out);
//		tu.StopCount("copyTo");
//		tu.StartCount();

//		cv::GaussianBlur(frame, out, cv::Size(5, 5), 3, 3);
//		tu.StopCount("GaussianBlur 1");
//		tu.StartCount();

//		cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);
//		tu.StopCount("GaussianBlur 2");

//---Downscale
		cv::pyrDown(frame, out);
		cv::pyrDown(out, out);
		tu.StopCount("cv::pyrDown");
		tu.StartCount();

		cv::cvtColor(out, out, cv::COLOR_BGR2GRAY);
		tu.StopCount("cv::cvtColor");
		tu.StartCount();

		cv::Canny(out, out, 10, 100, 3, true);
		tu.StopCount("cv::Canny");
		tu.Report();

		cv::imshow(WINDOW_NAME, frame);
		cv::imshow(WINDOW_NAME_OUT, out);
		cv::waitKey(25);
	}
}

void write_pixel(std::string path) {

	cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(WINDOW_NAME_OUT, cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(path);

	cv::Mat frame;
	cv::Mat out;

	int x = 16;
	int y = 32;
	while(x > 0) {
		cap >> frame;

		cv::Vec3b intensity = frame.at<cv::Vec3b>(y, x);
		uchar blue = intensity[0];
		uchar green= intensity[1];
		uchar red  = intensity[2];
		std::cout << "[-] At (x, y) = (" << x << ", " << y << "): (b, g, r) = (" << (unsigned int)blue 
		<< ", " << (unsigned int)green << ", " << (unsigned int)red << ")." << std::endl;

		cv::cvtColor(frame, out, cv::COLOR_BGR2GRAY);
		std::cout << "[-] Gray pixel is: " << (unsigned int)out.at<uchar>(y, x) << std::endl;

		out.at<uchar>(y, x) = 128;
		std::cout << "[-] set pixel: " << (unsigned int)out.at<uchar>(y, x) << std::endl;

		//cv::pyrDown(out, out);
		//cv::pyrDown(out, out);
		//std::cout << "[-] Pyramide2 pixel is: " << (unsigned int)out.at<uchar>(y/4, x/4) << std::endl;

		cv::imshow(WINDOW_NAME, frame);
		cv::imshow(WINDOW_NAME_OUT, out);
		cv::waitKey(20);
	}
}

int main(int argc, char **argv) {
	LogUtil::d( __FUNCTION__, "MAIN" );

	init();

	if ( argc > 1 ) {
		std::string path = argv[1];
		//show_image( path );
		//show_video( path );
		//smooth_image(path);
		write_pixel(path);
	}

	return 0;
}
