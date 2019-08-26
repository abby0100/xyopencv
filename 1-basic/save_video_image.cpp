#include <iostream>
#include "opencv2/opencv.hpp"
#include "unistd.h"

/*
 * https://blog.csdn.net/BobAuditore/article/details/82153686
 */
void save_video_image(std::string path) {

	if(path.empty() || path.size() == 0) {
		cout << "Not a valid video file, exiting..." << endl;
		return;
	}

	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open(path);
	if(!cap.isOpened()) {
		cout << "Failed to open video file, exiting..." << endl;
		return;
	}

	// create dump fold
	char dump_folder[256] = "dumped_files";
	if (access(dump_folder, F_OK) != 0) {
		if (mkdir(dump_folder, 0755) != 0) {
			cout << "Create dumped folder failed, exit..." << endl;
			return;
		}
	}

	int nFrame = 0;
	while(true) {
		// read input (video) frame
		cap >> frame;
		if( frame.empty() ) break;

		// write frame
		char frame_path[64];
		sprintf(frame_path, "dumped_files/frame_%d.bmp", nFrame);
		cv::imwrite(frame_path, frame);
		nFrame++;
	}

	cap.release();
}


int main(int argc, char **argv) {

	std::string filepath = "my.mp4";
	save_video_image( filepath );

	return 0;
}
