#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef LIDARCORE_EXPORTS
	#define LIDARCORE_API __declspec(dllexport)
#else
	#define LIDARCORE_API __declspec(dllimport)
#endif

#define MAX_STRING_BUFFER 256        // Max string length
#define LAS_BUFFER 100000            // Max number of las points reading from las file
#define MAX_LAS_POINTS 1.0e7         // Max number that the software could process

#define NULL 0                       // For null pointer
#define NAN -9999                    // Not a number
#define MINV -999999999.0            // Minimum value
#define MAXV 999999999.0             // Maximum value
#define PI 3.1415926                 // Appropriate value of pi
#define MIN_PT_DENSITY 1.0           // Minimum point number in each grid cell of grid index

#define LOG_OUT 1

/**
* For segmentation, if an object starts from a ground seed point, its type is set to p_ground; if an object starts 
* from a point unkonwn, its type is set to p_unclassified; if an object starts from a hole point, its type is set to p_created
*/
enum PointType
{
	p_created = 0, //never for classification
	p_unclassified = 1,
	p_ground = 2,
	p_low_vegetation = 3,
	p_medium_vegetation = 4,
	p_high_vegetation = 5,
	p_building = 6,
	p_low_point = 7,
	p_high_point = 8,
	p_water = 9,
	p_bridge = 10,
	p_road = 11,
	p_miscellaneous_structure = 12,
	p_railroad = 13,
	p_stream = 14,
	p_power_line = 15,
	p_power_line_tower = 16,
	p_power_line_tower_to_wire_connection = 17,
	// from 18 is just used for my application
	p_nonground = 18,
	p_tree_conifer = 19,               
	p_tree_deciduous = 20,
	p_reserved = 21
};

/**
* Structure used for neighborhood index and virtual entity
*/
typedef struct PT 
{
	PT()
	{
		this->x = NAN;
		this->y = NAN;
	}

	PT(const int &x, const int &y)
	{
		this->x = x;
		this->y = y;
	}

	bool operator==(const PT &pt) const
	{
		return ((this->x == pt.x) && (this->y == pt.y));
	}

	bool operator!=(const PT &pt) const
	{
		return ((this->x != pt.x) || (this->y != pt.y));
	}

	int x;
	int y;
}PT;

typedef struct PT3D
{
	PT3D()
	{
		this->pos = -1;
		this->x = NAN;
		this->y = NAN;
		this->z = NAN;
		this->type = PointType::p_created;
	}

	PT3D(const int &pos, const double &x, const double &y, const double &z, const PointType &type)
	{
		this->pos = pos;
		this->x = x;
		this->y = y;
		this->z = z;
		this->type = type;
	}

	/* Index position of grid cell */
	int pos;   
	/* (x, y, z) of the point */
	double x;    
	double y;
	double z;
	/* Point type */
	PointType type;
}PT3D;

/*
typedef struct BOX3D 
{
	BOX3D()
	{
		this->xMin = MINV;
		this->xMax = MAXV;
		this->yMin = MINV;
		this->yMax = MAXV;
		this->zMin = MINV;
		this->zMax = MAXV;
	}

	BOX3D(const double &xMin, const double &xMax, const double &yMin, const double &yMax,
		const double &zMin, const double &zMax)
	{
		this->xMin = xMin;
		this->xMax = xMax;
		this->yMin = yMin;
		this->yMax = yMax;
		this->zMin = zMin;
		this->zMax = zMax;
	}

	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double zMin;
	double zMax;
};
*/

#endif