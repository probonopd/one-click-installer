#ifndef CONFLICTRESOLUTIONSCREEN_H
#define CONFLICTRESOLUTIONSCREEN_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "oci_helper_interface.h"
class ConflictResolutionScreen : public QWidget
{
    Q_OBJECT
public:
    ConflictResolutionScreen();
private:
    QVBoxLayout *m_mainLayout;
    QPushButton *m_cancelInstallation;
    QPushButton *m_continueInstallation;
    org::opensuse::OCIHelper *m_ociHelper;
    
private slots:
    void cancelInstallation();
    void sendSolutionToOCIHelper();
    void problemSolutionWidget( QString problem, QString solutions );
};

#endif
