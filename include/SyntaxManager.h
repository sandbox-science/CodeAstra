#pragma once

#include "Syntax/SyntaxCpp.h"
#include "Syntax/SyntaxGo.h"
// #include "Syntax/SyntaxPy.h"  // Uncomment when implemented
// #include "Syntax/SyntaxEx.h" // Uncomment when implemented

#include <QString>
#include <QTextDocument>
#include <memory>

/**
 * @class SyntaxManager
 * @brief A utility class for creating syntax highlighters based on file extensions.
 *
 * This class provides a static method to create appropriate syntax highlighters
 * for different programming languages based on the file extension. If a syntax
 * highlighter for a specific file type is not implemented, it returns a nullptr.
 */
class SyntaxManager
{
public:
    static std::unique_ptr<QSyntaxHighlighter> createSyntaxHighlighter(const QString &extension, QTextDocument *doc)
    {
        qDebug() << "Creating highlighter for extension:" << extension;
        if (extension == "cpp" || extension == "h" || extension == "c")
        {
            qDebug() << "C/C++ syntax highlighter selected.";
            return std::make_unique<SyntaxCpp>(doc);
        }
        else if (extension == "py")
        {
            qDebug() << "Python syntax highlighter not implemented.";
            // return std::make_unique<SyntaxPy>(doc);
        }
        else if (extension == "go")
        {
            qDebug() << "Go syntax highlighter selected.";
            return std::make_unique<SyntaxGo>(doc);
        }
        else if (extension == "ex")
        {
            qDebug() << "Elixir syntax highlighter not implemented.";
            // return std::make_unique<SyntaxEx>(doc);
        }
        else
        {
            qDebug() << "No syntax highlighter available for" << extension << "file type.";
        }

        return nullptr;
    }
};
