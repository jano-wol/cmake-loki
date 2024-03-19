#include <gmock/gmock.h>

#include <loki/Visitor.h>

template <typename R, typename Visited>
struct DefaultCatchAll
{
  static R OnUnknownVisitor(Visited&, Loki::BaseVisitor&) { throw(std::runtime_error("Not registered type!")); }
};

class DocElement : public Loki::BaseVisitable<void, DefaultCatchAll>
{
public:
  DEFINE_VISITABLE()
};

class Paragraph : public DocElement
{
public:
  DEFINE_VISITABLE()
};

class Section : public DocElement
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

class MyStatVisitor : public Loki::BaseVisitor, public Loki::Visitor<DocElement>, public Loki::Visitor<Paragraph>
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

TEST(Visitor, TestAcyclic)
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
