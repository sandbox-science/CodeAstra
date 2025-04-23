#include <QtTest>
#include "Syntax.h"

#include <QTextDocument>
#include <QColor>
#include <yaml-cpp/yaml.h>

// Helper function to create a YAML node for testing
YAML::Node createTestConfig()
{
  YAML::Node config;
  YAML::Node keywords = config["keywords"];

  YAML::Node category1;
  YAML::Node rule1;
  rule1["regex"]  = "\\bint\\b";
  rule1["color"]  = "#ff0000";
  rule1["bold"]   = true;
  rule1["italic"] = false;
  category1.push_back(rule1);

  YAML::Node rule2;
  rule2["regex"]  = "\\bfloat\\b";
  rule2["color"]  = "#00ff00";
  rule2["bold"]   = false;
  rule2["italic"] = true;
  category1.push_back(rule2);

  keywords["types"] = category1;

  return config;
}

class TestSyntax : public QObject
{
  Q_OBJECT

protected:
  QTextDocument *document;
  Syntax *syntax;

private slots:
  void initTestCase();
  void cleanupTestCase();
  void testLoadValidSyntaxRules();
  void testLoadEmptySyntaxRules();
  void testLoadMissingKeywords();
};

void TestSyntax::initTestCase()
{
  qDebug() << "Initializing TestSyntax tests...";
  document = new QTextDocument();
  syntax   = new Syntax(document, YAML::Node());
}

void TestSyntax::cleanupTestCase()
{
  qDebug() << "Cleaning up TestSyntax tests...";
}

void TestSyntax::testLoadEmptySyntaxRules()
{
  YAML::Node config;
  syntax->loadSyntaxRules(config);

  // Verify that no rules were loaded
  QVERIFY(syntax->m_syntaxRules.isEmpty());
}

void TestSyntax::testLoadValidSyntaxRules()
{
  YAML::Node config = createTestConfig();
  syntax->loadSyntaxRules(config);

  // Verify that the rules were loaded correctly
  QVERIFY(syntax->m_syntaxRules.size() == 2);

  // Check the first rule
  const auto &rule1 = syntax->m_syntaxRules[0];
  QCOMPARE_H(rule1.m_pattern.pattern(), "\\bint\\b");
  QCOMPARE_H(rule1.m_format.foreground().color(), QColor("#ff0000"));
  QCOMPARE_H(rule1.m_format.fontWeight(), QFont::Bold);
  QCOMPARE_H(rule1.m_format.fontItalic(), true);

  // Check the second rule
  const auto &rule2 = syntax->m_syntaxRules[1];
  QCOMPARE_H(rule2.m_pattern.pattern(), "\\bfloat\\b");
  QCOMPARE_H(rule2.m_format.foreground().color(), QColor("#00ff00"));
  QCOMPARE_H(rule2.m_format.fontWeight(), QFont::Normal);
  QCOMPARE_H(rule2.m_format.fontItalic(), true);
}

void TestSyntax::testLoadMissingKeywords()
{
  YAML::Node config;
  config["other_section"] = YAML::Node();
  syntax->loadSyntaxRules(config);

  // Verify that no rules were loaded
  QVERIFY(syntax->m_syntaxRules.isEmpty());
}

QTEST_MAIN(TestSyntax)
#include "test_syntax.moc"