#include "Syntax/SyntaxGo.h"

#include <QColor>
#include <QFont>

SyntaxGo::SyntaxGo(QTextDocument *parent) : Syntax(parent)
{
    qDebug() << "SyntaxGo highlighter created";
    initQuotationRules();
}

void SyntaxGo::initQuotationRules()
{
    // Double quotation mark for string
    m_quotationMark.setForeground(Qt::darkGreen);
    addPattern("\"(\\\\.|[^\"\\\\])*\"", m_quotationMark);
    //  TO-DO: Add single quotation mark for character
}