/*
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 *
 * Created by Steven P. Goldsmith on November 3, 2015
 * sgoldsmith@codeferm.com
 */

#include <iostream>
#include <sys/time.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * Kernel used for contours.
 */
static const Mat CONTOUR_KERNEL = getStructuringElement(MORPH_DILATE,
		Size(3, 3), Point(1, 1));
/**
 * Point used for contour dilate and erode.
 */
static const Point CONTOUR_POINT = Point(-1, -1);

/**
 * Find contours and return in vector of Point.
 */
vector<vector<Point> > motion_contours(Mat source) {
	dilate(source, source, CONTOUR_KERNEL, CONTOUR_POINT, 15);
	erode(source, source, CONTOUR_KERNEL, CONTOUR_POINT, 10);
	// Find contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(source, contours, hierarchy, RETR_TREE,
			CHAIN_APPROX_SIMPLE, Point(0, 0));
	return contours;
}

/**
 * Uses moving average to determine change percent.
 *
 * argv[1] = source file or will default to "../../resources/traffic.mp4" if no
 * args passed.
 *
 * @author sgoldsmith
 * @version 1.0.0
 * @since 1.0.0
 */
int main(int argc, char *argv[]) {
	int return_val = 0;
	string url = "../../resources/traffic.mp4";
	string output_file = "../../output/motion-detect-cpp.avi";
	cout << CV_VERSION << endl;
	VideoCapture capture;
	Mat image;
	// See if URL arg passed
	if (argc == 2) {
		url = argv[1];
	}
	cout << "Input file:" << url << endl;
	cout << "Output file:" << output_file << endl;
	capture.open(url);
	// See if video capture opened
	if (capture.isOpened()) {
		cout << "Resolution: " << capture.get(CAP_PROP_FRAME_WIDTH) << "x"
				<< capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
		bool exit_loop = false;
		// Video writer
		VideoWriter writer(output_file, (int) capture.get(CAP_PROP_FOURCC),
				(int) capture.get(CAP_PROP_FPS),
				Size((int) capture.get(CAP_PROP_FRAME_WIDTH),
						(int) capture.get(CAP_PROP_FRAME_HEIGHT)));
		Mat work_img;
		Mat moving_avg_img;
		Mat gray_img;
		Mat diff_img;
		Mat scale_img;
		double motion_percent = 0.0;
		int frames_with_motion = 0;
		int frames = 0;
		Scalar color = Scalar(0, 255, 0);
		timeval start_time;
		gettimeofday(&start_time, 0);
		// Process all frames
		while (capture.read(image) && !exit_loop) {
			if (!image.empty()) {
				// Generate work image by blurring
				blur(image, work_img, Size(8, 8));
				// Generate moving average image if needed
				if (moving_avg_img.empty()) {
					moving_avg_img = Mat::zeros(work_img.size(), CV_32FC3);
				}
				// Generate moving average image
				accumulateWeighted(work_img, moving_avg_img, 0.03);
				// Convert the scale of the moving average
				convertScaleAbs(moving_avg_img, scale_img);
				// Subtract the work image frame from the scaled image average
				absdiff(work_img, scale_img, diff_img);
				// Convert the image to grayscale
				cvtColor(diff_img, gray_img, COLOR_BGR2GRAY);
				// Convert to BW
				threshold(gray_img, gray_img, 25, 255, THRESH_BINARY);
				// Total number of changed motion pixels
				motion_percent = 100.0 * countNonZero(gray_img) / image.total();
				// Detect if camera is adjusting and reset reference if more than
				// 25%
				if (motion_percent > 25.0) {
					work_img.convertTo(moving_avg_img, CV_32FC3);
				} else {
					// Threshold trigger motion
					if (motion_percent > 0.75) {
						frames_with_motion++;
						vector<vector<Point> > movement_locations =
								motion_contours(gray_img);
						// Process all points
						for (size_t i = 0, max = movement_locations.size();
								i != max; ++i) {
							Rect bounding_rect = boundingRect(
									movement_locations[i]);
							rectangle(image, bounding_rect.tl(),
									bounding_rect.br(), color, 2, 8, 0);
						}
					}
				}
				// Write frame with motion rectangles
				writer.write(image);
				frames++;
			} else {
				cout << "No frame captured" << endl;
				exit_loop = true;
			}
		}
		timeval end_time;
		gettimeofday(&end_time, 0);
		cout << frames << " frames, " << frames_with_motion
				<< " frames with motion" << endl;
		cout << "FPS " << (frames / (end_time.tv_sec - start_time.tv_sec))
				<< ", elapsed time: " << (end_time.tv_sec - start_time.tv_sec)
				<< " seconds" << endl;
		// Release VideoWriter
		writer.release();
		// Release VideoCapture
		capture.release();
	} else {
		cout << "Unable to open device" << endl;
		return_val = -1;
	}
	return return_val;
}
