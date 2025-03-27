#pragma once

#include <QTextCharFormat>
#include <QRegularExpression>
#include <Syntax/Syntax.h>
#include <QTextDocument>

/**
 * @class SyntaxGo
 * @brief A syntax highlighter class for Go code, inheriting from the Syntax base class.
 *
 * This class provides syntax highlighting for Golang code by defining 
 * various QTextCharFormat rules for different Go language elements.
 */
class SyntaxGo : public Syntax
{
public:
    SyntaxGo(QTextDocument *parent = nullptr);

private:
    QTextCharFormat m_quotationMark;

    // Initialization functions for different syntax highlighting rules
    void initQuotationRules();
};
