#include <gmock/gmock.h>

#include <loki/Visitor.h>

/* template <typename R, typename Visited>
struct DefaultCatchAll
{
  static R OnUnknownVisitor(Visited&, Loki::BaseVisitor&) { throw(std::runtime_error("Not registered type!")); }
};
 */

class DocElement;
class Paragraph;

typedef Loki::CyclicVisitor<void, TYPELIST_2(DocElement, Paragraph)> MyVisitorBase;

class DocElement
{
public:
  virtual void Visit(MyVisitorBase&) = 0;
  DEFINE_CYCLIC_VISITABLE(MyVisitorBase)
};

class Paragraph : public DocElement
{
public:
  DEFINE_CYCLIC_VISITABLE(MyVisitorBase)
};

class Section : public DocElement
{
public:
  DEFINE_CYCLIC_VISITABLE(MyVisitorBase)
};

class MyStatVisitor : public MyVisitorBase
{
public:
  MyStatVisitor() : docElementCount(0), paragraphCount(0){};

  void Visit(DocElement&) { ++docElementCount; }
  void Visit(Paragraph&)
  {
    ++docElementCount;
    ++paragraphCount;
  }
  int docElementCount;
  int paragraphCount;
};

TEST(Visitor, TestCyclic)
{
  MyStatVisitor statVisitor;
  Paragraph par;
  DocElement* d1 = &par;
  DocElement d2;
  d1->Accept(statVisitor);
  d2.Accept(statVisitor);
  EXPECT_EQ(statVisitor.docElementCount, 2);
  EXPECT_EQ(statVisitor.paragraphCount, 1);
  Section s;
  EXPECT_ANY_THROW(s.Accept(statVisitor));
}

TEST(Visitor, TestCyclic) {}
