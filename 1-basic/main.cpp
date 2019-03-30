#include <iostream>
#include "../999-utils/LogUtil.hpp"
//#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

const std::string WINDOW_NAME = "Window XY";

void init() {

}

void test_opencv(std::string path) {
	cv::Mat img = cv::imread( path, -1 );

	if ( img.empty() ) return;

	cv::namedWindow( WINDOW_NAME, cv::WINDOW_AUTOSIZE );
	cv::imshow( WINDOW_NAME, img );
	cv::waitKey( 0 );
	cv::destroyWindow( WINDOW_NAME );
}

int main(int argc, char **argv) {
	LogUtil::d( __FUNCTION__, "MAIN" );

	init();

	if ( argc > 1 ) {
		std::string path = argv[1];
		test_opencv( path );
	}

	return 0;
}
