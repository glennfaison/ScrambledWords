#include <QApplication>

#include "mainmenu.h"

#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(QStringLiteral("ScrambledWords"));
    QCoreApplication::setApplicationName(QStringLiteral("ScrambledWords"));

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Jumble - a game of scrambled words."));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    MainMenu window;
    window.show();

    return app.exec();
}
