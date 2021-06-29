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

static constexpr uint8_t N_CH = vmeplus::V895::GetChNumber(); // 16 channels
static constexpr uint8_t N_GROUPS = 2;
static constexpr uint8_t N_CH_IN_GROUP = N_CH / N_GROUPS; // 8 channels in group

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
        QSpinBox *fThrSpin[N_CH];
        QCheckBox *fEnableCheck[N_CH];
        QSpinBox *fWidthSpin[N_GROUPS];
        QSpinBox *fMajLevelSpin;
        QPushButton *fProgramButton, *fTestButton;

    protected :
        void CreateActions();
        void CreateCentralWidget();

    protected :
        void closeEvent( QCloseEvent *event ) override;

    public slots:
        void SendTest();  
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
