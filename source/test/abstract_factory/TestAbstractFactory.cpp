#include <gmock/gmock.h>

#include <loki/AbstractFactory.h>
#include <loki/Singleton.h>
#include <sstream>
#include <vector>

class Shape
{
public:
  virtual std::string name() const = 0;
  virtual ~Shape(){};
};

class Shape2D : public Shape
{
public:
  virtual std::string name() const = 0;
  virtual ~Shape2D(){};
};

class Shape3D : public Shape
{
public:
  virtual std::string name() const = 0;
  virtual ~Shape3D(){};
};

class Line : public Shape2D
{
public:
  ~Line(){};
  std::string name() const override { return "Line"; }
};

class Circle : public Shape2D
{
public:
  ~Circle(){};
  std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape2D
{
public:
  ~Rectangle(){};
  std::string name() const override { return "Rectangle"; }
};

class Plane : public Shape3D
{
public:
  ~Plane(){};
  std::string name() const override { return "Plane"; }
};

class Ball : public Shape3D
{
public:
  ~Ball(){};
  std::string name() const override { return "Ball"; }
};

class Box : public Shape3D
{
public:
  ~Box(){};
  std::string name() const override { return "Box"; }
};

typedef Loki::AbstractFactory<TYPELIST_2(Shape2D, Shape3D)> AbstractShapeFactory;
typedef Loki::ConcreteFactory<AbstractShapeFactory, Loki::OpNewFactoryUnit, TYPELIST_2(Line, Plane)>
    ShapeFactory2D;
/* typedef Loki::ConcreteFactory<AbstractShapeFactory, Loki::OpNewFactoryUnit, TYPELIST_3(Plane, Ball, Box)>
    ShapeFactory3D; */

TEST(AbstractFactory, TestAbstractFactory)
{
  typedef Loki::SingletonHolder<ShapeFactory2D, Loki::CreateUsingNew> Single2DFactory;
  auto& factory2D = Single2DFactory::Instance();
  Shape* pCircle = factory2D.Create<Line>();
  Shape* pPlane = factory2D.Create<Plane>();
}