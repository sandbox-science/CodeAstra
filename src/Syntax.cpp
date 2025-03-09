#include "Syntax.h"

Syntax::Syntax(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  SyntaxRule rule;

  keywordFormat.setForeground(Qt::darkBlue);
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
                  << "\\bvoid\\b" << "\\bvolatile\\b";
  foreach (const QString &pattern, keywordPatterns)
  {
    rule.pattern = QRegularExpression(pattern);
    rule.format  = keywordFormat;
    syntaxRules.append(rule);
  }

  // Single line comment format expression
  singleLineCommentFormat.setForeground(Qt::red);
  rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
  rule.format  = singleLineCommentFormat;
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
