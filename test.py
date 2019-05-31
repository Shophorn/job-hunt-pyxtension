# © Leo Tamminen. Do not use without explicit permission.
# shophorn@protonmail.com

# Codetest program for job application
# Python c extension test program

import codetest
from random import random
import time

# Define tests.
# Look for following line to locate c functions usage done for this program
# -------------------- Codetest c funtion --------------------

# This tests a few simple triangles, so viewer can easily confirm correctness
# by visualizing mentally or with pen and paper (ie. without computer)
def testForCorrectness():
	points = [
		(1, 1),		# far corner, not inside
		(3, 3),		# low mid corner, inside t0
		(7, 3),		# low mid corner, inside t0
		(5, 5),		# middle, inside both
		(3, 7),		# high mid corner, inside t1
		(7, 7),		# high mid corner, inside t2
		(9, 9)		# far corner, not inside
	]

	triangles = [
		# Upward and downward pointing triangels roughly in middle of area
		((5.0, 7.5), (7.5, 2.5), (2.5, 2.5)),
		((5.0, 2.5), (2.5, 7.5), (7.5, 7.5)),

		# huge triangle, all are inside
		((0, 0), (0, 20), (20, 0)),

		# tiny triangle, only contains middle point
		((4.99, 4.99), (5.0, 5.01), (5.01, 4.99))
	]

	print("\ttriangles:")
	for i in range(len(triangles)):	
		print("\t\t{}: {}".format(i, triangles[i]))

	print("\n\tPoints listed with matching triangle indices")
	for p in points:

		# -------------------- Codetest c funtion --------------------
		print ("\tpoint {}: {}".format(p, codetest.pointInsideTriangles(p, triangles)))	


# This tests against time. Change pointCount and triangleCount if wanted to experiment
# with different amounts.
def testForVolume():
	pointCount = 1000
	triangleCount = 10000
	area = 10

	# Generate random point
	def randomPoint():
		return (random() * area, random() * area)

	# Generate random triangle
	def randomTriangle():
		return (randomPoint(), randomPoint(), randomPoint())
	

	print("\tGenerating random points and triangles in {0} by {0} square".format(area))
	print("\t{} points\n\t{} triangles".format(pointCount, triangleCount))

	points = [randomPoint() for i in range(pointCount)]
	triangles = [randomTriangle() for i in range(triangleCount)]

	# Orient random triangles properly in advance so that we 
	# don't need to check each time we test against them.
	# -------------------- Codetest c function --------------------
	flipCount = codetest.windTriangles(triangles);
	print("\t{} triangles converted to clockwise winding".format(flipCount))


	# Test triangle inclusions
	print ("\tNow testing all points against all triangles...")
	startTime = time.time()

	# -------------------- Codetest c function --------------------
	isInsides = [codetest.pointInsideTriangles(p, triangles) for p in points]
	endTime = time.time()

	# Random statistic
	pointInsideCount = 0
	for item in isInsides:
		if len (item) > 0:
			pointInsideCount += 1

	percent = pointInsideCount / pointCount * 100
	print ("\t{:0.2f}% of points were inside at least one triangle".format(percent))

	totalTime = endTime - startTime
	singlePointTime = totalTime / pointCount
	print (
		"\ttotal time: {:0.4f} s\n\ttime per point: {:0.4f} s (against {} triangles)"
		.format(totalTime, singlePointTime, triangleCount))


# Do tests
print ("Accurate test")
testForCorrectness()

print("\nBig test")
testForVolume()

print ("\nAll Done")