#include "Syntax.h"

#include <QDebug>

namespace
{
constexpr int kMaxHighlightedBlockLength = 8192;
constexpr int kMaxSyntaxRules = 256;
constexpr int kMaxRegexPatternLength = 256;
}

Syntax::Syntax(QTextDocument *parent, const YAML::Node &config)
    : QSyntaxHighlighter(parent)
{
    qDebug() << "Syntax highlighter created";
    loadSyntaxRules(config);
}

void Syntax::highlightBlock(const QString &text)
{
    if (text.size() > kMaxHighlightedBlockLength)
    {
        return;
    }

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
            if (m_syntaxRules.size() >= kMaxSyntaxRules)
            {
                qWarning() << "Syntax rule limit reached. Skipping remaining rules.";
                return;
            }

            QString regex;
            try
            {
                std::string regexStr = rule["regex"].as<std::string>(); //will throw exception if the key does not exist 
                regex                = QString::fromStdString(regexStr);
            }
            catch(const YAML::Exception e)
            {
                qWarning() << " YAML exception when parsion the regex in syntax file" << e.what();
                continue;
            }

            if (regex.size() > kMaxRegexPatternLength)
            {
                qWarning() << "Regex too long in syntax file. Skipping...";
                continue;
            }

            if (regex.contains(QRegularExpression(R"(\\[1-9])")))
            {
                qWarning() << "Backreferences are not allowed in syntax regex. Skipping...";
                continue;
            }

            if (regex.contains(QRegularExpression(R"(\((?:[^()\\]|\\.)*[+*](?:[^()\\]|\\.)*\)[+*{])")))
            {
                qWarning() << "Potentially unsafe nested quantifier regex. Skipping...";
                continue;
            }

            QColor color;
            try
            {
                std::string colorStr = rule["color"].as<std::string>();
                color                = QColor(QString::fromStdString(colorStr));
            }
            catch(const YAML::Exception e)
            {
                qWarning() << " YAML exception when parsion the color in syntax file" << e.what();
                continue;
            }

            //checks if the color is a valid color
            if(!color.isValid())
            {
                qWarning() << "Invalid Color : Skipping...";
                continue;
            }

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
            QRegularExpression compiledRegex(regex);
            if (!compiledRegex.isValid())
            {
                qWarning() << "Invalid regex in syntax file:" << compiledRegex.errorString();
                continue;
            }

            m_syntaxRules.append({compiledRegex, format});
        }
    }
}
