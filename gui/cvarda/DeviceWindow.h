#pragma once

#include <QMainWindow>

#include "VSlave.h"

class QPushButton;

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

    protected :
        QPushButton     *fProgramButton;

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
