/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Find closest pair of points using divide and conquer algorithm
 *
 *        Version:  1.0
 *        Created:  03/05/19 22:01:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Maciej M
 *        Company:
 *
 * =====================================================================================
 */

#include <utility>
#include <tuple>
#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>
#include <stdio.h>

using namespace std;

typedef std::pair<int, int> Point;
typedef tuple<double, Point, Point> DistancePointPoint;

int getPointX(const Point &p) {
    return p.first;
}

int getPointY(const Point &p) {
    return p.second;
}

double euclideanDistance(const Point &a, const Point &b) {
    double dx = (double) getPointX(a) - (double) getPointX(b),
            dy = (double) getPointY(a) - (double) getPointY(b);
    return sqrt(dx * dx + dy * dy);
}

double getTupleDistance(DistancePointPoint t) {
    return get<0>(t);
}

Point getTupleFirstPoint(DistancePointPoint t) {
    return get<1>(t);
}

Point getTupleSecondPoint(DistancePointPoint t) {
    return get<2>(t);
}

DistancePointPoint constructTuple(const Point &p1, const Point &p2, double distance) {
    DistancePointPoint tuple = make_tuple(distance, p1, p2);
    return tuple;
}

DistancePointPoint bruteForceClosest(const vector<Point> &points) {
    DistancePointPoint closest = make_tuple(numeric_limits<double>::max(), make_pair(0, 0), make_pair(0, 0));

    for (size_t i = 0; i < points.size(); i++)
        for (size_t j = 0; j < i; j++) { // <= can give as closest pairs as possible XD
            closest = min(closest,
                          constructTuple(points[i], points[j], euclideanDistance(points[i], points[j])));
        }

    return closest;
}

DistancePointPoint closestInStrip(const vector<Point> &leftHalfPoints, const vector<Point> &rightHalfPoints, double d) {
    //assert points are sorted by x value
    vector<Point> strip;

    {
        //set the iterator to the first element with distance d from median
        auto lHalfIterator = leftHalfPoints.begin();
        while (getPointX(*lHalfIterator) < getPointX(leftHalfPoints.back()) - d)
            lHalfIterator++;

        //set the iterator to the last element with distance d from median
        auto rHalfEndIterator = rightHalfPoints.begin();
        while (getPointX(*rHalfEndIterator) <= getPointX(rightHalfPoints.front()) + d
               && (rHalfEndIterator != rightHalfPoints.end()))
            rHalfEndIterator++; //mind this is past the end after loop finishes (as in C++ standard e.g. valu returned by vector end() method)
        strip.resize((leftHalfPoints.end() - lHalfIterator) +
                     (strip.size() + (size_t) (rHalfEndIterator - rightHalfPoints.begin())));
        int i = 0;
        for (; i < strip.size() && lHalfIterator != leftHalfPoints.end(); i++, lHalfIterator++)
            strip[i] = *lHalfIterator;
        for (auto it = rightHalfPoints.begin(); it != rHalfEndIterator && it != rightHalfPoints.end(); i++, it++)
            strip[i] = *it;
    }

    //sort by y
    sort(strip.begin(), strip.end(), [](auto &p1, auto &p2) { return getPointY(p1) < getPointY(p2); });

    DistancePointPoint closest = make_tuple(numeric_limits<double>::max(), make_pair(0, 0), make_pair(0, 0));
    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; getPointY(strip[j]) - getPointY(strip[i]) < d && j < strip.size(); j++) {
            closest = min(closest, constructTuple(strip[i], strip[j], euclideanDistance(strip[i], strip[j])));
        }
    }

    return closest;
}

DistancePointPoint closestPairAuxiliary(const vector<Point> &points) {
    if (points.size() < 2)
        //assertion error?
        return make_tuple(numeric_limits<double>::max(), make_pair(0, 0), make_pair(0, 0));

    if (points.size() == 2)
        return make_tuple(euclideanDistance(points[0], points[1]), points[0], points[1]);

    if (points.size() < 7)
        return bruteForceClosest(points);

    const size_t medianIndex = points.size() / 2;

    const Point median = points[medianIndex];

    vector<Point> leftHalfPoints(points.begin(), points.begin() + medianIndex);
    vector<Point> rightHalfPoints(points.begin() + medianIndex, points.end());

    DistancePointPoint leftHalfClosest = closestPairAuxiliary(leftHalfPoints),
            rightHalfClosest = closestPairAuxiliary(rightHalfPoints);

    DistancePointPoint closestInHalves = min(leftHalfClosest, rightHalfClosest);

    DistancePointPoint dLRMin = closestInStrip(leftHalfPoints, rightHalfPoints, getTupleDistance(closestInHalves));
    return min(closestInHalves, dLRMin); //the comparison uses first element, here the distance
}

DistancePointPoint closestPair(vector<Point> points) {
    //sort by x
    sort(points.begin(), points.end());

    return closestPairAuxiliary(points);
}

void printPoint(const Point &p) {
    printf("%d %d ", getPointX(p), getPointY(p));
    return;
}

int main() {
    size_t n;
    scanf("%lu", &n);

    vector<Point> points(n); //not [n] certainly haha

    for (size_t i = 0; i < n; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        points[i] = make_pair(x, y);
    }

    DistancePointPoint result = closestPair(points);

    printPoint(getTupleFirstPoint(result));
    printf("\n");
    printPoint(getTupleSecondPoint(result));

    return 0;
}