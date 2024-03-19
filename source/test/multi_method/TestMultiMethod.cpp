#include <gmock/gmock.h>

#include <loki/MultiMethods.h>
#include <sstream>

class Shape
{
public:
  virtual ~Shape() {}
  virtual std::string getName() const { return "Shape"; }
};

class Rectangle : public Shape
{
public:
  std::string getName() const override { return "Rectangle"; }
};

class Ellipse : public Shape
{
public:
  std::string getName() const override { return "Ellipse"; }
};

class Line : public Shape
{
public:
  std::string getName() const override { return "Line"; }
};

class OutputDevice
{
public:
  virtual ~OutputDevice() {}
  virtual std::string getName() const { return "OutputDevice"; }
};

class Printer : public OutputDevice
{
public:
  std::string getName() const override { return "Printer"; }
};

class Screen : public OutputDevice
{
public:
  std::string getName() const override { return "Screen"; }
};

struct Painter
{
  std::string Fire(Rectangle&, Printer&) { return "Rectangle-Printer"; }
  std::string Fire(Ellipse&, Printer&) { return "Ellipse-Printer"; }
  std::string Fire(Rectangle&, Screen&) { return "Rectangle-Screen"; }
  std::string Fire(Ellipse&, Screen&) { return "Ellipse-Screen"; }
  std::string OnError(Shape&, OutputDevice&) { throw(std::runtime_error("not implemented")); }
};

typedef Loki::StaticDispatcher<Painter, Shape, TYPELIST_2(Rectangle, Ellipse), false, OutputDevice,
                               TYPELIST_2(Printer, Screen), std::string>
    Dispatcher;

TEST(MultiMethod, TestStaticDispatcher)
{
  Dispatcher disp;
  Ellipse e;
  Printer p;
  Shape* pSh = &e;
  OutputDevice* pDev = &p;
  Painter painter;
  std::string result = disp.Go(*pSh, *pDev, painter);
  EXPECT_EQ(result, "Ellipse-Printer");
  Line l;
  EXPECT_ANY_THROW(disp.Go(l, *pDev, painter));
}

std::string FireRectanglePrinter(Shape& lhs, OutputDevice& rhs)
{
  Rectangle& r = dynamic_cast<Rectangle&>(lhs);
  Printer& p = dynamic_cast<Printer&>(rhs);
  std::stringstream ss;
  ss << r.getName() << "-" << p.getName();
  return ss.str();
}

std::string FireEllipsePrinter(Shape& lhs, OutputDevice& rhs)
{
  Ellipse& r = dynamic_cast<Ellipse&>(lhs);
  Printer& p = dynamic_cast<Printer&>(rhs);
  std::stringstream ss;
  ss << r.getName() << "-" << p.getName();
  return ss.str();
}

TEST(MultiMethod, TestBasicDispatcher)
{
  Loki::BasicDispatcher<Shape, OutputDevice, std::string> disp;
  disp.Add<Rectangle, Printer>(FireRectanglePrinter);
  Rectangle r;
  Printer p;
  Shape* rSh = &r;
  OutputDevice* pDev = &p;
  std::string result = disp.Go(*rSh, *pDev);
  EXPECT_EQ(result, "Rectangle-Printer");
  Ellipse e;
  EXPECT_ANY_THROW(disp.Go(e, *pDev));
}

typedef Loki::FnDispatcher<Shape> FnDispatcher;

