#include "../include/MainWindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QFont>
#include <QFontDatabase>

// Function to create a round icon
QIcon createRoundIcon(const QString &iconPath)
{
    QPixmap pixmap(iconPath);

    // Create a round mask
    QBitmap mask(pixmap.size());
    mask.fill(Qt::white);

    QPainter painter(&mask);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, pixmap.width(), pixmap.height());

    pixmap.setMask(mask);

    return QIcon(pixmap);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QIcon roundIcon = createRoundIcon(":/resources/app_icon.png");
    QFont font      = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(12);
    
    app.setFont(font);
    app.setWindowIcon(roundIcon);
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Chris Dedman");
    app.setApplicationName("CodeAstra");

    MainWindow window;
    window.show();

    return app.exec();
}