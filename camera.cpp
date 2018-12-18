#include <iostream>
#include <cstdlib>

#include <QCameraInfo>
#include "camera.h"

using namespace std;

Q_DECLARE_METATYPE(QCameraInfo)

Camera::Camera(bool pc, QObject *parent) : QObject(parent)
{
    this->pc = pc;

    if (Camera::isCameraAvailable())
        camera.reset(new QCamera(QCameraInfo::defaultCamera()));
     else
        camera.reset(new QCamera("/dev/video0"));

    imageCapture.reset(new QCameraImageCapture(camera.data()));
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);

    camera->setCaptureMode(QCamera::CaptureStillImage);

    if (!Camera::isPc()) {
        QImageEncoderSettings imageSettings;
        imageSettings.setCodec("image/jpeg");
        imageSettings.setResolution(3264, 1840);
        imageSettings.setQuality(QMultimedia::HighQuality);
        imageCapture->setEncodingSettings(imageSettings);

        // Disable flash
        QCameraExposure *exposure = camera->exposure();
        exposure->setFlashMode(QCameraExposure::FlashOff);
        exposure->setManualAperture(2.4);
    }

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
    camera->start();
}

void Camera::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);

    cout << "Image saved to " << fileName.toStdString() << endl;

    if (!Camera::isPc()) {
        // Rotate picture and replace the original
        QImage image(fileName);
        QImage rotated = image.transformed(QTransform().rotate(90));
        rotated.save(fileName, "jpg", 90);
    }

    exit(0);
}

bool Camera::isCameraAvailable()
{
    int cameraCount = QCameraInfo::availableCameras().count();
    if (cameraCount == 0) {
        cerr << "No default camera available, using device /dev/video0" << endl;
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

bool Camera::isPc()
{
    return this->pc;
}
