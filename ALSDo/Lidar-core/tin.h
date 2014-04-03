#ifndef TIN_H
#define TIN_H

#include <list>
using namespace std;

typedef struct triangle 
{
  double *V[3];
  int N[3];
  int next;
}triangle;

class TIN
{
public:
	TIN(int ptNum);
	~TIN();

	bool initialize(double *v0, double *v1, double *v2);
	triangle* locate(double *p);
	bool add(double *p);
	int tinCount();
	triangle* triangleIndex(int pos);

private:
	triangle* locateBrute(double *p);
	triangle* locateSpecial(triangle *t, double *p);
	void update(triangle *t, double *p);

	bool stackEmpty();
	void stackPush(int value);
	int stackPop();
	void stackDestroy();

private:
	triangle *triangle_buffer;
	int triangle_buffer_alloc;
	int triangle_buffer_size;
	int triangle_next;
	int triangle_newest;

	// for depth first search for Bowyer & Watson horizon expansion
	int *dfs_stack;
	int dfs_stack_cur;
	int dfs_stack_max;

	list<double *>p_list;
};

#endif
