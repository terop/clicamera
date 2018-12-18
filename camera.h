#ifndef CAMERA_H
#define CAMERA_H

#include <QCamera>
#include <QCameraImageCapture>

class Camera : public QObject
{
     Q_OBJECT

private:
    QScopedPointer<QCamera> camera;
    QScopedPointer<QCameraImageCapture> imageCapture;
    QString filename;
    bool pc;

    void capture();
    bool isCameraAvailable();
    void imageSaved(int id, const QString &fileName);
    QString getFilename();
    bool isPc();

public:
    Camera(bool pc, QObject *parent = 0);
    void show();
    void setFilename(QString filename);
};

#endif // CAMERA_H
