#pragma once

#include <QMainWindow>

#include "VSlave.h"

class QPushButton;
class QAction;

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
        void CreateFileMenu();

    protected :
        QAction         *fConnectAction, *fDisconnectAction, *fExitAction;
        QPushButton     *fProgramButton;

    public slots :
        void Connect();
        void Disconnect();
        void OnControllerDisconnect( bool );
        virtual void Program() = 0;

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceWindow( Controller *parent );
        virtual ~DeviceWindow();
};
