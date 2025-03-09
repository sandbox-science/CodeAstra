#ifndef SYNTAX_H
#define SYNTAX_H

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
    QList<SyntaxRule> syntaxRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationMark;
    QTextCharFormat functionFormat;
    QTextCharFormat parenthesisFormat;
    QTextCharFormat charFormat;

    void addPattern(const QString &pattern, const QTextCharFormat &format);
};

#endif // SYNTAX_H