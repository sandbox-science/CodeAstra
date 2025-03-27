#include "Syntax/Syntax.h"

void Syntax::highlightBlock(const QString &text)
{
    for (const SyntaxRule &rule : m_syntaxRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.m_pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.m_format);
        }
    }
}

void Syntax::addPattern(const QString &pattern, const QTextCharFormat &format)
{
    SyntaxRule rule;
    rule.m_pattern = QRegularExpression(pattern);
    rule.m_format = format;
    m_syntaxRules.append(rule);
}