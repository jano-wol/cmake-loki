#include <gmock/gmock.h>

#include <loki/AbstractFactory.h>
#include <loki/Singleton.h>
#include <sstream>
#include <vector>

class Shape
{
public:
  virtual ~Shape(){};
};

class Polygon : public Shape
{
public:
  ~Polygon(){};
};

class Line : public Shape
{
public:
  ~Line(){};
};

class Circle : public Shape
{
public:
  ~Circle(){};
};

Polygon* createPolygon() { return new Polygon; }
Line* createLine() { return new Line; }
Circle* createCircle() { return new Circle; }

TEST(AbstractFactory, TestAbstractFactory)
{
}