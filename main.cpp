#include <QtWidgets>
#include <QCommandLineParser>

#include "camera.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("clicamera");
    QApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QApplication::translate("main", "Name of the output picture file."));

    parser.addOptions({
            {{"g", "gui"},
              QApplication::translate("main", "Show GUI (currently only viewfinder)")},
        });

    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (!args.length()) {
        std::cerr << "Error: value for argument 'filename' not provided" << std::endl;
        return 1;
    }

    QString filename;
    QFileInfo filenamePath(args.at(0));

    if (!filenamePath.fileName().contains(QRegularExpression("\\w+\\.jpe?g"))) {
        std::cerr << "Error: value for the 'filename' argument is invalid, it must be a file" << std::endl;
        return 1;
    }

    filenamePath.makeAbsolute();
    if (!QFileInfo(filenamePath.path()).exists()) {
        std::cerr << "Error: directory for the 'filename' argument does not exist" << std::endl;
        return 1;
    }

    filename = filenamePath.path() + "/" + filenamePath.fileName();

    Camera camera;
    camera.setFilename(filename);
    camera.setGuiEnabled(parser.isSet("gui"));

    camera.show();

    return app.exec();
}
