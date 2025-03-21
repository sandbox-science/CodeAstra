#include "Syntax.h"
#include <QColor>
#include <QFont>

Syntax::Syntax(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
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
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);

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
        addPattern(pattern, keywordFormat);
    }

    iterationFormat.setForeground(Qt::darkMagenta);
    iterationFormat.setFontWeight(QFont::Bold);
    QStringList iterationPatterns = {
        "\\bfor\\b", "\\bwhile\\b", "\\bdo\\b", "\\bif\\b", "\\belse\\b"};
    for (const QString &pattern : iterationPatterns)
    {
        addPattern(pattern, iterationFormat);
    }
}

void Syntax::initCommentRules()
{
    // Single line comment format expression
    singleLineCommentFormat.setForeground(Qt::darkGray);
    addPattern("//[^\n]*", singleLineCommentFormat);

    // TO-DO: Add multi-line comment support
}

void Syntax::initQuotationRules()
{
    // Double quotation mark for string
    quotationMark.setForeground(Qt::darkGreen);
    addPattern("\"(\\\\.|[^\"\\\\])*\"", quotationMark);

    //  TO-DO: Add single quotation mark for character
}

void Syntax::initFunctionRules()
{
    // Function format expression
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::darkYellow);
    addPattern("\\b(?!for\\b|if\\b|else\\b|while\\b|do\\b)[a-zA-Z_][a-zA-Z0-9_]*(?=\\s*\\()", functionFormat);
}

void Syntax::initParenthesisRules()
{
    // Color pattern for parenthesis
    QColor parenthesisColor("#6495ED");
    parenthesisFormat.setForeground(parenthesisColor);
    addPattern("[()]", parenthesisFormat);
}

// Regex for single character format 'a', '\n', etc.
void Syntax::initCharRules()
{
    charFormat.setForeground(Qt::darkCyan);
    addPattern("'(\\\\.|[^'])'", charFormat);
}

void Syntax::addPattern(const QString &pattern, const QTextCharFormat &format)
{
    SyntaxRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format  = format;
    syntaxRules.append(rule);
}

void Syntax::highlightBlock(const QString &text)
{
    for (const SyntaxRule &rule : syntaxRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
