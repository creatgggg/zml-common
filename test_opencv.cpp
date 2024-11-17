#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace cv;
int value = 180;
int main() {
	Mat img = imread("E:\\ddd.png");
	Mat Gray,res,res1;
	cvtColor(img, Gray, COLOR_BGR2GRAY);
	GaussianBlur(Gray, Gray, Size(9, 9), 2, 2);
	threshold(Gray, res, value, 255, THRESH_BINARY);
	double lower = 50;
	double upper = 200;
	Canny(res, res, lower, upper);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(res, res, kernel);

	vector<vector<Point>>contours;
	vector<Vec4i> hierarchy;
	findContours(res, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << contours.size() << endl;
	vector<vector<Point>>conPoly(contours.size());
	vector<Rect>boundRect(conPoly.size());
	//vector<Point2f>minrectpoint(4);
	Point max_point(0, 0), min_point(img.cols,img.rows);
	/*vector<Vec4i>lines;*/
	/*HoughLinesP(res, lines, 1, CV_PI / 180, 25,25,1);
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 2, LINE_AA);
	}*/
	for (int i = 0; i < contours.size(); i++) {
		RotatedRect minRect = minAreaRect(contours[i]);
		Point2f minrectpoint[4];
		minRect.points(minrectpoint);
		Size2f rectsize = minRect.size;
		if ( rectsize.width <= 1.2 * rectsize.height) {
			if (rectsize.width >= 0.8 * rectsize.height) {
				int areasize = contourArea(contours[i]);
				cout << areasize << endl;
				if (areasize > 100) {
					float peri = arcLength(contours[i], true);
					approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
					//drawContours(img, conPoly, i, Scalar(0, 255, 0), 2);
					cout << (int)conPoly[i].size() << endl;
					if ((int)conPoly[i].size() == 6) {
						boundRect[i] = boundingRect(conPoly[i]);
						//rectangle(img, boundRect[i], Scalar(0, 255, 0), 2);
						max_point.x = max(max_point.x, boundRect[i].br().x);
						max_point.y = max(max_point.y, boundRect[i].br().y);
						min_point.y = min(min_point.y, boundRect[i].tl().y);
						min_point.x = min(min_point.x, boundRect[i].tl().x);
					}
				}
			}
		}
		
		
		
	}

	rectangle(img, min_point, max_point, Scalar(0, 255, 0),2);
	putText(img, "Redemption stations",min_point, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0),2);
	
	imshow("image", img);
	imshow("result", res);
	waitKey(0);
}
