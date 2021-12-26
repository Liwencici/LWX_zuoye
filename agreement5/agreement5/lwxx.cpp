#include <iostream>
#include<fstream>
#include<stdint.h>
#include<cstdio>
using namespace std;

struct Head
{
	uint32_t fps_h; //֡ͷ
	uint32_t fps_length;  //Э�鳤��
	uint32_t type;  //Э�����ͣ�0�������ص㣬1������Ļ��С
	uint32_t spare2;  //����
};
struct Point
{
	uint32_t x;
	uint32_t y;
	uint32_t RBGA; //���ص���ɫ
};
struct Points
{
	uint32_t num;
	Point	*point;
	uint32_t X_num;
	uint32_t Y_num;
};
struct Tail
{
	uint32_t fps_t;
	uint32_t spare_1;
	uint32_t spare_2;
	uint32_t spare_3;
};

//������д���ص�
void write1()
{
	//����׼��
	//point1
	Point point1;
	point1.x = 1;
	point1.y = 2;
	point1.RBGA = 0xFF0000AA;
	//point2
	Point point2;
	point2.x = 2;
	point2.y = 2;
	point2.RBGA = 0xFF0000AA;
	//point3
	Point point3;
	point3.x = 1;
	point3.y = 1;
	point3.RBGA = 0xFF0000AA;
	//point4
	Point point4;
	point4.x = 2;
	point4.y = 1;
	point4.RBGA = 0xFF0000AA;

	// points
	Points points;  //��ջ�ڴ���Ϊpoints������һ���ڴ�ռ�
	points.num = 4;
	
	points.point = new Point[points.num];
	// new Point[points.num]Ϊ�ڶ��ڴ��п�����һ���СΪpoints.num��С�Ŀռ�
	
	points.point[0] = point1;
	points.point[1] = point2;
	points.point[2] = point3;
	points.point[3] = point4;
	
	//Head
	Head head;
	head.fps_h = 0xAABBCCDD;
	head.fps_length = sizeof(Head) +
		sizeof(Tail) +
		sizeof(uint32_t) +
		points.num * sizeof(Point);//����֡����=ͷ+β+���ص�+���ص������ռ��һ��λ��
	head.type = 0;
	head.spare2 = 0;
	//Tail
	Tail tail;
	tail.fps_t = 0xDDCCBBAA;

	//2.д�ļ� 
	ofstream ofs_write;
	ofs_write.open("Binary", ios::binary | ios::out);
	if (ofs_write.is_open() == 1)//�ֿ�д��
	{
		//дͷ
		ofs_write.write((char*)(&head), sizeof(head));
		//д���ص����
		ofs_write.write((char*)(&points.num), sizeof(points.num));
		//д�����ص�
		for (int i = 0; i < points.num; i++)
		{
			ofs_write.write((char*)(&points.point[i]), sizeof(Point));
		}
		//дβ
		ofs_write.write((char*)(&tail), sizeof(Tail));
		//д֮��ر��ļ�
		ofs_write.close();
	}
	delete[] points.point;
	points.point = nullptr;//ָ���ÿգ�Ҳ������NULL
}

//������д���ص�
void write2()
{
	//����׼��
	//point1
	Point point1;
	point1.x = 1;
	point1.y = 2;
	point1.RBGA = 0xFF0000AA;
	//point2
	Point point2;
	point2.x = 2;
	point2.y = 2;
	point2.RBGA = 0xFF0000AA;
	//point3
	Point point3;
	point3.x = 1;
	point3.y = 1;
	point3.RBGA = 0xFF0000AA;
	//point4
	Point point4;
	point4.x = 2;
	point4.y = 1;
	point4.RBGA = 0xFF0000AA;

	Points points;
	points.num = 4;
	points.X_num = 0;
	points.Y_num = 0;
	points.point = new Point[points.num];
	points.point[0] = point1;
	points.point[1] = point2;
	points.point[2] = point3;
	points.point[3] = point4;

	//��ȡX��Y�ĵ���������������׼����ʱ��ֻ��4���㣬ѭ�����Ϊ4
	for (int i = 1; i <= 4; i++)
		for (points.point[i].y = i; i < 4; i++)
		{
			while (points.point[i].x)
				points.X_num++;
		}

	for (int i = 1; i <= 4; i++)
		for (points.point[i].x = i; i < 4; i++)
		{
			while (points.point[i].y)
				points.Y_num++;
		}

	//Head
	Head head;
	head.fps_h = 0xAABBCCDD;
	head.fps_length = sizeof(Head) +
		sizeof(Tail) +
		sizeof(points.X_num) + sizeof(points.Y_num);//����֡����=ͷ+β+��Ļ��С
	head.type = 1;
	head.spare2 = 0;
	
	//Tail
	Tail tail;
	tail.fps_t = 0xDDCCBBAA;

	//д���ļ�
	ofstream ofs_write;
	ofs_write.open("Binary", ios::binary | ios::out);
	if (ofs_write.is_open() == 1)
	{
		// д��ͷ
		ofs_write.write((char*)(&head), sizeof(Head));
		//д��ĻX_num
		ofs_write.write((char*)(&points.X_num), sizeof(points.X_num));
		//д��ĻY_num
		ofs_write.write((char*)(&points.X_num), sizeof(points.X_num));
		// д��β
		ofs_write.write((char*)(&tail), sizeof(Tail));
		// �ر��ļ�
		ofs_write.close();
	}
}

// �����Ƕ����ص�
void read1()
{
	Points points;

	ifstream ifs_read;
	ifs_read.open("Binary", ios::in | ios::binary);
	if (ifs_read.is_open() == 1)
	{
		ifs_read.seekg(0, ifs_read.end);
		streamoff size = ifs_read.tellg();
		ifs_read.seekg(0, ifs_read.beg);

		//���潫���ν��������ж�,������Э��ͷ��Э�����ݣ�Э��β�����ݽ��ж���

		//��һ�Σ��ж϶�ȡ��size�Ƿ�>Э��ͷ���ȣ���С�������ݲ��������򲻽���
		if (size < sizeof(Head))
		{
			ifs_read.close();
			return;
		}

		//�ڶ��Σ���Head�����ٴ��ж��ļ���С�����ص��Ƿ񲻴���10����������10�����򲻽���
		Head head;
		ifs_read.read((char*)(&head), sizeof(Head));
		if (size < head.fps_length)
		{
			ifs_read.close();
			return;
		}

		//�ȶ�points��num���ٶ�points
		Points points;
		ifs_read.read((char*)&points.num, sizeof(points.num));
		points.point = new Point[points.num];
		for (int i = 0; i < points.num; i++) {
			ifs_read.read((char*)&points.point[i], sizeof(Point));//ѭ����ȡ

		}

		//��Tail
		Tail tail;
		ifs_read.read((char*)&tail, sizeof(Tail));

		ifs_read.close();
		delete []points.point;

	}

}

//��������Ļ��С��
void read2()
{
	Points points;

	ifstream ifs_read;
	ifs_read.open("Binary", ios::in | ios::binary);

	//�������һ���жϣ�������Э��ͷ����Ļ��С��Э��β����
	if (ifs_read.is_open() == 1)
	{
		ifs_read.seekg(0, ifs_read.end);
		streamoff size = ifs_read.tellg();
		ifs_read.seekg(0, ifs_read.beg);

		//�ж��ж϶�ȡ��size�Ƿ�>Э��ͷ���ȣ���С�������ݲ��������򲻽���
		if (size < sizeof(Head))
		{
			ifs_read.close();
			return;
		}

		//��head
		Head head;
		ifs_read.read((char*)(&head), sizeof(Head));

		//����Ļ��С
		Points points;
		ifs_read.read((char*)&points.X_num, sizeof(points.X_num));
		ifs_read.read((char*)&points.Y_num, sizeof(points.Y_num));

		//��Tail
		Tail tail;
		ifs_read.read((char*)&tail, sizeof(Tail));

		ifs_read.close();
	}
}
int main()
{
	write1();
	//write2();
	//read1();
	//read2();
}
