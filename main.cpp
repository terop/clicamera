#include <iostream>

#include <QCommandLineParser>
#include <QFileInfo>
#include <QRegularExpression>

#include "camera.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("clicamera");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("A CLI camera application for Jolla and PC with camera");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Name of the output picture file."));

    parser.addOptions({
        {{"p", "pc"},
             QCoreApplication::translate("main", "Running on PC, i.e. do no Jolla specific modifications")}
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
        std::cerr << "Error: value for the 'filename' argument is invalid, it must be a file ending with '.jp(e)g'" << std::endl;
        return 1;
    }

    filenamePath.makeAbsolute();
    if (!QFileInfo(filenamePath.path()).exists()) {
        std::cerr << "Error: directory for the 'filename' argument does not exist" << std::endl;
        return 1;
    }

    filename = filenamePath.path() + "/" + filenamePath.fileName();

    Camera camera(parser.isSet("pc"));
    camera.setFilename(filename);
    camera.show();

    return app.exec();
}
