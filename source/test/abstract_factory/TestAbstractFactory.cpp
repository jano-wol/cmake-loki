#include <gmock/gmock.h>

#include <loki/AbstractFactory.h>
#include <loki/Singleton.h>
#include <sstream>
#include <vector>

class Shape
{
public:
  virtual int vol() const = 0;
  virtual ~Shape(){};
};

class Shape2D
{
public:
  virtual int vol() const = 0;
  virtual ~Shape2D(){};

protected:
  int x;
  int y;
};

class Shape3D
{
public:
  virtual int vol() const = 0;
  virtual ~Shape3D(){};

protected:
  int x;
  int y;
  int z;
};

class Line : public Shape2D
{
public:
  ~Line(){};
  int vol() const override { return 0; }
};

class Circle : public Shape2D
{
public:
  ~Circle(){};
  int vol() const override { return 3 * x * x; }
};

class Square : Shape2D
{
public:
  ~Square(){};
  int vol() const override { return x * y; }
};

class Plane : Shape3D
{
public:
  ~Plane(){};
  int vol() const override { return 0; }
};

class Ball : Shape3D
{
public:
  ~Ball(){};
  int vol() const override { return x * x * x; }
};

class Cube : Shape3D
{
public:
  ~Cube(){};
  int vol() const override { return x * y * z; }
};

TEST(AbstractFactory, TestAbstractFactory) {}