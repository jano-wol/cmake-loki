#include <gmock/gmock.h>

#include <loki/AbstractFactory.h>
#include <loki/Singleton.h>
#include <sstream>
#include <vector>

class Shape
{
public:
  virtual std::string getName() const = 0;
  virtual ~Shape(){};
};

class Shape2D : public Shape
{
public:
  virtual std::string getName() const = 0;
  virtual ~Shape2D(){};
};

class Shape3D : public Shape
{
public:
  virtual std::string getName() const = 0;
  virtual ~Shape3D(){};
};

class Line : public Shape2D
{
public:
  ~Line(){};
  std::string getName() const override { return "Line"; }
};

class Circle : public Shape2D
{
public:
  ~Circle(){};
  std::string getName() const override { return "Circle"; }
};

class Rectangle : public Shape2D
{
public:
  ~Rectangle(){};
  std::string getName() const override { return "Rectangle"; }
};

class Plane : public Shape3D
{
public:
  ~Plane(){};
  std::string getName() const override { return "Plane"; }
};

class Ball : public Shape3D
{
public:
  ~Ball(){};
  std::string getName() const override { return "Ball"; }
};

class Box : public Shape3D
{
public:
  ~Box(){};
  std::string getName() const override { return "Box"; }
};

typedef Loki::AbstractFactory<TYPELIST_2(Shape2D, Shape3D)> AbstractShapeFactory;
typedef Loki::ConcreteFactory<AbstractShapeFactory, Loki::OpNewFactoryUnit, TYPELIST_2(Line, Plane)> LowDimension;

TEST(AbstractFactory, TestAbstractFactory)
{
  typedef Loki::SingletonHolder<LowDimension, Loki::CreateUsingNew> SingleLowDimensionFactory;
  auto& lowDimensionFactory = SingleLowDimensionFactory::Instance();
  Shape* pLine = lowDimensionFactory.Create<Shape2D>();
  Shape* pPlane = lowDimensionFactory.Create<Shape3D>();
  EXPECT_EQ(pLine->getName(), "Line");
  EXPECT_EQ(pPlane->getName(), "Plane");
}