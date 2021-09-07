#pragma once

#include <QMainWindow>

#include "VSlave.h"

class SButton;
class SFrame;
class QAction;
class QMenu;

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
        QMenu           *fViewMenu, *fConfigMenu;
        QAction         *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction         *fSaveConfigAction, *fLoadConfigAction;
        QAction         *fViewStatusBarAction;

        SButton         *fProgramButton, *fReadButton;
        SFrame          *fBottomFrame;

    public slots :
        void Connect();
        void Disconnect();
        void OnControllerDisconnect( bool );
        void ToggleStatusBar();
        virtual void Program() = 0;
        virtual void ReadConfig() = 0;


    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceWindow( V2718Window *parent );
        virtual ~DeviceWindow();
};
