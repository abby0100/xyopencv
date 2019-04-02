#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "unistd.h"
#include <iostream>

#include <thread>
//#include <pthread.h>
//#include <X11/Xlib.h>

#include "TimeUtil.hpp"

#define VIDEO_NUM_MAX	48
#define FRAME_INTERVAL	25000

using namespace cv;
using namespace std;

const std::string WINDOW_NAME	= "xy window";
const std::string HELLO_WORLD	= "Hello world, OpenCV! -- from xy";

cv::Mat window_mat;
cv::VideoCapture vc;
std::string inputVideoFile= "videos/0.mp4";
std::vector<cv::VideoCapture> vcs;
std::vector<std::string> inputVideoFiles;

struct Window {
	int window_width;
	int window_height;
	int row_number;
	int col_number;
	int subwindow_width;
	int subwindow_height;
} window;

void exit_and_release() {
	vc.release();
	destroyAllWindows();
	exit(-1);
}

void waitFinish() {
	int count = 0;
	int c;
	while(true) {
		c = getchar();
		//cout << "getchar " << c << endl;
		cout << "sleep " << count << endl;
		count++;
		sleep(1);
		if(c == 'q' || c == 'Q') {
			return;
			//exit(0);
		}
	}
}

cv::Mat getChildWindow(const int channel) {
	cv::Mat mat;
	//std::cout << "getChildWindow " << channel << std::endl;

	if (channel < 0 || channel >= VIDEO_NUM_MAX) {
		return mat;
	}

	int row_idx = channel / window.col_number;
	int col_idx = channel % window.col_number;
	int left = window.subwindow_width * col_idx;
	int top = window.subwindow_height * row_idx;

	//std::cout << "getChildWindow row\t" << row_idx << std::endl;
	//std::cout << "getChildWindow col\t" << col_idx << std::endl;
	//std::cout << "getChildWindow left\t" << left << std::endl;
	//std::cout << "getChildWindow top\t" << top << std::endl;

	//window_mat = cv::Mat(window_mat.rows, window_mat.cols, window_mat.type(), cv::Scalar(255, 0, 0, 1));
	return window_mat( cv::Rect(window.subwindow_width * col_idx, window.subwindow_height * row_idx, window.subwindow_width, window.subwindow_height) );
}

void drawVideo(int channel) {

	int frames = 0;
	cv::Mat video_image;
	cv::Mat child_mat = getChildWindow(channel);

	TimeUtil *tu = new TimeUtil();
	tu->StartCount();

	while (true) {
		if (vcs[channel].read(video_image)) {
			cv::resize(video_image, child_mat, child_mat.size());
			std::cout << "drawVideo read video " << channel << " " << inputVideoFiles[channel] << " " << ++frames << std::endl;
		} else {
			std::cout << "drawVideo read video " << channel << " " << inputVideoFiles[channel] << " " << ++frames << " failed" << std::endl;
		}

		if (frames % 30 == 0) {
			tu->StopCount("drawVideo " + std::to_string(channel) + " 30 Frames");
			tu->Report();
			tu->StartCount();
		}
		//video_image.copyTo(child_mat);
		usleep(FRAME_INTERVAL);
	}
}

void display_window() {

	int count = 0;
	cv::Mat video_image;

	while (true) {
		cv::imshow(WINDOW_NAME, window_mat);
		cout << "display_window " << ++count << endl;

		//cv::waitKey(1);
		//usleep(FRAME_INTERVAL);
		if( cv::waitKey(33) >= 0 ) break;
	}
}

void test_window(int num) {
	//image = cv::Mat::zeros(image.size(), image.type());
	//cv::Mat test_mat = cv::Mat(window.subwindow_height, window.subwindow_width, window_mat.type(), cv::Scalar(255, 0, 0, 0.1));

	cv::Mat video_image;
	cv::Mat child_mat;
	while (true) {
		for (int i = 0; i < num; ++i) {
			child_mat = getChildWindow(i);
			//if (vc.read(child_mat)) {
			if (vc.read(video_image)) {
				cv::resize(video_image, child_mat, child_mat.size());
				//video_image.copyTo(child_mat);
				
				//cv::imshow(inputVideoFile, child_mat);
				cv::imshow(inputVideoFile, window_mat);
				cv::waitKey(1);
				usleep(FRAME_INTERVAL);
			}
		}
	}
}

void update_multiple_window(int num) {
	std::cout << "update_multiple_window num " << num << std::endl;
	std::vector<std::thread> threads;

	for (int i = 0; i < num; ++i) {
		int *idx = new int(i);
		std::cout << "create thread " << *idx << std::endl;
		//std::thread th(drawVideo, *idx);
		std::thread th(&drawVideo, *idx);
		threads.push_back(move(th));
	}
	
	for (int i = 0; i < num; ++i) {
		//th.join();
		threads[i].join();
	}
}

void drawText(cv::Mat &image) {
	cv::Point center = cv::Point(0, 0);
	cv::Scalar scalar = cv::Scalar(255, 0, 0);
	putText(image, HELLO_WORLD, center, FONT_HERSHEY_COMPLEX, 1, scalar, 1, LINE_AA);
}

void init_window(int num) {

	for (int i = 0; i < num; ++i) {
		cv::VideoCapture vc;
		std::string inputVideoFile= "videos/" + std::to_string(i) + ".mp4";
		vc.open(inputVideoFile);
		if (!vc.isOpened()) {
			std::cout << "open video " << i << " " << inputVideoFile << " failed" << std::endl;
			exit_and_release();
		}
		vcs.push_back(vc);
		inputVideoFiles.push_back(inputVideoFile);
	}
	//vc.release();

	//window_mat = cv::Mat(window.window_height, window.window_width, CV_8UC3, cv::Scalar(255, 255, 255, 0.1));
	if (vcs[0].read(window_mat)) {
		// get video image width/height
		window.window_width = window_mat.cols;
		window.window_height = window_mat.rows;
		std::cout << "window_width\t\t"  << window.window_width  << std::endl;
		std::cout << "window_height\t\t" << window.window_height << std::endl;

		// calculate subwindow cols/rows
		window.row_number = sqrt(num);
		window.row_number = (window.row_number * window.row_number < num) ? window.row_number + 1 : window.row_number;
		window.col_number = window.row_number;
		std::cout << "row_number\t\t" << window.row_number << std::endl;
		std::cout << "col_number\t\t" << window.col_number << std::endl;
	
		// set subwindow width/height
		window.subwindow_width = window.window_width / window.col_number;
		window.subwindow_height= window.window_height/ window.row_number;
		std::cout << "subwindow_width\t\t"  << window.subwindow_width  << std::endl;
		std::cout << "subwindow_height\t" << window.subwindow_height << std::endl;
	} else {
		exit_and_release();
	}
}

void init(int argc, char **argv, int &num) {

	std::cout << "OpenCV\t\t\t" << CV_VERSION << std::endl;
	//XInitThreads();

	if (argc >= 2) {
		num = atoi(argv[1]);
		if (num > VIDEO_NUM_MAX) {
			std::cout << "[-] Only handle " << VIDEO_NUM_MAX << " of " << num << " videos!" << std::endl;
			num = VIDEO_NUM_MAX;
		}
	}
	std::cout << "video_num\t\t" << num << std::endl;

	init_window(num);
}


int main(int argc, char** argv) {

	int video_num = 1;	// default 1

	init(argc, argv, video_num);

	//test_window(video_num);

	std::thread dispaly_thread(display_window);
	update_multiple_window(video_num);
	dispaly_thread.join();

/*
	waitFinish();
*/

	exit_and_release();
	return 0;
}
