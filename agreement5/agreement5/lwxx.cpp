#include <iostream>
#include<fstream>
#include<stdint.h>
#include<cstdio>
using namespace std;

struct Head
{
	uint32_t fps_h; //帧头
	uint32_t fps_length;  //协议长度
	uint32_t type;  //协议类型，0代表像素点，1代表屏幕大小
	uint32_t spare2;  //备用
};
struct Point
{
	uint32_t x;
	uint32_t y;
	uint32_t RBGA; //像素点颜色
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

//下面是写像素点
void write1()
{
	//数据准备
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
	Points points;  //在栈内存里为points开辟了一块内存空间
	points.num = 4;
	
	points.point = new Point[points.num];
	// new Point[points.num]为在堆内存中开辟了一块大小为points.num大小的空间
	
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
		points.num * sizeof(Point);//数据帧长度=头+尾+像素点+像素点个数所占的一个位置
	head.type = 0;
	head.spare2 = 0;
	//Tail
	Tail tail;
	tail.fps_t = 0xDDCCBBAA;

	//2.写文件 
	ofstream ofs_write;
	ofs_write.open("Binary", ios::binary | ios::out);
	if (ofs_write.is_open() == 1)//分开写入
	{
		//写头
		ofs_write.write((char*)(&head), sizeof(head));
		//写像素点个数
		ofs_write.write((char*)(&points.num), sizeof(points.num));
		//写入像素点
		for (int i = 0; i < points.num; i++)
		{
			ofs_write.write((char*)(&points.point[i]), sizeof(Point));
		}
		//写尾
		ofs_write.write((char*)(&tail), sizeof(Tail));
		//写之后关闭文件
		ofs_write.close();
	}
	delete[] points.point;
	points.point = nullptr;//指针置空，也可以是NULL
}

//下面是写像素点
void write2()
{
	//数据准备
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

	//获取X，Y的点数，由于在数据准备的时候只有4个点，循环最大为4
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
		sizeof(points.X_num) + sizeof(points.Y_num);//数据帧长度=头+尾+屏幕大小
	head.type = 1;
	head.spare2 = 0;
	
	//Tail
	Tail tail;
	tail.fps_t = 0xDDCCBBAA;

	//写入文件
	ofstream ofs_write;
	ofs_write.open("Binary", ios::binary | ios::out);
	if (ofs_write.is_open() == 1)
	{
		// 写入头
		ofs_write.write((char*)(&head), sizeof(Head));
		//写屏幕X_num
		ofs_write.write((char*)(&points.X_num), sizeof(points.X_num));
		//写屏幕Y_num
		ofs_write.write((char*)(&points.X_num), sizeof(points.X_num));
		// 写入尾
		ofs_write.write((char*)(&tail), sizeof(Tail));
		// 关闭文件
		ofs_write.close();
	}
}

// 下面是读像素点
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

		//下面将依次进行三次判断,并按照协议头，协议内容，协议尾的数据进行读；

		//第一次，判断读取的size是否>协议头长度，若小于则数据不完整，则不解析
		if (size < sizeof(Head))
		{
			ifs_read.close();
			return;
		}

		//第二次，读Head，并再次判断文件大小。像素点是否不大于10个，若大于10个，则不解析
		Head head;
		ifs_read.read((char*)(&head), sizeof(Head));
		if (size < head.fps_length)
		{
			ifs_read.close();
			return;
		}

		//先读points的num，再读points
		Points points;
		ifs_read.read((char*)&points.num, sizeof(points.num));
		points.point = new Point[points.num];
		for (int i = 0; i < points.num; i++) {
			ifs_read.read((char*)&points.point[i], sizeof(Point));//循环读取

		}

		//读Tail
		Tail tail;
		ifs_read.read((char*)&tail, sizeof(Tail));

		ifs_read.close();
		delete []points.point;

	}

}

//下面是屏幕大小读
void read2()
{
	Points points;

	ifstream ifs_read;
	ifs_read.open("Binary", ios::in | ios::binary);

	//下面进行一次判断，并按照协议头、屏幕大小、协议尾来读
	if (ifs_read.is_open() == 1)
	{
		ifs_read.seekg(0, ifs_read.end);
		streamoff size = ifs_read.tellg();
		ifs_read.seekg(0, ifs_read.beg);

		//判断判断读取的size是否>协议头长度，若小于则数据不完整，则不解析
		if (size < sizeof(Head))
		{
			ifs_read.close();
			return;
		}

		//读head
		Head head;
		ifs_read.read((char*)(&head), sizeof(Head));

		//读屏幕大小
		Points points;
		ifs_read.read((char*)&points.X_num, sizeof(points.X_num));
		ifs_read.read((char*)&points.Y_num, sizeof(points.Y_num));

		//读Tail
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
