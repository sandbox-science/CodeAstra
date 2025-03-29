#include "SyntaxManager.h"
#include "Syntax.h"

#include <QDir>
#include <QFile>
#include <QDebug>

std::unique_ptr<QSyntaxHighlighter> SyntaxManager::createSyntaxHighlighter(const QString &extension, QTextDocument *doc)
{
    QString configPath = qgetenv("CONFIG_DIR");
    if (configPath.isEmpty())
    {
        configPath = "config";
    }

    QDir syntaxDir(configPath);

    QStringList yamlFiles = syntaxDir.entryList({"*.yaml", "*.yml"}, QDir::Files);
    qDebug() << "Directory being scanned: " << syntaxDir.absolutePath();

    if (syntaxDir.exists())
    {
        qDebug() << "Directory exists.";
    }
    else
    {
        qDebug() << "Directory does not exist.";
    }

    std::vector<YAML::Node> config;
    // Iterate over all YAML files and store their contents as separate nodes
    for (const QString &fileName : yamlFiles)
    {
        QFile file(syntaxDir.filePath(fileName));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            YAML::Node fileConfig = YAML::Load(file.readAll().toStdString());
            file.close();
            qDebug() << "Loaded YAML from: " << file.fileName();

            config.push_back(fileConfig);
        }
        else
        {
            qDebug() << "Failed to open file: " << file.fileName();
        }
    }

    return createHighlighter(doc, config, extension);
}

std::unique_ptr<QSyntaxHighlighter> SyntaxManager::createHighlighter(QTextDocument *doc, const std::vector<YAML::Node> &config, const QString &extension)
{
    qDebug() << "Creating highlighter for extension:" << extension;
    for (const auto &node : config)
    {
        if (node["extensions"])
        {
            for (const auto &ext : node["extensions"])
            {
                std::string extensionInConfig = ext.as<std::string>();
                if (extensionInConfig == extension.toStdString())
                {
                    return std::make_unique<Syntax>(doc, node);
                }
            }
        }
        else
        {
            qDebug() << "No extensions key in YAML config.";
        }
    }

    qDebug() << "No matching highlighter found for extension:" << extension;
    return nullptr;
}
