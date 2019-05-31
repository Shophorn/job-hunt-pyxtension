/*
© Leo Tamminen. Do not use without explicit permission.
shophorn@protonmail.com

Codetest program for job application
C extension for python, actual function definitions
*/

#include <Python.h>

///////////////////////////////////////////
/// C SIDE FUNCTIONS					///
///////////////////////////////////////////

/*
2d vector structure
*/
typedef struct { double x, y; } double2;

/*
Vanilla dot product between two vectors
*/
static double dot(double2 a, double2 b)
{
	return a.x * b.x + a.y * b.y;
}

/*
Compute 2d cross product that is used in determining direction of turn between edges.
Reduced from normal cross to yield length of imaginary z axis normal to xy plane.
*/
static double cross2d(double2 lhs, double2 rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

/*
Subtract one vectror from another duh
*/
static double2 subtract(double2 lhs,  double2 rhs)
{
	double2 result;
	
	result.x = rhs.x - lhs.x;
	result.y = rhs.y - lhs.y;

	return result;
}

/*
Parse 3 vectors to triangle array (double [3])

NOTE: triangle is expected to be of size 3, otherwise UB happens
*/
static void parseTrianglePoints(PyObject * args, double2 * triangle)
{
	PyArg_ParseTuple(
		args,
		"(dd)(dd)(dd)",
		&triangle[0].x, &triangle[0].y,
		&triangle[1].x, &triangle[1].y,
		&triangle[2].x, &triangle[2].y
	);
}

/*
Compute edge between a and b, and rotate 90 degrees.
This is not normalized, but we are only interested in signs of dot product so it doesn't matter
*/
static double2 getNormal(double2 a, double2 b)
{
	double2 edge = subtract(a, b);
	double2 normal;

	normal.x = edge.y;
	normal.y = -1 * edge.x;

	return normal;
};

/*
Test if three dot products are less tham or equal to 0.0.
Why: add readability and control 'equal to' part nicely in one place.
*/
static int testDotProducts(double a, double b, double c)
{
	return a <= 0.0 && b <= 0.0 && c <= 0.0;
	// return a < 0.0 && b < 0.0 && c < 0.0;
}

///////////////////////////////////////////
/// PYTHON SIDE FUNCTIONS				///
///////////////////////////////////////////

/*
Expose dot to python
*/
PyObject * py_dot(PyObject * self, PyObject * args)
{
	double2 a, b;
	PyArg_ParseTuple(args, "(dd)(dd)", &a.x, &a.y, &b.x, &b.y);

	return Py_BuildValue("d", dot(a, b));
}

/*
Expose cross2d to python
*/
PyObject * py_cross2d(PyObject * self, PyObject * args)
{
	double2 a, b;
	PyArg_ParseTuple(args, "(dd)(dd)", &a.x, &a.y, &b.x, &b.y);

	return Py_BuildValue("d", cross2d(a, b));	
}

/*
Order triangles made of 3 2d points, so that they are winded clockwise.
Return number of triangles flipped for whatever purposes
*/
PyObject * py_windTriangles(PyObject * self, PyObject * args)
{

	PyObject * triangleList;
	PyArg_ParseTuple(args, "O", &triangleList);

	int length = PyList_Size(triangleList);

	double2 triangle [3];
	double2 edge0, edge1;
	double cross;

	int flipCount = 0;

	for (int i = 0; i < length; i++)
	{
		PyObject * pyTriangle = PyList_GetItem(triangleList, i);
		parseTrianglePoints(pyTriangle, triangle);

		edge0 = subtract(triangle[1], triangle[0]);
		edge1 = subtract(triangle[2], triangle[1]);

		cross = cross2d(edge0, edge1);

		if (cross > 0.0)
		{
			flipCount++;

			PyTuple_SetItem(pyTriangle, 1, Py_BuildValue("(dd)", triangle[2].x, triangle[2].y));
			PyTuple_SetItem(pyTriangle, 2, Py_BuildValue("(dd)", triangle[1].x, triangle[1].y));
		}		
	}

	return Py_BuildValue("i", flipCount);
}

/*
Test if point (first argument) is inside any given triangle (other arguments, three points per triangle)
Return list of triangle incices that point is inside.

Return empty list if no list of triangles was given.
Depending on conventions, also error could be thrown in that case.

To optimize for memory parse and compute in single loop (CURRENT STYLE)
To optimize for speed parse triangles in separate loop
*/
PyObject * py_pointInsideTriangles(PyObject * self, PyObject * args)
{
	double2 point;
	PyObject * triangleList;

	PyArg_ParseTuple(args, "(dd)O", &point.x, &point.y, &triangleList);

	// Not list
	if (!PyList_Check(triangleList))
	{
		return PyList_New(0);
	}

	int length = PyList_Size(triangleList);

	PyObject * resultList = PyList_New(length);
	int resultCount = 0;

	double2 triangle[3];

	for(int i = 0; i < length; i++)
	{
		parseTrianglePoints(
			PyList_GetItem(triangleList, i),
			triangle
		);

		int isInside = testDotProducts (
			dot(subtract(triangle[0], point), getNormal(triangle[1], triangle[0])),
			dot(subtract(triangle[1], point), getNormal(triangle[2], triangle[1])),
			dot(subtract(triangle[2], point), getNormal(triangle[0], triangle[2]))
		);

		if (isInside) {
			PyList_SET_ITEM(resultList, resultCount, Py_BuildValue("i", i));
			resultCount++;
		}
	}

	return PyList_GetSlice(resultList, 0, resultCount);
}