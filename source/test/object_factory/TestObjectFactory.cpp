#include <gmock/gmock.h>

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <iostream>

class Shape
{
public:
  virtual int calculate() = 0;
  virtual ~Shape(){};
};

class Polygon : public Shape
{
public:
  Polygon(int i_) : i(i_) {}
  ~Polygon(){};

  int calculate() override { return i + 3; }

private:
  int i;
};

class Line : public Shape
{
public:
  Line(int i_) : i(i_) {}
  ~Line(){};

  int calculate() override { return i + 2; }

private:
  int i;
};

class Circle : public Shape
{
public:
  Circle(int i_) : i(i_) {}
  ~Circle(){};

  int calculate() override { return i + 360; }

private:
  int i;
};

Polygon* createPolygon() { return new Polygon(0); }
Line* createLine() { return new Line(0); }
Circle* createCircle() { return new Circle(0); }

TEST(ObjectFactory, TestObjectFactory)
{
  typedef Loki::Factory<Shape, int> FactoryType;
  typedef Loki::SingletonHolder<FactoryType, Loki::CreateUsingNew> SingleFactory;
  auto& factory = SingleFactory::Instance();
  factory.Register(0, (Shape * (*)()) createPolygon);
  factory.Register(1, (Shape * (*)()) createLine);
  factory.Register(2, (Shape * (*)()) createCircle);

  auto* a = factory.CreateObject(0);
  auto* b = factory.CreateObject(1);
  auto* c = factory.CreateObject(2);
  EXPECT_EQ(a->calculate(), 3);
  EXPECT_EQ(b->calculate(), 2);
  EXPECT_EQ(c->calculate(), 360);
}