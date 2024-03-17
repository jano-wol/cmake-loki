#include <gmock/gmock.h>

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <iostream>

class Shape
{
public:
  virtual ~Shape(){};
  virtual int calculate() const = 0;

  void setPos(std::pair<int, int> pos_) { pos = std::move(pos_); }

protected:
  std::pair<int, int> pos;
};

class Polygon : public Shape
{
public:
  ~Polygon(){};
  int calculate() const override { return pos.first + pos.second + 3; }
};

class Line : public Shape
{
public:
  ~Line(){};
  int calculate() const override { return pos.first + pos.second + 2; }
};

class Circle : public Shape
{
public:
  ~Circle(){};
  int calculate() const override { return pos.first + pos.second + 360; }
};

Polygon* createPolygon() { return new Polygon; }
Line* createLine() { return new Line; }
Circle* createCircle() { return new Circle; }

TEST(ObjectFactory, TestObjectFactory)
{
  typedef Loki::Factory<Shape, std::string> FactoryType;
  typedef Loki::SingletonHolder<FactoryType, Loki::CreateUsingNew> SingleFactory;
  auto& factory = SingleFactory::Instance();
  factory.Register("polygon", (Shape * (*)()) createPolygon);
  factory.Register("line", (Shape * (*)()) createLine);
  factory.Register("circle", (Shape * (*)()) createCircle);

  auto* a1 = factory.CreateObject("polygon");
  auto* a2 = factory.CreateObject("polygon");
  a1->setPos({100, 100});
  a2->setPos({200, 200});
  auto* b = factory.CreateObject("line");
  auto* c = factory.CreateObject("circle");
  EXPECT_EQ(a1->calculate(), 203);
  EXPECT_EQ(a2->calculate(), 403);
  EXPECT_EQ(b->calculate(), 2);
  EXPECT_EQ(c->calculate(), 360);
  EXPECT_ANY_THROW(factory.CreateObject("cube"));
}