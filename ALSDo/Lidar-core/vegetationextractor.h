#ifndef EXTRACTTREE_H
#define EXTRACTTREE_H

#include "obasegment.h"

/**
* There several problem with the VegetationExtractor, and it should be improved ...
* including logic and performance etc.. 
*/
class LIDARCORE_API VegetationExtractor : public OBASegment
{
public:
	VegetationExtractor(WorkSpace *workSpace);
	virtual ~VegetationExtractor();

	// Segmentation 
	void segmentRegionBySeeds(const double &maxObjSize, const double &hIter, const double &aIter, const char* classid_path);
	void exportTrees(const char *topPath, const char *boundaryPath);

protected:
	// Dispose the garbage 
	virtual void dispose();

private:
	// Segmentation for deriving tree objects 
	void findSeeds(const double &maxObjSize, std::vector<PT> &seeds, const short *idM);
	// Just for segmentation
	void enterQueue(Entity *entity, PT &pt, const double &hT, const double &angleT, const short *idM, bool *flags);
};

#endif
