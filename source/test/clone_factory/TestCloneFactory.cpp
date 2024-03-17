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

class NotRegistered : public Shape
{
public:
  ~NotRegistered(){};
  int calculate() const override { return 0; }
};

Polygon* createPolygon() { return new Polygon; }
Line* createLine() { return new Line; }
Circle* createCircle() { return new Circle; }
NotRegistered* createNotRegistered() { return new NotRegistered; }

Polygon* clonePolygon(const Polygon* other) { return new Polygon(*other); }
Line* cloneLine(const Line* other) { return new Line(*other); }
Circle* cloneCircle(const Circle* other) { return new Circle(*other); }
NotRegistered* cloneNotRegistered(const NotRegistered* other) { return new NotRegistered(*other); }

TEST(CloneFactory, TestCloneFactory)
{
  typedef Loki::Factory<Shape, std::string> FactoryType;
  typedef Loki::CloneFactory<Shape> CloneFactoryType;
  typedef Loki::SingletonHolder<FactoryType, Loki::CreateUsingNew> SingleFactory;
  typedef Loki::SingletonHolder<CloneFactoryType, Loki::CreateUsingNew> SingleCloneFactory;
  auto& factory = SingleFactory::Instance();
  auto& cloneFactory = SingleCloneFactory::Instance();
  factory.Register("polygon", (Shape * (*)()) createPolygon);
  cloneFactory.Register(Loki::TypeInfo(typeid(Polygon)), (Shape * (*)(const Shape*)) clonePolygon);
  factory.Register("line", (Shape * (*)()) createLine);
  cloneFactory.Register(Loki::TypeInfo(typeid(Line)), (Shape * (*)(const Shape*)) cloneLine);
  factory.Register("circle", (Shape * (*)()) createCircle);
  cloneFactory.Register(Loki::TypeInfo(typeid(Circle)), (Shape * (*)(const Shape*)) cloneCircle);

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
  std::vector<std::unique_ptr<Shape>> clonedShapes;
  for (const auto& sPtr : shapes) {
    clonedShapes.emplace_back(cloneFactory.CreateObject(sPtr.get()));
  }
  EXPECT_EQ(shapes[0]->calculate(), 203);
  EXPECT_EQ(clonedShapes[0]->calculate(), 203);
  EXPECT_EQ(shapes[1]->calculate(), 403);
  EXPECT_EQ(clonedShapes[1]->calculate(), 403);
  EXPECT_EQ(shapes[2]->calculate(), 2);
  EXPECT_EQ(clonedShapes[2]->calculate(), 2);
  EXPECT_EQ(shapes[3]->calculate(), 380);
  EXPECT_EQ(clonedShapes[3]->calculate(), 380);
  for (const auto& sPtr : clonedShapes) {
    sPtr->setPos({0, 0});
  }
  EXPECT_EQ(shapes[0]->calculate(), 203);
  EXPECT_EQ(clonedShapes[0]->calculate(), 3);
  EXPECT_EQ(shapes[1]->calculate(), 403);
  EXPECT_EQ(clonedShapes[1]->calculate(), 3);
  EXPECT_EQ(shapes[2]->calculate(), 2);
  EXPECT_EQ(clonedShapes[2]->calculate(), 2);
  EXPECT_EQ(shapes[3]->calculate(), 380);
  EXPECT_EQ(clonedShapes[3]->calculate(), 360);
  EXPECT_ANY_THROW(factory.CreateObject("cube"));
  auto* nr = new NotRegistered;
  auto* c = new Circle;
  EXPECT_ANY_THROW(cloneFactory.CreateObject(nr));
  EXPECT_NO_THROW(cloneFactory.CreateObject(c));
  delete c;
  delete nr;
}
