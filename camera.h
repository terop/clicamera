#ifndef CAMERA_H
#define CAMERA_H

#include <QMainWindow>

#include <QCamera>
#include <QCameraImageCapture>

class Camera : public QMainWindow
{
     Q_OBJECT

private:
    QScopedPointer<QCamera> camera;
    QScopedPointer<QCameraImageCapture> imageCapture;
    QString filename;
    bool guiEnabled;

    void capture();
    bool isCameraAvailable();
    void imageSaved(int id, const QString &fileName);
    QString getFilename();
    bool isGuiEnabled();

public:
    Camera(QWidget *parent = 0);
    void show();
    void setFilename(QString filename);
    void setGuiEnabled(bool value);
};

#endif // CAMERA_H
