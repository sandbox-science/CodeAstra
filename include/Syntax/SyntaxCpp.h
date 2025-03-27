#pragma once

#include <QTextCharFormat>
#include <QRegularExpression>
#include <Syntax/Syntax.h>
#include <QTextDocument>

/**
 * @class SyntaxCpp
 * @brief A syntax highlighter class for C/C++ code, inheriting from the Syntax base class.
 *
 * This class provides syntax highlighting for C and C++ code by defining various 
 * QTextCharFormat rules for different C++ language elements.
 */
class SyntaxCpp : public Syntax
{
public:
    SyntaxCpp(QTextDocument *parent = nullptr);

private:
    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_singleLineCommentFormat;
    QTextCharFormat m_quotationMark;
    QTextCharFormat m_functionFormat;
    QTextCharFormat m_parenthesisFormat;
    QTextCharFormat m_charFormat;
    QTextCharFormat m_iterationFormat;

    // Initialization functions for different syntax highlighting rules
    void initKeywordRules();
    void initCommentRules();
    void initFunctionRules();
    void initParenthesisRules();
    void initCharRules();
    void initQuotationRules();
};
