#pragma once

#include <QTextCharFormat>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextDocument>

/**
 * @class Syntax
 * @brief A class for syntax highlighting in a QTextDocument.
 *
 * This class inherits from QSyntaxHighlighter and provides functionality
 * to highlight different syntax elements such as keywords, comments,
 * functions, parentheses, characters, and quotations in a QTextDocument.
 *
 * The Syntax class uses regular expressions to define patterns for different
 * syntax elements and applies corresponding text formats to them.
 */
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
        QRegularExpression m_pattern;
        QTextCharFormat m_format;
    };
    QVector<SyntaxRule> m_syntaxRules;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_singleLineCommentFormat;
    QTextCharFormat m_quotationMark;
    QTextCharFormat m_functionFormat;
    QTextCharFormat m_parenthesisFormat;
    QTextCharFormat m_charFormat;
    QTextCharFormat m_iterationFormat;

    void addPattern(const QString &pattern, const QTextCharFormat &format);

    // Initialization functions for different syntax highlighting rules
    void initKeywordRules();
    void initCommentRules();
    void initFunctionRules();
    void initParenthesisRules();
    void initCharRules();
    void initQuotationRules();
};
