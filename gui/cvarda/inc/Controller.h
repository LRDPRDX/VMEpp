#pragma once

#include <QMainWindow>

#include "modules/V2718.h"

#include "DeviceWindow.h"

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;

static constexpr uint8_t N_INS = vmeplus::V2718::GetNInputs();
static constexpr uint8_t N_OUTS = vmeplus::V2718::GetNOutputs();
static constexpr uint8_t N_PULSERS = 2;

class Controller : public QMainWindow
{
    Q_OBJECT

    private :
        vmeplus::V2718 fController;

        QAction *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction *fViewStatusBarAction;
        QAction *fAddDeviceAction;

        QTabWidget *fMainTab;
        QPushButton *fProgramButton;

        // Inputs and Outputs tab
        QComboBox *fOutSrcCombo[N_OUTS], *fOutPolCombo[N_OUTS], *fOutLedCombo[N_OUTS];
        QComboBox *fInPolCombo[N_INS], *fInLedCombo[N_INS];

        //Pulser and Scaler tab
        QSpinBox  *fPulFreqSpin[N_PULSERS], *fPulDutySpin[N_PULSERS], *fPulNSpin[N_PULSERS];
        QComboBox *fPulStartCombo[N_PULSERS], *fPulStopCombo[N_PULSERS];
        QPushButton *fPulStartButton[N_PULSERS], *fPulStopButton[N_PULSERS];
        QSpinBox  *fScalLimitSpin;
        QComboBox *fScalHitCombo, *fScalGateCombo, *fScalResetCombo;
        QCheckBox *fScalAutoCheck;
        QPushButton *fScalStartButton, *fScalGateButton, *fScalResetButton;

        /****** Constructive methods ******/
    private :
        void CreateActions();
        void CreateCentralWidget();
        void CreateDockWidget();
        void CreateIOTab();
        void CreatePulserTab();

    protected :
        void closeEvent( QCloseEvent *event ) override;

        void Connect( short link, short conet );
        void Disconnect();

    private slots :
        void ToggleStatusBar();
        void OpenConnectDialog();
        void OpenDeviceDialog();

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        Controller( QWidget *parent = nullptr );
        ~Controller();

    friend class Connection;
    friend void DeviceWindow::Connect();
};

class Display : public QWidget
{
    Q_OBJECT

    protected :
        static const int N_A    = 32;
        static const int N_D    = 32;
        static const int N_AM   = 6;
        static const int N_IRQ  = 7;
        static const int N_DS   = 2;

    protected :
        QCheckBox   *fAddressLED[N_A], *fDataLED[N_D], *fAddressModLED[N_AM], *fIRQLED[N_IRQ], *fDSLED[N_DS];
        QCheckBox   *fASLED, *fIACKLED, *fWriteLED, *fLwordLED;
        QCheckBox   *fBreqLED, *fBgntLED, *fSresLED, *fDTKLED, *fBERRLED;
        QPushButton *fUpdateButton;

    protected :
        void CreateDisplay();

    public :
        Display( QWidget *parent = nullptr );
        ~Display();
};
