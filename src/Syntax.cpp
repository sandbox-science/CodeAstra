#include "Syntax.h"

Syntax::Syntax(QTextDocument *parent, const YAML::Node &config)
    : QSyntaxHighlighter(parent)
{
    qDebug() << "Syntax highlighter created";
    loadSyntaxRules(config);
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

void Syntax::addPattern(const QString &pattern, const QTextCharFormat &format)
{
    SyntaxRule rule;
    rule.m_pattern = QRegularExpression(pattern);
    rule.m_format  = format;
    m_syntaxRules.append(rule);
}

void Syntax::loadSyntaxRules(const YAML::Node &config)
{
    m_syntaxRules.clear();

    if (!config["keywords"])
    {
        return;
    }

    auto keywords = config["keywords"];
    for (const auto &category : keywords)
    {
        const std::string key = category.first.as<std::string>();
        const YAML::Node &rules = category.second;

        // Iterate through each rule in the category
        for (const auto &rule : rules)
        {
            QString regex = QString::fromStdString(rule["regex"].as<std::string>());
            QColor color(QString::fromStdString(rule["color"].as<std::string>()));
            qDebug() << "regex: " << regex;

            // Create a QTextCharFormat for the rule
            QTextCharFormat format;
            format.setForeground(color);
            if (rule["bold"] && rule["bold"].as<bool>())
            {
                format.setFontWeight(QFont::Bold);
            }
            if (rule["italic"] && rule["italic"].as<bool>())
            {
                format.setFontItalic(true);
            }

            // Append the rule to the list of syntax rules
            m_syntaxRules.append({QRegularExpression(regex), format});
        }
    }
}
