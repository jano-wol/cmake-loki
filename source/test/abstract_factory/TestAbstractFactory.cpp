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

class Entity
{
public:
  virtual int getHp() const = 0;

protected:
  virtual Entity* Clone() const = 0;
  int baseHp = 10;
};

class Soldier : public Entity
{
public:
  virtual ~Soldier() {}
  virtual int getHp() const override = 0;
  virtual Soldier* Clone() const override = 0;
};
class Monster : public Entity
{
public:
  virtual ~Monster() {}
  virtual int getHp() const override = 0;
  virtual Monster* Clone() const override = 0;
};
class SuperMonster : public Entity
{
public:
  virtual ~SuperMonster() {}
  virtual int getHp() const override = 0;
  virtual SuperMonster* Clone() const override = 0;
};

class SillySoldier : public Soldier
{
  int getHp() const override { return baseHp; };
  SillySoldier* Clone() const override { return new SillySoldier(*this); };
};

class SillyMonster : public Monster
{
  int getHp() const override { return baseHp * 5; };
  SillyMonster* Clone() const override { return new SillyMonster(*this); };
};

class SillySuperMonster : public SuperMonster
{
  int getHp() const override { return baseHp * 10; };
  SillySuperMonster* Clone() const override { return new SillySuperMonster(*this); };
};

class BadSoldier : public Soldier
{
  int getHp() const override { return baseHp * 5; };
  BadSoldier* Clone() const override { return new BadSoldier(*this); };
};

class BadMonster : public Monster
{
  int getHp() const override { return baseHp * 25; };
  BadMonster* Clone() const override { return new BadMonster(*this); };
};

class BadSuperMonster : public SuperMonster
{
  int getHp() const override { return baseHp * 50; };
  BadSuperMonster* Clone() const override { return new BadSuperMonster(*this); };
};

typedef Loki::AbstractFactory<TYPELIST_2(Shape2D, Shape3D)> AbstractShapeFactory;
typedef Loki::ConcreteFactory<AbstractShapeFactory, Loki::OpNewFactoryUnit, TYPELIST_2(Line, Plane)> LowDimension;
typedef Loki::ConcreteFactory<AbstractShapeFactory, Loki::OpNewFactoryUnit, TYPELIST_2(Circle, Box)> Mixed;
typedef Loki::AbstractFactory<TYPELIST_3(Soldier, Monster, SuperMonster)> AbstractShapeFactory2;
typedef Loki::ConcreteFactory<AbstractShapeFactory2, Loki::PrototypeFactoryUnit> Proto;

TEST(AbstractFactory, TestAbstractFactory)
{
  typedef Loki::SingletonHolder<LowDimension, Loki::CreateUsingNew> SingleLowDimensionFactory;
  typedef Loki::SingletonHolder<Mixed, Loki::CreateUsingNew> MixedFactory;
  auto& lowDimensionFactory = SingleLowDimensionFactory::Instance();
  auto& mixedFactory = MixedFactory::Instance();
  std::unique_ptr<Shape> pLine(lowDimensionFactory.Create<Shape2D>());
  std::unique_ptr<Shape> pPlane(lowDimensionFactory.Create<Shape3D>());
  std::unique_ptr<Shape> pCircle(mixedFactory.Create<Shape2D>());
  std::unique_ptr<Shape> pBox(mixedFactory.Create<Shape3D>());
  EXPECT_EQ(pLine->getName(), "Line");
  EXPECT_EQ(pPlane->getName(), "Plane");
  EXPECT_EQ(pCircle->getName(), "Circle");
  EXPECT_EQ(pBox->getName(), "Box");
}

TEST(AbstractFactory, TestPrototypeFactory)
{
  typedef Loki::SingletonHolder<Proto, Loki::CreateUsingNew> ProtoFactory;
  auto& protoFactory = ProtoFactory::Instance();
  protoFactory.SetPrototype<SillySoldier>(new SillySoldier);
  protoFactory.SetPrototype<SillyMonster>(new SillyMonster);
  Entity* pSoldier = protoFactory.Create<Soldier>();
  Entity* pMonster = protoFactory.Create<Monster>();
  EXPECT_EQ(pSoldier->getHp(), 10);
  EXPECT_EQ(pMonster->getHp(), 50);
  protoFactory.SetPrototype<BadSoldier>(new BadSoldier);
  pSoldier = protoFactory.Create<Soldier>();
  EXPECT_EQ(pSoldier->getHp(), 50);
}