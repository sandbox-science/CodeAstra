#include <QtTest>
#include "Syntax.h"
#include "SyntaxManager.h"

#include <QTextDocument>
#include <QColor>
#include <QFile>
#include <QTemporaryDir>
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
  void testCreateSyntaxHighlighter_skipsInvalidYaml();
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
  QCOMPARE_EQ(rule1.m_pattern.pattern(), "\\bint\\b");
  QCOMPARE_EQ(rule1.m_format.foreground().color(), QColor("#ff0000"));
  QCOMPARE_EQ(rule1.m_format.fontWeight(), QFont::Bold);
  QCOMPARE_NE(rule1.m_format.fontItalic(), true);

  // Check the second rule
  const auto &rule2 = syntax->m_syntaxRules[1];
  QCOMPARE_EQ(rule2.m_pattern.pattern(), "\\bfloat\\b");
  QCOMPARE_EQ(rule2.m_format.foreground().color(), QColor("#00ff00"));
  QCOMPARE_EQ(rule2.m_format.fontWeight(), QFont::Normal);
  QCOMPARE_EQ(rule2.m_format.fontItalic(), true);
}

void TestSyntax::testLoadMissingKeywords()
{
  YAML::Node config;
  config["other_section"] = YAML::Node();
  syntax->loadSyntaxRules(config);

  // Verify that no rules were loaded
  QVERIFY(syntax->m_syntaxRules.isEmpty());
}

void TestSyntax::testCreateSyntaxHighlighter_skipsInvalidYaml()
{
  QTemporaryDir tempDir;
  QVERIFY2(tempDir.isValid(), "Temporary directory should be valid.");

  QFile invalidConfig(tempDir.filePath("broken.syntax.yaml"));
  QVERIFY2(invalidConfig.open(QIODevice::WriteOnly | QIODevice::Text),
           "Malformed syntax config should be writable.");
  invalidConfig.write("keywords: [broken");
  invalidConfig.close();

  qputenv("CONFIG_DIR", tempDir.path().toUtf8());
  auto highlighter = SyntaxManager::createSyntaxHighlighter("cpp", document);
  qunsetenv("CONFIG_DIR");

  QVERIFY2(highlighter == nullptr, "Malformed YAML should be skipped without crashing.");
}

QTEST_MAIN(TestSyntax)
#include "test_syntax.moc"
