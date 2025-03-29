#pragma once

#include "syntax/Syntax.h"

#include <QString>
#include <QTextDocument>
#include <memory>

#include <yaml-cpp/yaml.h>

/**
 * @class SyntaxManager
 * @brief Manages the creation of syntax highlighters for different file types.
 *
 * The SyntaxManager class provides functionality to create syntax highlighters
 * based on file extensions. It supports dynamic creation of highlighters by
 * utilizing configuration data and applies them to QTextDocument objects.
 *
 * @note This class is designed to work with the Qt framework and YAML configuration.
 */
class SyntaxManager
{
public:
    /**
     * @brief Creates a syntax highlighter based on the file extension.
     * @param extension The file extension (e.g., "cpp", "py").
     * @param doc The QTextDocument to which the highlighter will be applied.
     * @return A unique pointer to the appropriate syntax highlighter, or nullptr if not available.
     */
    static std::unique_ptr<QSyntaxHighlighter> createSyntaxHighlighter(const QString &extension, QTextDocument *doc);

private:
    static std::unique_ptr<QSyntaxHighlighter> createHighlighter(QTextDocument *doc, const std::vector<YAML::Node> &config, const QString &extension);
};
