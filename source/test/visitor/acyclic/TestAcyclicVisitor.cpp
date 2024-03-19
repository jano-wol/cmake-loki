#include <gmock/gmock.h>

#include <loki/Visitor.h>

class DocElement : public Loki::BaseVisitable<>
{
public:
  DEFINE_VISITABLE()
};

class Paragraph : public DocElement
{
public:
  DEFINE_VISITABLE()
};

class MyConcreteVisitor : public Loki::BaseVisitor, public Loki::Visitor<DocElement>, public Loki::Visitor<Paragraph>
{
public:
  MyConcreteVisitor() : docElementCount(0), paragraphCount(0){};

  void Visit(DocElement&) { ++docElementCount; }
  void Visit(Paragraph&)
  {
    ++docElementCount;
    ++paragraphCount;
  }
  int docElementCount;
  int paragraphCount;
};

TEST(Visitor, TestAcyclic)
{
  MyConcreteVisitor visitor;
  Paragraph par;
  DocElement* d1 = &par;
  DocElement d2;
  d1->Accept(visitor);
  d2.Accept(visitor);
  EXPECT_EQ(visitor.docElementCount, 2);
  EXPECT_EQ(visitor.paragraphCount, 1);
}
