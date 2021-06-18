#pragma once

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QApplication>

#include "modules/V2718.h"

class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;

class Connection;

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
        void CreateIOTab();
        void CreatePulserTab();

    protected :
        void closeEvent( QCloseEvent *event ) override;

        void Connect( short link, short conet );
        void Disconnect();

    private slots :
        void ToggleStatusBar();
        void OpenConnectDialog();

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        Controller( QWidget *parent = nullptr );
        ~Controller();

    friend class Connection;
};
