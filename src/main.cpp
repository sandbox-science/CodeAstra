#include "MainWindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QFont>
#include <QFontDatabase>
#include <QFile>

// Function to create a round icon
QIcon createRoundIcon(const QString &iconPath)
{
    QPixmap pixmap(iconPath);
    if (pixmap.isNull())
    {
        qWarning() << "Failed to load icon:" << iconPath;
        return QIcon();
    }

    QPixmap roundPixmap(pixmap.size());
    roundPixmap.fill(Qt::transparent);

    QPainter painter(&roundPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, pixmap.width(), pixmap.height());

    return QIcon(roundPixmap);
}

QFont setFont()
{
    QStringList preferreFontFamilies = {"Monaco", "Menlo", "Consolas", "Courier New", "Monospace"};
    QStringList availableFamilies    = QFontDatabase::families();

    QString chosenFamily;
    for(QString family: preferreFontFamilies)
    {
        if (availableFamilies.contains(family))
        {
            chosenFamily = family;
            break;
        }
    }

    if (chosenFamily.isEmpty())
    {
        chosenFamily = QFontDatabase::systemFont(QFontDatabase::FixedFont).family();
    }

    QFont font;
    font.setFamily(chosenFamily);
    font.setFixedPitch(true);
    font.setPointSize(13);
    
    return font;
}

QPalette setPalette()
{
	QPalette palette;
    palette.setColor(QPalette::Base, QColor("#1e1e1e"));
    palette.setColor(QPalette::Text, QColor("#d4d4d4"));
    
    return palette;
}

QString getStyleConfig()
{
	QFile styleFile(":/resources/themes/dark.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        return QString::fromUtf8(styleFile.readAll());
    }
    
    return QString{};
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	QIcon windowIcon = createRoundIcon(":/resources/app_icon.png");
    QFont font       = setFont();
    QPalette palette = setPalette();
    QString theme    = getStyleConfig();
    if (!theme.isEmpty())
    {
    	app.setStyleSheet(theme);
    }

    app.setPalette(palette);
    app.setFont(font);
    app.setWindowIcon(windowIcon);
    app.setStyle("Fusion");

    app.setApplicationVersion(QStringLiteral("0.2.0"));
    app.setOrganizationName(QStringLiteral("Chris Dedman"));
    app.setApplicationName(QStringLiteral("CodeAstra"));
    app.setApplicationDisplayName(QStringLiteral("CodeAstra"));

    QScopedPointer<MainWindow> window(new MainWindow);
    window->show();

    return app.exec();
}