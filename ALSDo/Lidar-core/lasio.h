#ifndef LASIO_H
#define LASIO_H

#include "global.h"
#include <string>
using namespace std;

//227 bytes
#pragma pack(1)
typedef struct LASHeader
{
	char signature[4];	        			
	unsigned int reserved;                
	unsigned int guid1;                      
	unsigned short guid2;                                  
	unsigned short guid3;                   
	unsigned char guid4[8];                    
	unsigned char version_major;              
	unsigned char version_minor;              
	char system_id[32];              
	char software_id[32];              
	unsigned short julian_day;              
	unsigned short year;                   
	unsigned short header_size;              
	unsigned int data_offset;          //c          
	unsigned int vlr_number;                      
	unsigned char format_id;                  
	unsigned short record_length;            
	unsigned int records_number;       //c          
	unsigned int returns_number[5];               
	double xScale;                                 
	double yScale;                  
	double zScale;                  
	double xOffset;
	double yOffset;
	double zOffset;
	double xMax;                      //c
	double xMin;                      //c
	double yMax;                      //c
	double yMin;                      //c
	double zMax;                      //c
	double zMin;                      //c
}LASHeader;

//LAS fomartid=0, 20 bytes 
typedef struct LASRec0
{
	int x;
	int y;
	int z;
	unsigned short intensity;
	//回波号、回波数、扫描方向、飞行线边界
	unsigned char return_id : 3;
	unsigned char return_number : 3;
	unsigned char scan_direction : 1;
	unsigned char flight_edge : 1;	
	unsigned char class_id;
	char scan_angle;
	unsigned char user_data;
	unsigned short source_id; 
}LASRec0;

//LAS fomartid=1, 28 bytes
typedef struct LASRec1 
{
	int x;
	int y;
	int z;
	unsigned short intensity;
	//回波号、回波数、扫描方向、飞行线边界
	unsigned char return_id : 3;
	unsigned char return_number : 3;
	unsigned char scan_direction : 1;
	unsigned char flight_edge : 1;	
	unsigned char class_id;
	char scan_angle;
	unsigned char user_data;
	unsigned short source_id;
	double gps_time;
}LASRec1;

//LAS fomartid=3, 34 bytes
typedef struct LASRec3 
{
	int x;
	int y;
	int z;
	unsigned short intensity;
	//回波号、回波数、扫描方向、飞行线边界
	unsigned char return_id;
	unsigned char return_number;
	unsigned char scan_direction;
	unsigned char flight_edge;	
	unsigned short class_id;
	unsigned short scan_angle;
	unsigned short user_data;
	unsigned short source_id;
	double gps_time;
}LASRec3;

//data point, 40 bytes 
typedef struct LASPoint
{
	double x;
	double y;
	double z;

	unsigned short intensity;
	unsigned char return_id : 3;
	unsigned char return_number : 3;
	unsigned char scan_direction : 1;
	unsigned char flight_edge : 1;
	unsigned char class_id;
	char scan_angle;
	unsigned char user_data;
	unsigned short source_id;  //航带号	
	double gps_time;
}LASPoint;
#pragma pack()


// for las file i/o operation
class LIDARCORE_API LasIO
{
public:
	LasIO(string path);
	~LasIO();

	//open las file, while the las file information could be retrieved by lasheader
	LASHeader* open(bool create = false);
	//get offset point number
	int offsetPtNum();
	//read las file by sequence, the ptNum is usually LAS_BUFFER, it is the actually point number if the number of left points is less that LAS_BUFFER
	bool readPts(LASPoint *pts, int &ptNum);
	//read all las points
	LASPoint* readAllPts();
	//write las file by point
	void writePt(const LASPoint &pt, int pointType = -1); 
	//write las file by sequence
	void writePts(LASPoint *pts, int ptNum, int pointType = -1);
	//close las file
	void close();

	//convert text to las file
	static void text2las(const char *text_path, const char *las_path);
	//convert las to text file
	static void las2text(const char *las_path, const char *text_path); 

	string& lasPath();
	LASHeader* lasHeader();

private:
	bool _create;
	bool _closed;
	string _path;
	FILE *_fp;
	LASHeader* _header;
	int _offsetPtNum;
	LASPoint _buffer[LAS_BUFFER];
	int _bufferNum;
};

#endif
