#include "tin.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TIN_TRIANGLE(i) (i/3)
#define TIN_CORNER(i) (i%3)
#define TIN_INDEX(t,c) (t*3+c)
#define TIN_NEXT(c) ((c+1)%3)
#define TIN_PREV(c) ((c+2)%3)

#define MAX_LOCATE_STEPS 10000

/*************************************************************
this three functions are used to test geometry
exactinit is initialize function
************************************************************/
extern void exactinit();
/************************************************************
return value f>0 if pc is on the left side of line pa->pb, otherwise f < 0
if pa, pb, pc is in line f = 0
 ***********************************************************/
extern double orient2d(const double *pa, const double *pb, const double *pc);
/***********************************************************
to make it work right, pa->pb->pc should be counterclockwise,
lef c is the outer circle of triangle(pa, pb, pc), if pd in c, f > 0,
if pd is on c, f = 0, otherwise f < 0
***********************************************************/
extern double incircle(const double *pa, const double *pb, const double *pc, const double *pd);

//p in [a, b]
static int insegment(const double *a, const double *b, const double *p)
{
	if (a[0] < b[0])
	{
		if ((p[0] < a[0]) || (p[0] > b[0]))
		{
			return 0;
		}
		else if (p[0] == a[0])
		{
			return 1;
		}
		else if (p[0] == b[0])
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else if (a[0] > b[0])
	{
		if ((p[0] > a[0]) || (p[0] < b[0]))
		{
			return 0;
		}
		else if (p[0] == a[0])
		{
			return 1;
		}
		else if (p[0] == b[0])
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else if (a[1] < b[1])
	{
		if ((p[1] < a[1]) || (p[1] > b[1]))
		{
			return 0;
		}
		else if (p[1] == a[1])
		{
			return 1;
		}
		else if (p[1] == b[1])
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else if (a[1] > b[1])
	{
		if ((p[1] > a[1]) || (p[1] < b[1]))
		{
			return 0;
		}
		else if (p[1] == a[1])
		{
			return 1;
		}
		else if (p[1] == b[1])
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		fprintf(stderr, "FATAL ERROR: identical endpoints in insegment() test\n");
		return -1;
	}
}

// p locate on [a, b]
static bool onsegment(const double *a, const double *b, const double *p)
{
	if (a[0] < b[0])
	{
		return ((a[0] <= p[0]) && (p[0] <= b[0]));
	}
	else if (a[0] > b[0])
	{
		return ((a[0] >= p[0]) && (p[0] >= b[0]));
	}
	else if (a[1] < b[1])
	{
		return ((a[1] <= p[1]) && (p[1] <= b[1]));
	}
	else if (a[1] > b[1])
	{
		return ((a[1] >= p[1]) && (p[1] >= b[1]));
	}
	else
	{
		fprintf(stderr, "FATAL ERROR: identical endpoints in onsegment() test\n");
		return false;
	}
}

static bool incircle(const triangle *t, const double *p)
{
	if (t->V[0] == 0)  // infinite circle
	{
		double d = -orient2d(t->V[1], t->V[2], p);
		return (d < 0 ? true : (d > 0 ? false : onsegment(t->V[1], t->V[2], p)));
	}
	else // finite circle
	{
		double d = -incircle(t->V[0], t->V[1], t->V[2], p);
		return (d <= 0);
	}
}


TIN::TIN(int ptNum)
{
	this->triangle_buffer_alloc = 2*ptNum;
	this->triangle_buffer = NULL;
	this->triangle_buffer_size = 0;

	this->dfs_stack = NULL;
	this->dfs_stack_cur = 0;
	this->dfs_stack_max = 0;
}

TIN::~TIN()
{
	//free the points
	double *p = NULL;
	for (list<double *>::iterator it = p_list.begin(); it != p_list.end(); ++it)
	{
		p = *it;
		if (p != NULL)
		{
			delete []p;
			p = NULL;
		}
	}

	if(this->triangle_buffer != NULL) 
	{
		free(this->triangle_buffer);
		this->triangle_buffer = NULL;
	}
	this->stackDestroy();
}

bool TIN::initialize(double *v0, double *v1, double *v2)
{
	// initialize jonathan's predicate function
	exactinit();

	double *p0 = new double[3];
	p0[0] = v0[0];
	p0[1] = v0[1];
	p0[2] = v0[2];
	p_list.push_back(p0);

	double *p1 = new double[3];
	p1[0] = v1[0];
	p1[1] = v1[1];
	p1[2] = v1[2];
	p_list.push_back(p1);

	double *p2 = new double[3];
	p2[0] = v2[0];
	p2[1] = v2[1];
	p2[2] = v2[2];
	p_list.push_back(p2);
	// initialize triangle buffer

	if (triangle_buffer) 
	{
		free(triangle_buffer);
	}

	triangle_buffer = (triangle*)malloc(sizeof(triangle)*triangle_buffer_alloc);
	if (triangle_buffer == 0)
	{
		fprintf(stderr, "ERROR (TINclean): failed malloc for %d triangle buffer\n", triangle_buffer_alloc);
		return false;
	}

	for (int i = 0; i < triangle_buffer_alloc; i++)
	{
		triangle_buffer[i].next = i+1; // points to next unused triangle
	}
	triangle_buffer_size = 0;

	int i,j,k;
	// create the finite triangle, make sure that p0, p1, p2 are not on a line
	double orient = orient2d(p0, p1, p2);
	if (orient > 0)
	{
		triangle_buffer[0].V[0] = p0;
		triangle_buffer[0].V[1] = p1;
		triangle_buffer[0].V[2] = p2;
	}
	else if (orient < 0)
	{
		triangle_buffer[0].V[0] = p1;
		triangle_buffer[0].V[1] = p0;
		triangle_buffer[0].V[2] = p2;
	} 
	triangle_buffer[0].next = -1; // in use
	
	// create the other three infinite triangles
	triangle_buffer[1].next = -1; // in use
	triangle_buffer[1].V[0] = 0;
	triangle_buffer[1].V[1] = triangle_buffer[0].V[2];
	triangle_buffer[1].V[2] = triangle_buffer[0].V[1];

	triangle_buffer[2].next = -1; // in use
	triangle_buffer[2].V[0] = 0;
	triangle_buffer[2].V[1] = triangle_buffer[0].V[0];
	triangle_buffer[2].V[2] = triangle_buffer[0].V[2];

	triangle_buffer[3].next = -1; // in use
	triangle_buffer[3].V[0] = 0;
	triangle_buffer[3].V[1] = triangle_buffer[0].V[1];
	triangle_buffer[3].V[2] = triangle_buffer[0].V[0];
	
	// set up neighbor pointers
	for (i=0; i<4; i++)
	{
		for (j=i+1; j<4; j++)
		{
			int c1 = -1;
			int c2 = -1;
			for (k=0; k<3; k++)
			{
				if (triangle_buffer[i].V[k] != triangle_buffer[j].V[0] && triangle_buffer[i].V[k] != triangle_buffer[j].V[1] && triangle_buffer[i].V[k] != triangle_buffer[j].V[2])
				{
					assert(c1==-1);
					c1 = k;
				}
			}
			assert(c1!=-1);
			for (k=0; k<3; k++)
			{
				if (triangle_buffer[j].V[k] != triangle_buffer[i].V[0] && triangle_buffer[j].V[k] != triangle_buffer[i].V[1] && triangle_buffer[j].V[k] != triangle_buffer[i].V[2])
				{
					assert(c2==-1);
					c2 = k;
				}
			}
			assert(c2!=-1);
			triangle_buffer[i].N[c1] = TIN_INDEX(j,c2);
			triangle_buffer[j].N[c2] = TIN_INDEX(i,c1);
		}
	}
	triangle_newest = 0;
	triangle_next = 4;
	triangle_buffer_size = 4;
	return true;
}

triangle* TIN::locate(double *p)
{
	int steps = 0;
	double d;
	int c1;
	int c2;
	int ci;
	triangle* t = triangle_buffer + triangle_newest;

	// are point p and triangle t on the same side of the oriented edge (v1,v2) 

	if ((d = orient2d(p, t->V[1], t->V[2])) >= 0)
	{
		// yes ... use this oriented edge as the starting point
		ci = 0; 
	}
	else
	{
		// no ... try reversing the orientation of the edge using the adjacent triangle
		c1 = t->N[0];
		ci = TIN_CORNER(c1);
		t = triangle_buffer + TIN_TRIANGLE(c1);
	}

	c1 = TIN_NEXT(ci);
	c2 = TIN_PREV(ci);

	// early out?
	if (d == 0)
	{
		if ((steps = insegment(t->V[1], t->V[2], p)))
		{
			if (steps == 3)
			{
				return t;
			}
			else
			{
				return 0; // DUPLICATE_POINT
			}
		}
	}

	// compute q, the other end of the straight line we walk.
	double q[2];
	q[0] = ((double)(t->V[c2][0]) + (double)(t->V[c1][0]))/(double)(2.0);
	q[1] = ((double)(t->V[c2][1]) + (double)(t->V[c1][1]))/(double)(2.0);

	// walk a maximum of MAX_LOCATE_STEPS steps and maintain the edge (t, ci)
	while( (steps++) < MAX_LOCATE_STEPS )
	{
		assert (t->next == -1);
		if (t->V[0] == 0) // is the third vertex the infinite vertex?
		{
			if (d == 0)
			{
				// special handling because p is *on* the line defined by the finite edge of the infinite triangle
				// fprintf(stderr,"WARNING ... using special_locate\n");
				return this->locateSpecial(t, p);
			}
			else
			{
				assert(incircle(t,p));
				return t;
			}
		}  
		else if ((d = orient2d(p, q, t->V[ci])) > 0) // does the ray from q to p pass vertex t->V[ci] on the left
		{
			// if p lies past the corresponding triangle edge we continue walking
			c1 = TIN_NEXT(ci);
			if ((d = orient2d(p, t->V[c1], t->V[ci])) > 0)
			{
				c2 = t->N[TIN_PREV(ci)];
				ci = TIN_CORNER(c2);
				t = triangle_buffer + TIN_TRIANGLE(c2);
			}
			else if (d == 0) // if p lies on the line through the corresponding triangle edge we check
			{
				if ((c2 = insegment(t->V[c1], t->V[ci], p)))
				{
					if (c2 == 3)
					{
						assert(incircle(t,p));
						return t;
					}
					else
					{
						return 0; // DUPLICATE_POINT
					}
				}
				else
				{
					c2 = t->N[TIN_PREV(ci)];
					ci = TIN_CORNER(c2);
					t = triangle_buffer + TIN_TRIANGLE(c2);
				}
			}
			else // if p lies before the corresponding triangle edge we are done
			{
				assert(incircle(t,p));
				return t;
			}
		} 
		else // the ray from q to p pass passes t->V[ci] on the right or directly through it
		{
			// if the ray passes directly through t->V[ci] then p and t->V[ci] may be identical
			if (d == 0)
			{
				if (p[0] == t->V[ci][0] && p[1] == t->V[ci][1]) // p could be a duplicate of V[ci] 
				{
					return 0; // DUPLICATE_POINT
				}
			}
			// if p lies past the corresponding triangle edge we continue walking
			c2 = TIN_PREV(ci);
			if ((d = orient2d(p, t->V[ci], t->V[c2])) > 0)
			{
				c1 = t->N[TIN_NEXT(ci)];
				ci = TIN_CORNER(c1);
				t = triangle_buffer + TIN_TRIANGLE(c1);
			}
			else if (d == 0) // if p lies on the line through the corresponding triangle edge we check
			{
				if ((c1 = insegment(t->V[ci], t->V[c2], p)))
				{
					if (c1 == 3)
					{
						assert(incircle(t,p));
						return t;
					}
					else
					{
						return 0; // DUPLICATE_POINT
					}
				}
				else
				{
					c1 = t->N[TIN_NEXT(ci)];
					ci = TIN_CORNER(c1);
					t = triangle_buffer + TIN_TRIANGLE(c1);
				}
			}
			else // if p lies before the corresponding triangle edge we are done
			{
				assert(incircle(t,p));
				return t;
			}
		}
	}
	fprintf(stderr,"WARNING ... using brute_locate\n");
	return this->locateBrute(p);
}

bool TIN::add(double *p)
{
	// find a triangle whose circumcircle contains p
	triangle* t = this->locate(p);
	// discard duplicate points 
	if (t == 0)
	{
		return false;
	}

	// update triangulation with Bowyer-Watson
	double *p_new = new double[3];
	p_new[0] = p[0];
	p_new[1] = p[1];
	p_new[2] = p[2];
	p_list.push_back(p_new);
	this->update(t, p_new); 
	return true;
}

int TIN::tinCount()
{
	return triangle_buffer_size;
}

triangle* TIN::triangleIndex(int pos)
{
	return triangle_buffer + pos;
}

triangle* TIN::locateSpecial(triangle *t, double *p)
{
	do
	{
		assert(t->V[0] == 0);
		if (t->V[2][0] > t->V[1][0])
		{
			if (p[0] > t->V[2][0]) // is p beyond t->V[2]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[1]);
			}
			else if (p[0] < t->V[1][0]) // is p beyond t->V[1]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[2]);
			}
			else if (p[0] == t->V[2][0]) // is p a duplicate of t->V[2]
			{
				assert(p[1] == t->V[2][1]);
				return 0; // DUPLICATE_POINT
			}
			else if (p[0] == t->V[1][0]) // is p a duplicate of t->V[1]
			{
				assert(p[1] == t->V[1][1]);
				return 0; // DUPLICATE_POINT
			}
			else // found it
			{
				break;
			}
		}
		else if (t->V[2][0] < t->V[1][0])
		{
			if (p[0] < t->V[2][0]) // is p beyond t->V[2]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[1]);
			}
			else if (p[0] > t->V[1][0]) // is p beyond t->V[1]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[2]);
			}
			else if (p[0] == t->V[2][0]) // is p a duplicate of t->V[2]
			{
				assert(p[1] == t->V[2][1]);
				return 0; // DUPLICATE_POINT
			}
			else if (p[0] == t->V[1][0]) // is p a duplicate of t->V[1]
			{
				assert(p[1] == t->V[1][1]);
				return 0; // DUPLICATE_POINT
			}
			else // found it
			{
				break;
			}
		}
		else if (t->V[2][1] > t->V[1][1])
		{
			if (p[1] > t->V[2][1]) // is p beyond t->V[2]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[1]);
			}
			else if (p[1] < t->V[1][1]) // is p beyond t->V[1]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[2]);
			}
			else if (p[1] == t->V[2][1]) // is p a duplicate of t->V[2]
			{
				assert(p[0] == t->V[2][0]);
				return 0; // DUPLICATE_POINT
			}
			else if (p[1] == t->V[1][1]) // is p a duplicate of t->V[1]
			{
				assert(p[0] == t->V[1][0]);
				return 0; // DUPLICATE_POINT
			}
			else // found it
			{
				break;
			}
		}
		else if (t->V[2][1] < t->V[1][1])
		{
			if (p[1] < t->V[2][1]) // is p beyond t->V[2]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[1]);
			}
			else if (p[1] > t->V[1][1]) // is p beyond t->V[1]
			{
				t = triangle_buffer + TIN_TRIANGLE(t->N[2]);
			}
			else if (p[1] == t->V[2][1]) // is p a duplicate of t->V[2]
			{
				assert(p[0] == t->V[2][0]);
				return 0; // DUPLICATE_POINT
			}
			else if (p[1] == t->V[1][1])
			{
				assert(p[0] == t->V[1][0]); // is p a duplicate of t->V[1]
				return 0; // DUPLICATE_POINT
			}
			else // found it
			{
				break; 
			}
		}
		else
		{
			fprintf(stderr, "FATAL ERROR: degenerate finite edge of infinite triangle\n");
		}
	} while (orient2d(p, t->V[2], t->V[1]) == 0);

	assert(incircle(t,p));
	return t;
}

triangle* TIN::locateBrute(double *p)
{
	int i;
	triangle* t;
	double d[3];

	for (i = 0; i < triangle_buffer_alloc; i++)
	{
		t = triangle_buffer + i;
		if (t->next >= 0) continue;

		if (t->V[0] == 0) // infinite triangle?
		{
			if ((d[0] = orient2d(p, t->V[1], t->V[2])) >= 0)
			{
				if (d[0] == 0)
				{
					if ((d[0] = insegment(t->V[1], t->V[2], p)) > 0)
					{
						if (d[0] == 3)
						{
							return t;
						}
						else
						{
							return 0; // DUPLICATE_POINT
						}
					}
				}
				else
				{
					return t;
				}
			}
		}
		else
		{
			if ( ((d[0] = orient2d(p, t->V[1], t->V[2])) >= 0) &&
				((d[1] = orient2d(p, t->V[2], t->V[0])) >= 0) &&
				((d[2] = orient2d(p, t->V[0], t->V[1])) >= 0) )
			{
				if (d[0] == 0)
				{
					if (d[1] == 0)
					{
						assert((p[0] == t->V[2][0]) && (p[1] == t->V[2][1]));
						return 0; // DUPLICATE_POINT
					}
					else if (d[2] == 0)
					{
						assert((p[0] == t->V[1][0]) && (p[1] == t->V[1][1]));
						return 0; // DUPLICATE_POINT
					}
				}
				else if (d[1] == 0)
				{
					if (d[2] == 0)
					{
						assert((p[0] == t->V[0][0]) && (p[1] == t->V[0][1]));
						return 0; // DUPLICATE_POINT
					}
				}
				return t;
			}
		}
	}
	// we should never get here
	fprintf(stderr, "FATAL ERROR: fail to brute_locate a point\n");
	return 0;
}

void TIN::update(triangle *t, double *p)
{
	// the circumcircle of triangle tri_idx must enclose the new point p.
	int c;
	int ci;

	assert(this->stackEmpty());

	// add edges of seed triangle in CCW order to the stack
	for (ci = 0; ci < 3; ci++)
	{
		this->stackPush(t->N[ci]);
	}

	// delete the triangle
	t->next = triangle_next;
	triangle_next = t - triangle_buffer;

	int prev_idx = -1;
	int prev_ci = -1;
	int last_idx = -1;
	int last_ci = -1;

	while(!this->stackEmpty())
	{
		// get a corner from the horizon search stack
		c = this->stackPop();

		ci = TIN_CORNER(c);
		t = triangle_buffer + TIN_TRIANGLE(c);

		assert(t->next < 0);

		if (incircle(t,p)) // if the circumcircle of t encloses p we need to expand the horizon
		{
			// add the two unchecked edges of this triangle in CCW order to the stack
			for (c = 0; c < 2; c++)
			{
				ci = TIN_NEXT(ci);   
				this->stackPush(t->N[ci]);
			}
			// delete the triangle
			t->next = triangle_next;
			triangle_next = t - triangle_buffer;
		}
		else // we found a horizon edge. create the adjacent new triangle.
		{
			int tn_idx = triangle_next;
			triangle* tn = triangle_buffer + tn_idx;
			triangle_newest = tn_idx;
			triangle_next = tn->next;
			tn->next = -1;
			if (triangle_buffer_size < triangle_next) triangle_buffer_size = triangle_next;

			// one of the vertices on the horizon could be the infinite vertex ... make sure it is stored in tn->V[0] 
			if (ci == 1)
			{
				// assign vertices with correct orientation
				tn->V[0] = t->V[0];
				tn->V[1] = t->V[2];  
				tn->V[2] = p;
				// link the triangles across the horizon edge
				tn->N[2] = c;   
				t->N[1] = TIN_INDEX(tn_idx, 2);
				// link the new triangles around the new vertex
				if (prev_idx != -1) // does a previous triangle exist?
				{
					tn->N[0] = TIN_INDEX(prev_idx, prev_ci); // tn->N[0] should point to previous
					(triangle_buffer + prev_idx)->N[prev_ci] = TIN_INDEX(tn_idx, 0);
				}
				else // no ... this pointer will be set at the end
				{
					last_idx = tn_idx;
					last_ci = 0;
				}
				prev_idx = tn_idx;
				prev_ci = 1;
			}
			else if (ci == 2)
			{
				// assign vertices with correct orientation
				tn->V[0] = t->V[0];
				tn->V[1] = p;  
				tn->V[2] = t->V[1];  
				// link the triangles across the horizon edge
				tn->N[1] = c;   
				t->N[2] = TIN_INDEX(tn_idx, 1);    
				// link the new triangles around the new vertex
				if (prev_idx != -1) // does a previous triangle exist?
				{
					tn->N[2] = TIN_INDEX(prev_idx, prev_ci); // tn->N[2] should point to previous
					(triangle_buffer + prev_idx)->N[prev_ci] = TIN_INDEX(tn_idx, 2);
				}
				else // no ... this pointer will be set at the end
				{
					last_idx = tn_idx;
					last_ci = 2;
				}
				prev_idx = tn_idx;
				prev_ci = 0;
			}
			else
			{
				// assign vertices with correct orientation
				tn->V[0] = p;
				tn->V[1] = t->V[2];  
				tn->V[2] = t->V[1];  
				// link the triangles across the horizon edge
				tn->N[0] = c;   
				t->N[0] = TIN_INDEX(tn_idx, 0);    
				// link the new triangles around the new vertex
				if (prev_idx != -1) // does a previous triangle exist?
				{
					tn->N[1] = TIN_INDEX(prev_idx, prev_ci); // tn->N[1] should point to previous
					(triangle_buffer + prev_idx)->N[prev_ci] = TIN_INDEX(tn_idx, 1);
				}
				else // no ... this pointer will be set at the end
				{
					last_idx = tn_idx;
					last_ci = 1;
				}
				prev_idx = tn_idx;
				prev_ci = 2;
			}
		}
	}
	// link the last two triangles
	assert((prev_idx != -1) && (last_idx != -1));
	(triangle_buffer + last_idx)->N[last_ci] = TIN_INDEX(prev_idx, prev_ci);
	(triangle_buffer + prev_idx)->N[prev_ci] = TIN_INDEX(last_idx, last_ci);
}

bool TIN::stackEmpty()
{
	return (this->dfs_stack_cur == 0);
}

void TIN::stackPush(int value)
{
	if (this->dfs_stack_cur == this->dfs_stack_max) 
	{
		this->dfs_stack_max += 500; 
		this->dfs_stack = (int*)realloc(this->dfs_stack, sizeof(int)*this->dfs_stack_max);
	} 
	this->dfs_stack[this->dfs_stack_cur++] = value; 
}

int TIN::stackPop()
{
	return this->dfs_stack[--this->dfs_stack_cur];
}

void TIN::stackDestroy()
{ 
	if (this->dfs_stack != NULL) 
	{
		free(this->dfs_stack);
		this->dfs_stack = 0;
		this->dfs_stack_cur = 0; 
		this->dfs_stack_max = 0; 
	}
}
