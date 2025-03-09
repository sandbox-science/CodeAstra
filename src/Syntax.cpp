#include "Syntax.h"

Syntax::Syntax(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  keywordFormat.setForeground(Qt::blue);
  keywordFormat.setFontWeight(QFont::Bold);
  QStringList keywordPatterns;
  keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                  << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                  << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                  << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                  << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                  << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                  << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                  << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                  << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                  << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bforeach\\b";
  foreach (const QString &pattern, keywordPatterns)
  {
    addPattern(pattern, keywordFormat);
  }

  // Single line comment format expression
  singleLineCommentFormat.setForeground(Qt::darkGray);
  addPattern("//[^\n]*", singleLineCommentFormat);

  // Double quotation mark for string
  quotationMark.setForeground(Qt::darkGreen);
  addPattern("\".*\"", quotationMark);

  // Function format expression
  functionFormat.setFontItalic(true);
  functionFormat.setForeground(Qt::darkYellow);
  addPattern("\\b[a-zA-Z_][a-zA-Z0-9_]*(?=\\s*\\()", functionFormat);

  // Color pattern for parenthesis
  QColor parenthesisColor("#6495ED");
  parenthesisFormat.setForeground(parenthesisColor);
  addPattern("[()]", parenthesisFormat);

  // Regex for single character format 'a', '\n', etc
  charFormat.setForeground(Qt::darkCyan);
  addPattern("'(\\\\.|[^'])'", charFormat);
}

// Add syntax highlighting patterns
void Syntax::addPattern(const QString &pattern, const QTextCharFormat &format)
{
    SyntaxRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format  = format;
    syntaxRules.append(rule);
}

void Syntax::highlightBlock(const QString &text)
{
  foreach (const SyntaxRule &rule, syntaxRules)
  {
    QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
    while (matchIterator.hasNext())
    {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}
