#ifndef OBACLASSIFIER_H
#define OBACLASSIFIER_H

#include "obasegment.h"

class LIDARCORE_API OBAClassifier : public OBASegment
{
public:
	OBAClassifier(WorkSpace *workSpace);
	virtual ~OBAClassifier();

	// Segment the region by max seeds
	void segmentRegionBySeeds(const double &maxObjSize, const double &segHeightTh, const double &segSlopeTh);
	// Classify entitys by default
	void classifyEntitys();
	// Classify entitys by paramTag
	void classifyEntitys(const string &paramTag, const double &valueMin, const double &valueMax);
	// Rest the type of entitys
	void resetEntitysType();

protected:
	// Method just for segmentation 
	virtual void enterQueue(PT& pt, PointType type, bool *enterQueueFlag, const double &segHeightTh, const double &segSlopeTh);

	virtual void dispose();

private:
	// Identify whether the 'useEntityParam' is firstly called
	bool _firstFlag;
};

#endif
