#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "lasio.h"

LasIO::LasIO(string path)
{
	this->_create = false;
	this->_closed = false;
	this->_path = path;
	this->_fp = NULL;
	this->_header = NULL;
	this->_offsetPtNum = 0;

	memset(this->_buffer, 0, sizeof(LASPoint)*LAS_BUFFER);
	this->_bufferNum = 0;
}

LasIO::~LasIO()
{
	if(!this->_closed)
	{
		this->close();
	}

	if(this->_fp != NULL)
	{
		fflush(this->_fp);
		fclose(this->_fp);
		this->_fp = NULL;
	}
	if(this->_header != NULL)
	{
		delete this->_header;
		this->_header = NULL;
	}
}

LASHeader* LasIO::open(bool create)
{
	this->_create = create;

	if(this->_create)
	{
		if(this->_fp != NULL)
		{
			fclose(this->_fp);
			this->_fp = NULL;
		}
		if(this->_header != NULL)
		{
			delete this->_header;
			this->_header = NULL;
		}

		this->_header = new LASHeader;
		memset(this->_header, 0, sizeof(LASHeader));
		memcpy(this->_header->signature, "LASF", 4);
		//reserved, guid1234 -->0
		this->_header->version_major = 1;
		this->_header->version_minor = 0;
		//system_id --> ""
		memcpy(this->_header->software_id, "ALSDemo", 10);
		//julian_day, year -->0
		this->_header->header_size = sizeof(LASHeader);
		this->_header->data_offset = sizeof(LASHeader) + 2;
		//vlr_number, format_id --> 0
		this->_header->record_length = 20;
		this->_header->records_number = 0;
		this->_header->returns_number[0] = 0;
		//returns_number[1], returns_number[2], returns_number[3], returns_number[4] --> 0
		this->_header->xScale = 1.0;
		this->_header->yScale = 1.0;
		this->_header->zScale = 1.0;
		//xOffset, yOffset, zOffset --> 0

		this->_header->xMin = 999999999.0;
		this->_header->xMax = -999999999.0;
		this->_header->yMin = 999999999.0;
		this->_header->yMax = -999999999.0;
		this->_header->zMin = 999999999.0;
		this->_header->zMax = -999999999.0;

		this->_fp = fopen(this->_path.c_str(), "wb");
		assert(this->_fp != NULL);
		fwrite(this->_header, sizeof(LASHeader), 1, this->_fp);
		char offset[2] = {'\0', '\0'};
		fwrite(this->_header, sizeof(char), 2, this->_fp);
		fflush(this->_fp);
	}
	else
	{
		if(this->_fp != NULL)
		{
			fflush(this->_fp);
			fclose(this->_fp);
			this->_fp = NULL;
		}
		if(this->_header != NULL)
		{
			delete this->_header;
			this->_header = NULL;
		}

		this->_fp = fopen(this->_path.c_str(), "rb");
		if(this->_fp == NULL)
		{
			printf("%s (%d) -- %s: open las file error!\n", __FILE__, __LINE__ , __FUNCTION__);
			return NULL;
		}

		this->_header = new LASHeader;
		memset(this->_header, 0, sizeof(LASHeader));
		if(fread((char*)this->_header, sizeof(LASHeader), 1, this->_fp) != 1) 
		{
			printf("%s (%d) -- %s: read las header error!\n", __FILE__, __LINE__ , __FUNCTION__);
			if(this->_fp != NULL) 
			{
				fclose(this->_fp);	
				this->_fp = NULL;
			}
			return NULL;
		}
	}

	this->_offsetPtNum = 0;
	return this->_header;
}

int LasIO::offsetPtNum()
{
	return this->_offsetPtNum;
}

bool LasIO::readPts(LASPoint *pts, int &ptNum)
{
	int leftPtNum = 0, format_id = 0, offset = 0, read_num = 0, ii = 0;
	LASPoint *pt = NULL;
	
	if(this->_header == NULL) 
	{
		printf("%s (%d) -- %s: open las file first!\n", __FILE__, __LINE__ , __FUNCTION__);
		return false;
	}
	assert(ptNum == LAS_BUFFER);

	leftPtNum = this->_header->records_number - this->_offsetPtNum;
	if(leftPtNum <= 0)
	{
		ptNum = 0;
		return true;
	}
	
	//decide offset
	ptNum = leftPtNum <= LAS_BUFFER ? leftPtNum : LAS_BUFFER;
	format_id = this->_header->format_id;
	if(format_id == 0)
	{
		offset = this->_header->data_offset + this->_offsetPtNum * sizeof(LASRec0);
	}
	else if(format_id == 1)
	{
		offset = this->_header->data_offset + this->_offsetPtNum * sizeof(LASRec1);
	}
	fseek(this->_fp, offset, SEEK_SET);
	
	//read data points
	if(format_id == 0)
	{
		LASRec0 *rec = new LASRec0[ptNum];
		read_num = fread(rec, sizeof(LASRec0), ptNum, this->_fp);		
		assert(read_num == ptNum);

		if(read_num != ptNum)
		{
			printf("%s (%d) -- %s: bad las file records!\n", __FILE__, __LINE__ , __FUNCTION__);
			if(rec != NULL)
			{
				delete []rec;
				rec = NULL;
			}
			return false;
		}

		for(ii = 0; ii < ptNum; ++ii) 
		{
			pt = &pts[ii];
			pt->x = rec[ii].x * this->_header->xScale + this->_header->xOffset;
			pt->y = rec[ii].y * this->_header->yScale + this->_header->yOffset;
			pt->z = rec[ii].z * this->_header->zScale + this->_header->zOffset;			
			pt->intensity = rec[ii].intensity;
			pt->return_id = rec[ii].return_id;
			pt->return_number = rec[ii].return_number;
			pt->scan_direction = rec[ii].scan_direction;
			pt->flight_edge = rec[ii].flight_edge;
			pt->class_id = rec[ii].class_id;
			pt->scan_angle = rec[ii].scan_angle;
			pt->user_data = rec[ii].user_data;
			pt->source_id = rec[ii].source_id;
			//no gpstime for lasrec0
		}

		if(rec != NULL) 
		{
			delete []rec;
			rec = NULL;
		}
	}
	else if(format_id == 1)	
	{
		LASRec1 *rec = new LASRec1[ptNum];
		read_num = fread(rec, sizeof(LASRec1), ptNum, this->_fp);
		assert(read_num == ptNum);

		if(read_num != ptNum)
		{
			printf("%s (%d) -- %s: bad las file records!\n", __FILE__, __LINE__ , __FUNCTION__);
			if(rec != NULL)
			{
				delete []rec;
				rec = NULL;
			}
			return false;
		}

		for(ii = 0; ii < ptNum; ++ii) 
		{
			pt = &pts[ii];
			pt->x = rec[ii].x * this->_header->xScale + this->_header->xOffset;
			pt->y = rec[ii].y * this->_header->yScale + this->_header->yOffset;
			pt->z = rec[ii].z * this->_header->zScale + this->_header->zOffset;				
			pt->intensity = rec[ii].intensity;
			pt->return_id = rec[ii].return_id;
			pt->return_number = rec[ii].return_number;
			pt->scan_direction = rec[ii].scan_direction;
			pt->flight_edge = rec[ii].flight_edge;
			pt->class_id = rec[ii].class_id;
			pt->scan_angle = rec[ii].scan_angle;
			pt->user_data = rec[ii].user_data;
			pt->source_id = rec[ii].source_id;
			pt->gps_time = rec[ii].gps_time;
		}

		if(rec != NULL) 
		{
			delete []rec;
			rec = NULL;
		}
	}
	
	return true;
}

LASPoint* LasIO::readAllPts()
{
	int ii = 0, tmp_num = 0, pt_ii = 0;
	unsigned int pt_num = 0;
	LASPoint* pts = NULL, *pt = NULL;

	if(this->_header == NULL) 
	{
		printf("%s (%d) -- %s: open las file first!\n", __FILE__, __LINE__ , __FUNCTION__);
		return NULL;
	}

	pt_num = this->_header->records_number;
	pts = new LASPoint[pt_num];
	fseek(this->_fp, this->_header->data_offset, SEEK_SET);
	
	if(this->_header->format_id == 0)
	{
		LASRec0 *rec = new LASRec0[LAS_BUFFER];
		while (pt_num > 0)
		{
			tmp_num = fread(rec, sizeof(LASRec0), LAS_BUFFER, this->_fp);
			pt_num -= tmp_num;

			if (pt_num > 0 && tmp_num != LAS_BUFFER) 
			{
				printf("%s (%d) -- %s: read las file error!\n", __FILE__, __LINE__ , __FUNCTION__);
				if(rec != NULL) 
				{
					delete []rec;
					rec = NULL;
				}
				if(pts != NULL) 
				{
					delete []pts;
					pts = NULL;
				}
				return NULL;
			}			
			else 
			{
				if (pt_num < 0) 
				{
					tmp_num = this->_header->records_number % LAS_BUFFER;
				}

				for(ii = 0; ii < tmp_num; ++ii) 
				{
					pt = &pts[pt_ii];
					pt->x = rec[ii].x * this->_header->xScale + this->_header->xOffset;
					pt->y = rec[ii].y * this->_header->yScale + this->_header->yOffset;
					pt->z = rec[ii].z * this->_header->zScale + this->_header->zOffset;
					pt->intensity = rec[ii].intensity;
					pt->return_id = rec[ii].return_id;
					pt->return_number = rec[ii].return_number;
					pt->scan_direction = rec[ii].scan_direction;
					pt->flight_edge = rec[ii].flight_edge;
					pt->class_id = rec[ii].class_id;
					pt->scan_angle = rec[ii].scan_angle;
					pt->user_data = rec[ii].user_data;
					pt->source_id = rec[ii].source_id;
					pt_ii++;
				}
			}
		}
		if(rec != NULL) 
		{
			delete []rec;
			rec = NULL;
		}
	}
	else if(this->_header->format_id == 1)	
	{
		LASRec1 *rec = new LASRec1[LAS_BUFFER];			
		while (pt_num > 0) 
		{
			tmp_num = fread(rec, sizeof(LASRec1), LAS_BUFFER, this->_fp);
			pt_num -= tmp_num;

			if (pt_num >0 && tmp_num != LAS_BUFFER) 
			{
				printf("%s (%d) -- %s: read las file error!\n", __FILE__, __LINE__ , __FUNCTION__);

				if(rec != NULL)
				{
					delete []rec;
					rec = NULL;
				}
				if(pts != NULL) 
				{
					delete []pts;
					pts = NULL;
				}
				return NULL;
			}
			else
			{
				if (pt_num < 0)
				{
					tmp_num =  this->_header->records_number % LAS_BUFFER;
				}

				for(ii = 0; ii < tmp_num; ii++)
				{
					pt = &pts[pt_ii];
					pt->x = rec[ii].x * this->_header->xScale + this->_header->xOffset;
					pt->y = rec[ii].y * this->_header->yScale + this->_header->yOffset;
					pt->z = rec[ii].z * this->_header->zScale + this->_header->zOffset;
					pt->intensity = rec[ii].intensity;
					pt->return_id = rec[ii].return_id;
					pt->return_number = rec[ii].return_number;
					pt->scan_direction = rec[ii].scan_direction;
					pt->flight_edge = rec[ii].flight_edge;
					pt->class_id = rec[ii].class_id;
					pt->scan_angle = rec[ii].scan_angle;
					pt->user_data = rec[ii].user_data;
					pt->source_id = rec[ii].source_id;
					pt->gps_time = rec[ii].gps_time;
					pt_ii++;
				}
			}
		}
		if(rec != NULL)
		{
			delete []rec;
			rec = NULL;
		}
	}
	return pts;
}

//write las file by point
void LasIO::writePt(const LASPoint &pt, int pointType)
{
	if(this->_header == NULL) 
	{
		printf("%s (%d) -- %s: open las file first!\n", __FILE__, __LINE__ , __FUNCTION__);
		return;
	}
	if((pointType != -1) && (pt.class_id != pointType)) 
	{
		return;
	}

	this->_header->xMin = pt.x < this->_header->xMin ? pt.x : this->_header->xMin;
	this->_header->xMax = pt.x > this->_header->xMax ? pt.x : this->_header->xMax;
	this->_header->yMin = pt.y < this->_header->yMin ? pt.y : this->_header->yMin;
	this->_header->yMax = pt.y > this->_header->yMax ? pt.y : this->_header->yMax;
	this->_header->zMin = pt.z < this->_header->zMin ? pt.z : this->_header->zMin;
	this->_header->zMax = pt.z > this->_header->zMax ? pt.z : this->_header->zMax;

	this->_buffer[this->_bufferNum] = pt;
	this->_bufferNum++;

	if(this->_bufferNum == LAS_BUFFER)
	{
		this->writePts(this->_buffer, this->_bufferNum);
		this->_bufferNum = 0;
	}
}

void LasIO::writePts(LASPoint *pts, int ptNum, int pointType)
{
	int ii = 0, valid_pt_num = 0, format_id = 0, offset = 0, buffer_count = 0;
	LASPoint *pt = NULL;

	if(this->_header == NULL) 
	{
		printf("%s (%d) -- %s: open las file first!\n", __FILE__, __LINE__ , __FUNCTION__);
		return;
	}

	for(ii = 0; ii < ptNum; ++ii) 
	{
		pt = &pts[ii];		
		if((pointType != -1) && (pt->class_id != pointType)) 
		{
			continue;
		}

		valid_pt_num++;
		this->_header->xMin = pt->x < this->_header->xMin ? pt->x : this->_header->xMin;
		this->_header->xMax = pt->x > this->_header->xMax ? pt->x : this->_header->xMax;
		this->_header->yMin = pt->y < this->_header->yMin ? pt->y : this->_header->yMin;
		this->_header->yMax = pt->y > this->_header->yMax ? pt->y : this->_header->yMax;
		this->_header->zMin = pt->z < this->_header->zMin ? pt->z : this->_header->zMin;
		this->_header->zMax = pt->z > this->_header->zMax ? pt->z : this->_header->zMax;		
	}

	//decide offset
	format_id = this->_header->format_id;
	if(format_id == 0)
	{
		offset = this->_header->data_offset + this->_offsetPtNum * sizeof(LASRec0);
	}
	else if(format_id == 1)
	{
		offset = this->_header->data_offset + this->_offsetPtNum * sizeof(LASRec1);
	}
	fseek(this->_fp, offset, SEEK_SET);

	if(this->_header->format_id == 0)	
	{
		LASRec0 *rec = new LASRec0[LAS_BUFFER];
		buffer_count = 0;

		for(ii = 0; ii < ptNum; ++ii) 
		{
			pt = &pts[ii];
			if((pointType != -1) && (pt->class_id != pointType)) 
			{
				continue;
			}

			rec[buffer_count].x = (pt->x - this->_header->xOffset)/this->_header->xScale;
			rec[buffer_count].y = (pt->y - this->_header->yOffset)/this->_header->yScale;
			rec[buffer_count].z = (pt->z - this->_header->zOffset)/this->_header->zScale;
			rec[buffer_count].intensity = pt->intensity;
			rec[buffer_count].return_id = pt->return_id; 
			rec[buffer_count].return_number = pt->return_number;
			rec[buffer_count].scan_direction = pt->scan_direction;
			rec[buffer_count].flight_edge = pt->flight_edge;
			rec[buffer_count].class_id = pt->class_id;
			rec[buffer_count].scan_angle = pt->scan_angle;
			rec[buffer_count].user_data = pt->user_data;
			rec[buffer_count].source_id = pt->source_id;

			buffer_count++;
			if(buffer_count == LAS_BUFFER) 
			{
				fwrite(rec, sizeof(LASRec0), LAS_BUFFER, this->_fp);
				fflush(this->_fp);				
				buffer_count = 0;
			}
		}

		if(buffer_count != 0) 
		{
			fwrite(rec, sizeof(LASRec0), buffer_count, this->_fp);
			fflush(this->_fp);
		}

		if(rec != NULL) 
		{
			delete []rec;
			rec = NULL;
		}
	}
	else if(this->_header->format_id == 1)	
	{
		LASRec1 *rec = new LASRec1[LAS_BUFFER];			
		buffer_count = 0;

		for(ii = 0; ii < ptNum; ++ii) 
		{
			pt = &pts[ii];
			if((pointType != -1) && (pt->class_id != pointType)) 
			{
				continue;
			}

			rec[buffer_count].x = (pt->x - this->_header->xOffset)/this->_header->xScale;
			rec[buffer_count].y = (pt->y - this->_header->yOffset)/this->_header->yScale;
			rec[buffer_count].z = (pt->z  - this->_header->zOffset)/this->_header->zScale;
			rec[buffer_count].intensity = pt->intensity;
			rec[buffer_count].return_id = pt->return_id; 
			rec[buffer_count].return_number = pt->return_number;
			rec[buffer_count].scan_direction = pt->scan_direction;
			rec[buffer_count].flight_edge = pt->flight_edge;
			rec[buffer_count].class_id = pt->class_id;
			rec[buffer_count].scan_angle = pt->scan_angle;
			rec[buffer_count].user_data = pt->user_data;
			rec[buffer_count].source_id = pt->source_id;
			rec[buffer_count].gps_time = pt->gps_time;

			buffer_count++;
			if(buffer_count == LAS_BUFFER) 
			{
				fwrite(rec, sizeof(LASRec1), LAS_BUFFER, this->_fp);
				fflush(this->_fp);
				buffer_count = 0;
			}
		}

		if(buffer_count != 0)
		{
			fwrite(rec, sizeof(LASRec1), buffer_count, this->_fp);
			fflush(this->_fp);
		}

		if(rec != NULL) 
		{
			delete []rec;
			rec = NULL;
		}
	}

	this->_offsetPtNum += valid_pt_num;
	this->_header->records_number = this->_offsetPtNum;
}

void LasIO::close()
{
	if(this->_create)
	{
		if(this->_bufferNum != 0)
		{
			this->writePts(this->_buffer, this->_bufferNum);
			this->_bufferNum = 0;
		}

		this->_header->returns_number[0] = this->_header->records_number;
		fseek(this->_fp, 0, SEEK_SET);
		fwrite(this->_header, sizeof(LASHeader), 1, this->_fp);
		fflush(this->_fp);
	}

	if(this->_fp != NULL)
	{
		fflush(this->_fp);
		fclose(this->_fp);
		this->_fp = NULL;
	}

	this->_closed = true;
}

void LasIO::text2las(const char *text_path, const char *las_path)
{
	FILE *fp = NULL;	
	LasIO *lasIO = NULL;
	int ptN = 0, ii = 0, id = 0;
	double x = 0.0,y = 0.0,z = 0.0;
	LASPoint *pts = NULL;
	
	fp = fopen(text_path, "r");
	while(fscanf(fp, "%d %lf %lf %lf", &id, &x, &y, &z) != EOF) 
	{
		ptN++;
	}
	fseek(fp, 0, SEEK_SET);

	pts = new LASPoint[ptN];
	memset(pts, 0, ptN * sizeof(LASPoint));
	for(ii = 0; ii < ptN; ++ii) 
	{
		fscanf(fp, "%d %lf %lf %lf", &id, &x, &y, &z);
		pts[ii].x = x;
		pts[ii].y = y;
		pts[ii].z = z;
		//intensity, return_id, return_number, scan_direction, flight_edge --> 0
		pts[ii].class_id =  1;  //p_unclassified
		//scan_angle, user_data, source_id, gps_time  --> 0
	}
	fclose(fp);
	fp = NULL;

	lasIO = new LasIO(string(las_path));
	lasIO->open(true);
	lasIO->writePts(pts, ptN); 
	lasIO->close();

	if(pts != NULL)
	{
		delete []pts;
		pts = NULL;
	}
	if(lasIO != NULL)
	{
		delete lasIO;
		lasIO = NULL;
	}
}

void LasIO::las2text(const char *las_path, const char *text_path)
{
	LasIO *lasIO = NULL;
	LASHeader* header = NULL;
	LASPoint* pts = NULL;
	int ii = 0;
	FILE *fp = NULL;

	lasIO = new LasIO(string(las_path));
	header = lasIO->open();
	pts = lasIO->readAllPts();

	fp = fopen(text_path, "w+");
	for(ii = 0; ii < header->records_number; ++ii)
	{
		fprintf(fp, "%lf %lf %lf\n", pts[ii].x, pts[ii].y, pts[ii].z);
	}
	fflush(fp);
	fclose(fp);

	if(pts != NULL)
	{
		delete []pts;
		pts = NULL;
	}
	if(lasIO != NULL)
	{
		delete lasIO;
		lasIO = NULL;
	}
}

string& LasIO::lasPath()
{
	return this->_path; 
}

LASHeader* LasIO::lasHeader()
{ 
	return this->_header; 
}

