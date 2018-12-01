#include <iostream>

#include <cstdlib>

#include <QCameraInfo>
#include <QCameraViewfinder>

#include "camera.h"

using namespace std;

Q_DECLARE_METATYPE(QCameraInfo)

Camera::Camera(QWidget *parent) : QMainWindow(parent)
{
    if(!Camera::isCameraAvailable())
        return;

    camera.reset(new QCamera(QCameraInfo::defaultCamera()));

    imageCapture.reset(new QCameraImageCapture(camera.data()));
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

    camera->setCaptureMode(QCamera::CaptureStillImage);

    connect(imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &Camera::capture);
    connect(imageCapture.data(), &QCameraImageCapture::imageSaved, this, &Camera::imageSaved);
}

void Camera::capture()
{
    camera->searchAndLock();

    imageCapture->capture(Camera::getFilename());

    camera->unlock();
}

void Camera::show()
{
    if (Camera::isGuiEnabled()) {
        QCameraViewfinder *viewfinder = new QCameraViewfinder;
        camera->setViewfinder(viewfinder);
        viewfinder->show();
    }

    camera->start();
}

void Camera::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);

    cout << "Image saved to " << fileName.toStdString() << endl;
    exit(0);
}

bool Camera::isCameraAvailable()
{
    int cameraCount = QCameraInfo::availableCameras().count();
    if (cameraCount == 0) {
        cerr << "No camera available, exiting" << endl;
        return false;
    } else if (cameraCount > 1) {
        cerr << "More than one camera available, only one camera is supported, exiting" << endl;
        return false;
    }

    return true;
}

void Camera::setFilename(QString filename)
{
    this->filename = filename;
}

QString Camera::getFilename()
{
    return this->filename;
}

void Camera::setGuiEnabled(bool value)
{
    this->guiEnabled = value;
}

bool Camera::isGuiEnabled()
{
    return this->guiEnabled;
}
