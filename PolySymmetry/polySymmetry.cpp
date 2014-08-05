/*
 * given a simple polygon, determine if it is reflectively symmetric
    facts about polygons having reflective symmetry:
      the poly will have an equal number of points on either side of its line of symmetry.
      the poly may have a line of symmetry that not intersect any of its points (trapezoid for instance)

    facts about mirror symmetry across a line:
      two points A and B have mirror symmetry across a line, L,  if 
        1: the midpoint C, between A and B lies on L
        2: line L2(A, B) is at a right-angle to L
      a single point A has mirror symmetry across line L if
        1: A on the line

    so...
      if all points of a polygon have mirror symmetry across a line
      then the polygon itself has mirror symmetry across the line

    facts about lines-of-symmetry across polygons:
      There are three types of lines a polygon may be symmetrical across
        1. vertex-to-vertex: 
          a line which intersects two vertices of the polygon
          (example: a rhombus' line of symmetry)
        2. vertex-to-opposite-midpoint: 
          a line that intersects a vertex of the polygon and a point at the midpoint between two adjacent vertices of the polygon
          (example: an equalateral triangle's line of symmetry)
        3. midpoint-to-midpoint : 
          a line that intersects no vertices of the polygon, but intersects a pair of midpoints between two pairs of adjacent vertices on the polygon
          (example: a symmetric trapezoid's line of symmetry)


  Algorithm's strategy:
    Given a vector of N points representing a polygon
    Find all possible lines of symmetry (when N is odd, there are N lines. When N is even, there are 2N lines)
    For each of these lines, test each point for symmetry across the line.
    If all points have symmetry across the line, the polygon has symmetry across the line.

  Point symmetry algorithm:
    Given a line of symmetry L1(p1,p2) and two points q1, q2
    The points q1 and q2 have mirror symmetry across L1 if:
      A line Q1(q1,q2) intersects L1 at Q1's midpoint
        and
      Q1 is perpendicular to P1
  *
  */

    #include <vector>
    #include <iostream>

    class point {
      point() : _x(0), _y(0), _invalid(true){}
      bool _invalid;
    public:
      double _x, _y;

      point(double x, double y) : _x(x), _y(y), _invalid(false) {}
      bool isInvalid() {return _invalid;}
      static point midpoint (const point &a, const point &b) {
        if (a.isInvalid() || b.isInvalid()) return invalidPoint();
        else {
          // std::cout << "debug - returning midpoint between (" << a._x << "," << a._y << ") and (" << b._x << "," << b._y << ")\n";
          return point ( (a._x + b._x)/2, (a._y + b._y)/2 );
        }
      }
      bool equalTo(point a) {
        if (_invalid || a.isInvalid()) return false;
        else return (_x == a._x) && (_y == a._y); }
      static const point invalidPoint() { static point invalidpt = point(); return invalidpt;}
    };

    class line {
    private:
      point _a, _b;
    public:
      line (point a, point b): _a(a), _b(b) {
        // std::cout << "debug - created line (" << a._x <<"," << a._y << ")-(" << b._x <<"," << b._y << ")\n";
      }
      point midpoint() { return point::midpoint(_a,_b); };
      point intersection(const line &l) {
        // code adapted from examples at http://paulbourke.net/geometry/lineline2d/
        float denom  = (l._b._y-l._a._y)*(_b._x-_a._x) - (l._b._x-l._a._x)*(_b._y-_a._y);
        if (denom == 0.0f) // lines are parallel or coincident
          return point::invalidPoint();

        float uanum  = ((l._b._x-l._a._x)*(_a._y-l._a._y) - (l._b._y-l._a._y)*(_a._x-l._a._x))/denom;
        // ubnum not necessary, I think
        // float ubnum  = ((_b._x-_a._x)*(_a._y-l._a._y) - (_b._y-_a._y)*(_a._x-l._a._x))/denom;

        // solve for x and y of intersection
        return point(_a._x + uanum * (_b._x-_a._x), _a._y + uanum * (_b._y-_a._y));
      }
      bool isPerpendicularTo (const line &l) {
        // 
        // lines are perpendicular if the slope of one is the inverse negation of the other
        return  (_a._y - _b._y) * (l._a._y - l._b._y) == (_a._x - _b._x) * (l._b._x - l._a._x);
      }
    };
 
    class polygon {
      std::vector <point> vertices;
      int vertexCount;
      bool isOdd() { return vertexCount%2; }

      // for circular iteration over the vector
      int nextVertex(int i){ return (i+1 == vertexCount? 0: i+1); }
      int prevVertex(int i){ return (i-1 == -1? vertexCount-1: i-1); }

      // for finding indexes to the right vertexes in the vector
      int oppositeVertex(int i) {  // vertex 0's opposite in a 4-tex poly is 2
        if (isOdd()) throw "cannot operate on odd N";
        int opp = ((i+vertexCount/2) % vertexCount);
        // std::cout << "debug - oppositeVertex: i = " << i << ", opp = " << opp << "\n";
        return opp;
      }
      void oppositeVertices(int i, int &first, int &second) {
        if (!isOdd()) throw "cannot operate on even N";
        first = (i + vertexCount/2) % vertexCount;
        second = (i + 1 + vertexCount/2)%vertexCount;
      }
    public:
      polygon (std::vector <point> v) : vertices(v), vertexCount(vertices.size()) {}

      bool hasMirrorSymmetry() {
        bool polyHasSymmetry = false;
        bool pointsHaveSymmetry = false;
        if (isOdd()) {
          for (int i = 0; i < vertexCount; ++i) { // for each possible line of symmetry
            int ov1, ov2;
            oppositeVertices(i, ov1, ov2);

            line vertToMidSymmetricPossibility(vertices[i], point::midpoint(vertices[ov1],vertices[ov2]));
            // a forward-iteration and a reverse-iteration over the two halves of the poly's vertices
            while ((ov2 != prevVertex(i)) && (ov1 != nextVertex(i))) {
              line lineAcrossOppositeVertices(vertices[ov1], vertices[ov2]);
              pointsHaveSymmetry = 
                ( lineAcrossOppositeVertices.midpoint().equalTo(lineAcrossOppositeVertices.intersection(vertToMidSymmetricPossibility)) &&
                  lineAcrossOppositeVertices.isPerpendicularTo(vertToMidSymmetricPossibility));
              if (!pointsHaveSymmetry)
                break; // out of while-loop, if any pair fails, try the next possible line of symmetry
              ov1 = prevVertex(ov1);
              ov2 = nextVertex(ov2);
            }
            if (pointsHaveSymmetry) { // all points have symmetry
              polyHasSymmetry = true; // therefore the poly has symmetry
              break; // out of for-loop
            }
          }
        } else { // even
          for (int i = 0; i < vertexCount/2; ++i) {
            line vertToVertSymmetricPossibility(vertices[i], vertices[oppositeVertex(i)]);
            int ov1 = nextVertex(i);
            int ov2 = prevVertex(i);
            while (ov1 != oppositeVertex(i)) {
              line lineAcrossOppositeVertices(vertices[ov1], vertices[ov2]);
              pointsHaveSymmetry = 
                ( lineAcrossOppositeVertices.midpoint().equalTo(lineAcrossOppositeVertices.intersection(vertToVertSymmetricPossibility)) &&
                  lineAcrossOppositeVertices.isPerpendicularTo(vertToVertSymmetricPossibility));
              if (!pointsHaveSymmetry)
                break; // out of while-loop, if any pair fails, try the next possible line of symmetry
              ov1 = nextVertex(ov1);
              ov2 = prevVertex(ov2);
            }
            if (pointsHaveSymmetry) { // all points have symmetry
              polyHasSymmetry = true; // therefore the poly has symmetry
              break; // out of for-loop
            }

            ov1 = i;
            ov2 = nextVertex(ov1);
            // std::cout << "debug - ov1 = " << ov1 << " ov2 = " << ov2 << "\n";
            line midToMidSymmetricPossibility(
                  point::midpoint(vertices[ov1], vertices[nextVertex(ov1)]), 
                  point::midpoint(vertices[oppositeVertex(ov1)], vertices[nextVertex(oppositeVertex(ov1))]) ); // midpoint-to-midpoint case

            while (ov1 != oppositeVertex(i)) {
              line lineAcrossOppositeVertices(vertices[ov1], vertices[ov2]);
              pointsHaveSymmetry = 
                (lineAcrossOppositeVertices.midpoint().equalTo(lineAcrossOppositeVertices.intersection(midToMidSymmetricPossibility)) &&
                  lineAcrossOppositeVertices.isPerpendicularTo(midToMidSymmetricPossibility));
              if (!pointsHaveSymmetry) break; // out of while-loop, if any pair fails, try the next possible line of symmetry
              ov1=prevVertex(ov1);
              ov2=nextVertex(ov2);
            }
            if (pointsHaveSymmetry) { // all points have symmetry
              polyHasSymmetry = true; // therefore the poly has symmetry
              break; // out of for-loop
            }
          }
        }
        return polyHasSymmetry;
      }
    };

    void runSymmetryTest ( const char * name, bool expected_result, std::vector<point> &testVertices ) {
      std::cout << "Running test " << name << "...\n";
      std::cout << "  expected result (" << (expected_result? "true" : "false") << ")\n";
      try {
        bool actual_result(false);
        polygon testpoly(testVertices);
        actual_result = testpoly.hasMirrorSymmetry();
        std::cout << "  result " << (expected_result == actual_result? "Success!" : "Failure!") << "\n";
      }catch (char *e) {
        std::cout << "Exception caught: "<< e << "\n";
      }
      catch (...) {
        std::cout << "Unexpected exception caught... oops!\n";
      }
    }

    void test_poly_code(){
      std::vector <point> squareVertices;
      squareVertices.push_back(point(-1.0f, -1.0f));
      squareVertices.push_back(point(-1.0f, 1.0f));
      squareVertices.push_back(point(1.0f, 1.0f));
      squareVertices.push_back(point(1.0f, -1.0f));
      runSymmetryTest("square", true, squareVertices);

      std::vector <point> rectangleVertices;
      rectangleVertices.push_back(point(5.0f, 2.0f ));
      rectangleVertices.push_back(point(5.0f ,-2.0f ));
      rectangleVertices.push_back(point(-7.0f, -2.0f));
      rectangleVertices.push_back(point(-7.0f,2.0f ));
      runSymmetryTest("rectangle", true, rectangleVertices);

      std::vector <point> trapezoidVertices;
      trapezoidVertices.push_back(point(-2.0f, -1.0f));  // los should be midpoint (0,3) -> midpoint (1,2)
      trapezoidVertices.push_back(point(-1.0f, 1.0f)); 
      trapezoidVertices.push_back(point(1.0f, 1.0f)); 
      trapezoidVertices.push_back(point(2.0f, -1.0f));
      runSymmetryTest("symmetric trapezoid", true, trapezoidVertices);

      std::vector <point> pentagonishVertices;
      pentagonishVertices.push_back(point(-1.0f, 1.0f));
      pentagonishVertices.push_back(point(0.0f,2.0f));
      pentagonishVertices.push_back(point(1.0f,1.0f));
      pentagonishVertices.push_back(point(0.5f,0.0f));
      pentagonishVertices.push_back(point(-0.5f,0.0f));
      runSymmetryTest("symmetric 5-vertex poly", true, pentagonishVertices);

      std::vector <point> asymPoly1;
      asymPoly1.push_back(point(-0.3f, -4.5f));
      asymPoly1.push_back(point(-3.7f, 0.5f));
      asymPoly1.push_back(point(-1.7f,1.5f));
      asymPoly1.push_back(point(1.5f,1.5f));
      asymPoly1.push_back(point(2.7f,-3.4f));
      asymPoly1.push_back(point(-3.3f, -2.0f));
      asymPoly1.push_back(point(-0.3f,-2.0f));
      runSymmetryTest("asymetric 7-vertex poly", false, asymPoly1);

      std::vector <point> asymPoly2;
      asymPoly2.push_back(point(-0.3f, -4.5f));
      asymPoly2.push_back(point(-3.7f, 0.5f));
      asymPoly2.push_back(point(-1.7f,1.5f));
      asymPoly2.push_back(point(1.5f,1.5f));
      asymPoly2.push_back(point(2.7f,-3.4f));
      asymPoly2.push_back(point(-3.3f, -2.0f));
      runSymmetryTest("asymetric 6-vertex poly", false, asymPoly1);
    }

    int main (void) {
      test_poly_code();
    }

