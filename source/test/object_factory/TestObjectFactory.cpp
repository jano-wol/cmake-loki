#include <gmock/gmock.h>

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <sstream>
#include <vector>

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

  std::string command = "polygon 100 100 polygon 200 200 line 0 0 circle 10 10";
  std::stringstream ss(command);
  std::string typeStr;
  std::string coordStr;
  std::vector<std::unique_ptr<Shape>> shapes;
  while (ss >> typeStr) {
    std::unique_ptr<Shape> aPtr(factory.CreateObject(typeStr));
    ss >> coordStr;
    int x = std::stoi(coordStr);
    ss >> coordStr;
    int y = std::stoi(coordStr);
    aPtr->setPos({x, y});
    shapes.emplace_back(std::move(aPtr));
  }
  EXPECT_EQ(shapes[0]->calculate(), 203);
  EXPECT_EQ(shapes[1]->calculate(), 403);
  EXPECT_EQ(shapes[2]->calculate(), 2);
  EXPECT_EQ(shapes[3]->calculate(), 380);
  EXPECT_ANY_THROW(factory.CreateObject("cube"));
}