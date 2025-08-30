#include "SyntaxManager.h"
#include "Syntax.h"

#include <QDir>
#include <QFile>
#include <QDebug>

void SyntaxManager::initializeUserSyntaxConfig()
{
    QString userSyntaxDir = QDir::homePath() + "/.config/codeastra/syntax";
    QDir dir(userSyntaxDir);

    if (!dir.exists())
    {
        qDebug() << "[Setup] First run detected. Creating syntax config directory:" << userSyntaxDir;

        if (!dir.mkpath("."))
        {
            qWarning() << "[Setup] Failed to create user syntax config directory:" << userSyntaxDir;
            return;
        }

        // List of default syntax files to copy
        QDir defaultDir(":/resources/syntax/");
        QStringList defaultSyntaxFiles = defaultDir.entryList({"*.yaml", "*.yml"}, QDir::Files);

        for (const QString &fileName : defaultSyntaxFiles)
        {
            QString resourcePath = ":/resources/syntax/" + fileName;
            QString destPath     = userSyntaxDir + "/" + fileName;

            QFile resFile(resourcePath);
            if (resFile.copy(destPath))
            {
                qDebug() << "[Setup] Copied default config:" << fileName;
            }
            else
            {
                qWarning() << "[Setup] Failed to copy:" << resourcePath << "to" << destPath;
            }
        }
    }
    else
    {
        qDebug() << "[Setup] User syntax directory already exists. Skipping first-run config.";
    }
}

std::unique_ptr<QSyntaxHighlighter> SyntaxManager::createSyntaxHighlighter(const QString &extension, QTextDocument *doc)
{
    QString baseDir;

    if (qEnvironmentVariableIsSet("CONFIG_DIR"))
    {
        baseDir = qEnvironmentVariable("CONFIG_DIR");
    }
    else
    {
        QString userSyntaxDir = QDir::homePath() + "/.config/codeastra/syntax";
        if (QDir(userSyntaxDir).exists())
        {
            baseDir = userSyntaxDir;
        }
        else
        {
            baseDir = "config";
        }
    }

//     qDebug() << "[SyntaxManager] Using config directory:" << baseDir;

    QDir syntaxDir(baseDir);
    QStringList yamlFilePath = syntaxDir.entryList({"*.yaml", "*.yml"}, QDir::Files);

    std::vector<YAML::Node> config;
    // Iterate over all YAML files and store their contents as separate nodes
    for (const QString &fileName : yamlFilePath)
    {
        QFile file(syntaxDir.filePath(fileName));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            YAML::Node fileConfig = YAML::Load(file.readAll().toStdString());
            file.close();
            // qDebug() << "[SyntaxManager] Loaded config for extension:" << extension << "from:" << file.fileName();

            config.push_back(fileConfig);
        }
        else
        {
            qWarning() << "[SyntaxManager] Failed to open syntax config for extension:" << extension << "at:" << yamlFilePath;
            return nullptr;
        }
    }

    return createHighlighter(doc, config, extension);
}

std::unique_ptr<QSyntaxHighlighter> SyntaxManager::createHighlighter(QTextDocument *doc, const std::vector<YAML::Node> &config, const QString &extension)
{
    // qDebug() << "[SyntaxManager] Creating highlighter for extension:" << extension;
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
