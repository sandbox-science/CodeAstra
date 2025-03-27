#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

/**
 * @class Syntax
 * @brief A custom syntax highlighter class that extends QSyntaxHighlighter to provide
 *        syntax highlighting functionality based on user-defined rules.
 *
 * This class allows you to define syntax highlighting rules using regular expressions
 * and associated text formats. It applies these rules to text blocks to highlight
 * specific patterns.
 *
 * @note This class inherits the constructor from QSyntaxHighlighter.
 */
class Syntax : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    using QSyntaxHighlighter::QSyntaxHighlighter;

    virtual ~Syntax() = default;

protected:
    /**
     * @brief Highlights the given text block based on the defined syntax rules.
     *
     * @param text The text block to be highlighted.
     */
    void highlightBlock(const QString &text) override;

public:
    /**
     * @struct SyntaxRule
     * @brief Represents a single syntax highlighting rule.
     *
     * A syntax rule consists of a regular expression pattern and a text format
     * to apply to matching text.
     */
    struct SyntaxRule
    {
        QRegularExpression m_pattern;
        QTextCharFormat m_format;
    };

    QVector<SyntaxRule> m_syntaxRules;

    /**
     * @brief Adds a new syntax highlighting rule.
     *
     * This method allows you to define a new rule by specifying a regular expression
     * pattern and the corresponding text format.
     *
     * @param pattern The regular expression pattern for the rule.
     * @param format The text format to apply to matches of the pattern.
     */
    void addPattern(const QString &pattern, const QTextCharFormat &format);
};