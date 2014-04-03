#ifndef OBAFILTER_H
#define OBAFILTER_H

#include "obasegment.h"

class LIDARCORE_API OBAFilter : public OBASegment
{
public:
	OBAFilter(WorkSpace *workSpace);
	virtual ~OBAFilter();

	// Segment the region by seeds
	void segmentRegionBySeeds(const double &seedHeightTh, const double &seedSlopeTh, const double &maxObjSize, 
		const double &seedCellSize, const double &segHeightTh, const double &segSlopeTh);
	// Filetering method 
	void filterRegion(const double &filteringHeightTh, const double &filteringSlopeTh, const double &dtmFactor, const double &objectHT);

protected:
	virtual void dispose();

private:
	// Filter Entitys
	void filterEntitys();
};

#endif