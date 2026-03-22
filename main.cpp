#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Kernel/kernel.h"
#include <QStringListModel>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("RCStudio");
    app.setApplicationName("PismoGenerator");

    Kernel appKernel;
    ArchitektDataModel *archModel = appKernel.getModelArchitekci();
   // QStringListModel archModel;
   // archModel.setStringList(appKernel.getListaArchitekci());



    QQmlApplicationEngine engine;
    engine.addImportPath(QString("%1/Kernel").arg(QGuiApplication::applicationDirPath()));
    engine.addImportPath(QString("%1/UI").arg(QGuiApplication::applicationDirPath()));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.setInitialProperties({
        {"backEnd", QVariant::fromValue(&appKernel)},
        {"archModel", QVariant::fromValue(archModel)},
    });
    engine.loadFromModule("ListGenerator", "Main");

    return QCoreApplication::exec();
}
