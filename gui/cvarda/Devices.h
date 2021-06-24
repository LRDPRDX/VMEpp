#pragma once

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QApplication>

#include "modules/V895.h"

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;

class Controller;

class DeviceV895 : public QMainWindow
{
    Q_OBJECT

    private :
        Controller  *fParent;

    private :
        vmeplus::V895 fDevice;

    protected :
        QAction *fConnectAction, *fDisconnectAction, *fExitAction;

    protected :
        void CreateActions();

    protected :
        void closeEvent( QCloseEvent *event ) override;

    public :
        void Connect();
        void Disconnect();
        void OnControllerDisconnect( bool );

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceV895( uint32_t address, Controller *parent );
        ~DeviceV895();
};
