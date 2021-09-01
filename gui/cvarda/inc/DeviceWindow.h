#pragma once

#include <QMainWindow>

#include "VSlave.h"

class QPushButton;
class QFrame;
class QAction;

class V2718Window;


class DeviceWindow : public QMainWindow
{
    Q_OBJECT

    private :
        V2718Window *fParent;

    protected :
        vmeplus::VSlave *fDevice;

    protected :
        void closeEvent( QCloseEvent *event ) override;
        void CreateFileMenu();

    protected :
        QAction         *fConnectAction, *fDisconnectAction, *fExitAction;
        QPushButton     *fProgramButton, *fReadButton;
        QFrame          *fBottomFrame;

    public slots :
        void Connect();
        void Disconnect();
        void OnControllerDisconnect( bool );
        virtual void Program() = 0;
        virtual void ReadConfig() = 0;


    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceWindow( V2718Window *parent );
        virtual ~DeviceWindow();
};
