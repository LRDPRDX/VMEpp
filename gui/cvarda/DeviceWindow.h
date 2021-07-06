#pragma once

#include <QMainWindow>

#include "VSlave.h"

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;

class Controller;

class DeviceWindow : public QMainWindow
{
    Q_OBJECT

    private :
        Controller *fParent;

    protected :
        vmeplus::VSlave *fDevice;

    protected :
        void closeEvent( QCloseEvent *event ) override;

    public slots :
        void Connect();
        void Disconnect();
        void OnControllerDisconnect( bool );

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceWindow( Controller *parent );
        virtual ~DeviceWindow();
};
