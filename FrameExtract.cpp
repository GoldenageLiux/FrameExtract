#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <direct.h>

using namespace std;
using namespace cv;

void splitWithStrtok(const char* str, const char* delim, vector<string>& ret)
{
	char* strcopy = new char[strlen(str) + 1];
	strcpy(strcopy, str);
	char* word = strtok(strcopy, delim);
	ret.push_back(word);
	while (word = strtok(nullptr, delim))
		ret.push_back(word);
	delete[] strcopy;
}

int main()
{
	
	printf("usage:   ��Ƶ��ַ ��֡���\n");
	printf("example: F:/FrameExtract/test.mp4 5\n");
	
	char videopath[100];
	cin >> videopath;
	int interval;
	cin >> interval;

	string folderPath;
	vector<string> folderPaths;
	string fileName;
	vector<string> fileNames;
	splitWithStrtok(videopath, "/", folderPaths);
	for (int i = 0; i < folderPaths.size() - 1; i++) {
		folderPath += folderPaths[i];
		folderPath += "/";
	}
	splitWithStrtok(folderPaths.back().c_str(), ".", fileNames);
	fileName = fileNames[0];
	folderPath += fileName;
	folderPath += "/";

	cout << folderPath;
	//����Ƶͬ��Ŀ¼�´����ļ��У��ļ���������Ƶ��һ��
	
	/*string command;
	command = "md " + folderPath;
	system(command.c_str());*/
	_mkdir(folderPath.c_str());
	

	VideoCapture capture(videopath);
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture
	if (!capture.isOpened())
		cout << "fail to open!" << endl;

	//��ȡ����֡��
	long totalFrameNumber = capture.get(CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;


	//���ÿ�ʼ֡()
	long frameToStart = 0;
	capture.set(CAP_PROP_POS_FRAMES, frameToStart);
	cout << "�ӵ�" << frameToStart << "֡��ʼ��" << endl;

	//���ý���֡
	int frameToStop = totalFrameNumber - 1;

	if (frameToStop < frameToStart)
	{
		cout << "����֡С�ڿ�ʼ֡��������󣬼����˳���" << endl;
		return -1;
	}
	else
	{
		cout << "����֡Ϊ����" << frameToStop << "֡" << endl;
	}

	//��ȡ֡��
	double rate = capture.get(CAP_PROP_FPS);
	cout << "֡��Ϊ:" << rate << endl;


	//����һ���������ƶ�ȡ��Ƶѭ�������ı���
	bool stop = false;

	//����ÿһ֡��ͼ��
	Mat frame;

	//��ʾÿһ֡�Ĵ���
	//namedWindow( "Extractedframe" );

	//��֡��ļ��ʱ��:
	//int delay = 1000/rate;
	double delay = 1000 / rate;


	//����whileѭ����ȡ֡
	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���
	long currentFrame = frameToStart;


	while (!stop)
	{
		//��ȡ��һ֡
		if (!capture.read(frame))
		{
			cout << "��Ƶ��֡����" << endl;
			return -1;
		}

		//cout << "���ڶ�ȡ��" << currentFrame << "֡" << endl;
		/*namedWindow("Extractedframe", 0);
		imshow("Extractedframe", frame);*/

		//�˴�Ϊ��֡����
		if (currentFrame % interval == 0) //֡�����
		{
			cout << "����д��" << currentFrame << "֡" << endl;
			stringstream str;
			str << folderPath << fileName << "_" << currentFrame << ".jpg";        /*ͼƬ�洢λ��*/

			cout << str.str() << endl;
			imwrite(str.str(), frame);
		}

		//waitKey(intdelay=0)��delay�� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���
		int c = waitKey(delay);
		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}

	//�ر���Ƶ�ļ�
	capture.release();
	waitKey(0);
	return 0;
}