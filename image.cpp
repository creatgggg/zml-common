#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
void main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	Mat imgGray,imgBlur,imgCanny,imgDil,imgErode;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 50, 150);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	erode(imgDil, imgErode, kernel);
	imshow("image", img);
	imshow("image Gray", imgGray);
	imshow("image Blur", imgBlur);
	imshow("imgag Canny", imgCanny);
	imshow("imgag Dliate", imgDil);
	imshow("image Erode", imgErode);
	waitKey(0);
}
