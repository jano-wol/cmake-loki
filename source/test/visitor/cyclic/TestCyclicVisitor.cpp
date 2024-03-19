#include <gmock/gmock.h>

#include <loki/Visitor.h>

class DocElement;
class Paragraph;
class Section;
typedef Loki::CyclicVisitor<void, TYPELIST_3(DocElement, Paragraph, Section)> MyVisitorBase;

class MyStatVisitor : public MyVisitorBase
{
public:
  MyStatVisitor() : docElementCount(0), paragraphCount(0), sectionCount(0){};

  void Visit(DocElement&) { ++docElementCount; }
  void Visit(Paragraph&)
  {
    ++docElementCount;
    ++paragraphCount;
  }
  void Visit(Section&)
  {
    ++docElementCount;
    ++sectionCount;
  }
  int docElementCount;
  int paragraphCount;
  int sectionCount;
};

class DocElement
{
public:
  DEFINE_CYCLIC_VISITABLE(MyVisitorBase);
};
class Paragraph : public DocElement
{
public:
  DEFINE_CYCLIC_VISITABLE(MyVisitorBase);
};

class Section : public DocElement
{
public:
  DEFINE_CYCLIC_VISITABLE(MyVisitorBase);
};

TEST(Visitor, TestCyclic)
{
  DocElement d;
  Paragraph p;
  Section s;
  MyStatVisitor v;
  d.Accept(v);
  p.Accept(v);
  EXPECT_EQ(v.docElementCount, 2);
  EXPECT_EQ(v.paragraphCount, 1);
  EXPECT_EQ(v.sectionCount, 0);
  s.Accept(v);
  EXPECT_EQ(v.docElementCount, 3);
  EXPECT_EQ(v.paragraphCount, 1);
  EXPECT_EQ(v.sectionCount, 1);
  DocElement* d1 = &d;
  DocElement* d2 = &p;
  DocElement* d3 = &s;
  d1->Accept(v);
  EXPECT_EQ(v.docElementCount, 4);
  EXPECT_EQ(v.paragraphCount, 1);
  EXPECT_EQ(v.sectionCount, 1);
  d2->Accept(v);
  EXPECT_EQ(v.docElementCount, 5);
  EXPECT_EQ(v.paragraphCount, 2);
  EXPECT_EQ(v.sectionCount, 1);
  d3->Accept(v);
  EXPECT_EQ(v.docElementCount, 6);
  EXPECT_EQ(v.paragraphCount, 2);
  EXPECT_EQ(v.sectionCount, 2);
}
