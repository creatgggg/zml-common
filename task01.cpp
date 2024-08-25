#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	

	// 2D �������������꣬��������PS�ҳ���Ҳ����������¼�����������
	vector<Point2d> image_points;
	image_points.push_back(Point2d(100, 150));
	image_points.push_back(Point2d(300, 150));
	image_points.push_back(Point2d(300, 300));
	image_points.push_back(Point2d(100, 300));

	// �����ĸ�������
	

	// 3D �������������꣬�����������Ӧ����λ��mm
	std::vector<Point3d> model_points;
	model_points.push_back(Point3d(0, 0, 0)); // ���Ͻ�(-42.5mm,-42.5mm)
	model_points.push_back(Point3d(100,0, 0));
	model_points.push_back(Point3d(100,100, 0));
	model_points.push_back(Point3d(0,100, 0));
	//��ע�������������������Ҫһһ��Ӧ

	// ����ڲξ���ͻ���ϵ����������궨����ó�
	// ����ڲξ���
	Mat camera_matrix = (Mat_<double>(3, 3) << 1462.3697, 0, 398.59394,
		0,1469.68385, 110.68997,
		0, 0, 1);
	// �������ϵ��
	Mat dist_coeffs = (Mat_<double>(5, 1) << 0.003518,- 0.311778,
		-0.016581,0.023682,0.0000);

	cout << "Camera Matrix " << endl << camera_matrix << endl << endl;
	// ��ת����
	Mat rotation_vector;
	// ƽ������
	Mat translation_vector;

	// pnp���
	solvePnP(model_points, image_points, camera_matrix, dist_coeffs, \
		rotation_vector, translation_vector, 0, SOLVEPNP_ITERATIVE);
	// Ĭ��ITERATIVE�������ɳ����޸�ΪEPNP��CV_EPNP��,P3P��CV_P3P��

	cout << "Rotation Vector " << endl << rotation_vector << endl << endl;
	cout << "Translation Vector" << endl << translation_vector << endl << endl;

	Mat Rvec;
	Mat_<float> Tvec;
	rotation_vector.convertTo(Rvec, CV_32F);  // ��ת����ת����ʽ
	translation_vector.convertTo(Tvec, CV_32F); // ƽ������ת����ʽ 

	Mat_<float> rotMat(3, 3);
	Rodrigues(Rvec, rotMat);
	// ��ת����ת����ת����
	cout << "rotMat" << endl << rotMat << endl << endl;

	Mat P_oc;
	P_oc = -rotMat.inv() * Tvec;
	// ���������������꣬�ó�p_oc�ĵ�����Ԫ�ؼ����������ľ��뼴�����Ϣ����λ��mm
	cout << "P_oc" << endl << P_oc << endl;

	
	waitKey(0);
}
