#include "Syntax.h"
#include <QColor>
#include <QFont>

Syntax::Syntax(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    initKeywordRules();
    initFunctionRules();
    initParenthesisRules();
    initCharRules();

    // Keep these two calls are at the end
    // to correctly apply the rules for strings and comments
    initCommentRules();
    initQuotationRules();
}

void Syntax::initKeywordRules()
{
    // Keyword format
    m_keywordFormat.setForeground(Qt::blue);
    m_keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns = {
        "\\bchar\\b", "\\bclass\\b", "\\bconst\\b",
        "\\bdouble\\b", "\\benum\\b", "\\bexplicit\\b",
        "\\bfriend\\b", "\\binline\\b", "\\bint\\b",
        "\\blong\\b", "\\bnamespace\\b", "\\boperator\\b",
        "\\bprivate\\b", "\\bprotected\\b", "\\bpublic\\b",
        "\\bshort\\b", "\\bsignals\\b", "\\bsigned\\b",
        "\\bslots\\b", "\\bstatic\\b", "\\bstruct\\b",
        "\\btemplate\\b", "\\btypedef\\b", "\\btypename\\b",
        "\\bunion\\b", "\\bunsigned\\b", "\\bvirtual\\b",
        "\\bvoid\\b", "\\bvolatile\\b", "\\bforeach\\b"};

    for (const QString &pattern : keywordPatterns)
    {
        addPattern(pattern, m_keywordFormat);
    }

    m_iterationFormat.setForeground(Qt::darkMagenta);
    m_iterationFormat.setFontWeight(QFont::Bold);
    QStringList iterationPatterns = {"\\bfor\\b", "\\bwhile\\b", "\\bdo\\b", "\\bif\\b", "\\belse\\b"};
    for (const QString &pattern : iterationPatterns)
    {
        addPattern(pattern, m_iterationFormat);
    }
}

void Syntax::initCommentRules()
{
    // Single line comment format expression
    m_singleLineCommentFormat.setForeground(Qt::darkGray);
    addPattern("//[^\n]*", m_singleLineCommentFormat);

    // TO-DO: Add multi-line comment support
}

void Syntax::initQuotationRules()
{
    // Double quotation mark for string
    m_quotationMark.setForeground(Qt::darkGreen);
    addPattern("\"(\\\\.|[^\"\\\\])*\"", m_quotationMark);

    //  TO-DO: Add single quotation mark for character
}

void Syntax::initFunctionRules()
{
    // Function format expression
    m_functionFormat.setFontItalic(true);
    m_functionFormat.setForeground(Qt::darkYellow);
    addPattern("\\b(?!for\\b|if\\b|else\\b|while\\b|do\\b)[a-zA-Z_][a-zA-Z0-9_]*(?=\\s*\\()", m_functionFormat);
}

void Syntax::initParenthesisRules()
{
    // Color pattern for parenthesis
    QColor parenthesisColor("#6495ED");
    m_parenthesisFormat.setForeground(parenthesisColor);
    addPattern("[()]", m_parenthesisFormat);
}

// Regex for single character format 'a', '\n', etc.
void Syntax::initCharRules()
{
    m_charFormat.setForeground(Qt::darkCyan);
    addPattern("'(\\\\.|[^'])'", m_charFormat);
}

void Syntax::addPattern(const QString &pattern, const QTextCharFormat &format)
{
    SyntaxRule rule;
    rule.m_pattern = QRegularExpression(pattern);
    rule.m_format  = format;
    m_syntaxRules.append(rule);
}

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
