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
  Ellipse& e = dynamic_cast<Ellipse&>(lhs);
  Printer& p = dynamic_cast<Printer&>(rhs);
  std::stringstream ss;
  ss << e.getName() << "-" << p.getName();
  return ss.str();
}

// FnDispatcher is more convinient than BasicDispatcher
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

typedef Loki::FnDispatcher<Shape, OutputDevice, std::string> FnDispatcher;
typedef Loki::FnDispatcher<Shape, Shape, std::string> FnDispatcherSymmetric;

std::string FireRectanglePrinterSimple(Rectangle& r, Printer& p)
{
  std::stringstream ss;
  ss << r.getName() << "-" << p.getName();
  return ss.str();
}

std::string FireEllipsePrinterSimple(Ellipse& e, Printer& p)
{
  std::stringstream ss;
  ss << e.getName() << "-" << p.getName();
  return ss.str();
}

std::string FireEllipseRectangleSimple(Ellipse& e, Rectangle& r)
{
  std::stringstream ss;
  ss << e.getName() << "-" << r.getName();
  return ss.str();
}

TEST(MultiMethod, TestFnDispatcher)
{
  FnDispatcher disp;
  disp.Add<Rectangle, Printer, FireRectanglePrinterSimple>();
  Rectangle r;
  Printer p;
  Shape* rSh = &r;
  OutputDevice* pDev = &p;
  std::string result = disp.Go(*rSh, *pDev);
  EXPECT_EQ(result, "Rectangle-Printer");
  Ellipse e;
  Shape* eSh = &e;
  EXPECT_ANY_THROW(disp.Go(e, *pDev));
  FnDispatcherSymmetric dispSymmetric;
  dispSymmetric.Add<Ellipse, Rectangle, FireEllipseRectangleSimple, true>();
  result = dispSymmetric.Go(*eSh, *rSh);
  EXPECT_EQ(result, "Ellipse-Rectangle");
  result = dispSymmetric.Go(*rSh, *eSh);
  EXPECT_EQ(result, "Ellipse-Rectangle");
  EXPECT_ANY_THROW(dispSymmetric.Go(*eSh, *eSh));
}

typedef Loki::FunctorDispatcher<Shape, OutputDevice, std::string, Loki::StaticCaster> FunctorDispatcher;

struct SomeFunctor
{
  std::string operator()(Ellipse& e, Printer& p)
  {
    std::stringstream ss;
    ss << e.getName() << "-" << p.getName() << "-functor";
    return ss.str();
  }

  std::string operator()(Rectangle& r, Printer& p)
  {
    std::stringstream ss;
    ss << r.getName() << "-" << p.getName() << "-functor";
    return ss.str();
  }
};

struct SomeClass
{
  std::string FireRectanglePrinter(Rectangle& r, Printer& p)
  {
    std::stringstream ss;
    ss << r.getName() << "-" << p.getName() << "-class";
    return ss.str();
  }

  std::string FireEllipsePrinter(Ellipse& e, Printer& p)
  {
    std::stringstream ss;
    ss << e.getName() << "-" << p.getName() << "-class";
    return ss.str();
  }
};

TEST(MultiMethod, TestFunctorDispatcher)
{
  Rectangle r;
  Printer p;
  Screen s;
  Shape* rSh = &r;
  OutputDevice* pDev = &p;
  OutputDevice* sDev = &s;
  Ellipse e;
  Shape* eSh = &e;
  SomeClass myObject;
  Loki::Functor<std::string, TYPELIST_2(Rectangle&, Printer&)> member1(&myObject, &SomeClass::FireRectanglePrinter);
  Loki::Functor<std::string, TYPELIST_2(Ellipse&, Printer&)> member2(&myObject, &SomeClass::FireEllipsePrinter);
  Loki::Functor<std::string, TYPELIST_2(Rectangle&, Printer&)> function1(FireRectanglePrinterSimple);
  Loki::Functor<std::string, TYPELIST_2(Ellipse&, Printer&)> function2(FireEllipsePrinterSimple);

  FunctorDispatcher disp1;
  disp1.Add<Rectangle, Printer>(SomeFunctor());
  disp1.Add<Ellipse, Printer>(SomeFunctor());
  EXPECT_EQ(disp1.Go(*rSh, *pDev), "Rectangle-Printer-functor");
  EXPECT_EQ(disp1.Go(*eSh, *pDev), "Ellipse-Printer-functor");
  EXPECT_EQ(disp1.Go(e, *pDev), "Ellipse-Printer-functor");
  EXPECT_ANY_THROW(disp1.Go(e, *sDev));
  disp1.Add<Rectangle, Printer>(member1);
  disp1.Add<Ellipse, Printer>(member2);
  EXPECT_EQ(disp1.Go(*rSh, *pDev), "Rectangle-Printer-class");
  EXPECT_EQ(disp1.Go(*eSh, *pDev), "Ellipse-Printer-class");
  EXPECT_EQ(disp1.Go(e, *pDev), "Ellipse-Printer-class");
  EXPECT_ANY_THROW(disp1.Go(e, *sDev));
  disp1.Add<Rectangle, Printer>(function1);
  disp1.Add<Ellipse, Printer>(function2);
  EXPECT_EQ(disp1.Go(*rSh, *pDev), "Rectangle-Printer");
  EXPECT_EQ(disp1.Go(*eSh, *pDev), "Ellipse-Printer");
  EXPECT_EQ(disp1.Go(e, *pDev), "Ellipse-Printer");
  EXPECT_ANY_THROW(disp1.Go(e, *sDev));
}