#include <gmock/gmock.h>

#include <loki/MultiMethods.h>

class Shape
{
public:
  virtual ~Shape() {}
};

class Rectangle : public Shape
{};

class Ellipse : public Shape
{};

class Line : public Shape
{};

class OutputDevice
{
public:
  virtual ~OutputDevice() {}
};

class Printer : public OutputDevice
{};

class Screen : public OutputDevice
{};

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
