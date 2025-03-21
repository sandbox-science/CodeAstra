#pragma once

#include <QTextCharFormat>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextDocument>

class Syntax : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Syntax(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct SyntaxRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<SyntaxRule> syntaxRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationMark;
    QTextCharFormat functionFormat;
    QTextCharFormat parenthesisFormat;
    QTextCharFormat charFormat;
    QTextCharFormat iterationFormat;

    void addPattern(const QString &pattern, const QTextCharFormat &format);

    // Initialization functions for different syntax highlighting rules
    void initKeywordRules();
    void initCommentRules();
    void initFunctionRules();
    void initParenthesisRules();
    void initCharRules();
    void initQuotationRules();
};
