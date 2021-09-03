#pragma once

#include <QMainWindow>

#include "modules/V2718.h"

#include "DeviceWindow.h"

class QAction;
class SButton;
class QComboBox;
class QMenu;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;
class QLedIndicatorWithLabel;
class QLineEdit;

class Display;

using namespace vmeplus;

Q_DECLARE_METATYPE(CVIOPolarity)   
Q_DECLARE_METATYPE(CVLEDPolarity)   
Q_DECLARE_METATYPE(CVIOSources)   


class V2718Window : public QMainWindow
{
    Q_OBJECT

    public :
        static constexpr uint8_t N_INS = V2718::GetInNumber();
        static constexpr uint8_t N_OUTS = V2718::GetOutNumber();
        static constexpr uint8_t N_PULSERS = 2;

    private :
        V2718 fController;

        Display *fDisplay;

        QMenu   *fViewMenu, *fConfigMenu;
        QAction *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction *fSaveConfigAction, *fLoadConfigAction;
        QAction *fViewStatusBarAction;
        QAction *fAddDeviceAction;

        QTabWidget *fMainTab;
        SButton *fProgramButton, *fReadButton;

        // Inputs and Outputs tab
        QComboBox *fOutSrcCombo[N_OUTS], *fOutPolCombo[N_OUTS], *fOutLedCombo[N_OUTS];
        QComboBox *fInPolCombo[N_INS], *fInLedCombo[N_INS];

        //Pulser and Scaler tab
        QSpinBox  *fPulFreqSpin[N_PULSERS], *fPulDutySpin[N_PULSERS], *fPulNSpin[N_PULSERS];
        QComboBox *fPulStartCombo[N_PULSERS], *fPulStopCombo[N_PULSERS];
        SButton *fPulStartButton[N_PULSERS], *fPulStopButton[N_PULSERS];
        QSpinBox  *fScalLimitSpin;
        QComboBox *fScalHitCombo, *fScalGateCombo, *fScalResetCombo;
        QCheckBox *fScalAutoCheck;
        SButton *fScalGateButton, *fScalStopButton, *fScalResetButton;

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
        void Program();
        void ReadConfig();
        void PulserSlot();

    public slots :
        void UpdateDisplay();
        UConfig<V2718> CollectConfig();
        void SpreadConfig( const UConfig<V2718>& cfg );
        void SaveConfig();
        void LoadConfig();
        void StartPulser( CVPulserSelect p );
        void StopPulser( CVPulserSelect p );
        void StartScaler();
        void StopScaler();
        void ResetScaler();

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        V2718Window( QWidget *parent = nullptr );
        ~V2718Window();

    public :
        void HandleError( const VException& e );

    friend class Connection;
    friend void DeviceWindow::Connect();
};


class Display : public QWidget
{
    Q_OBJECT

    protected :
        static const int N_AM   = 6;
        static const int N_IRQ  = 7;

    protected :
        V2718Window  *fController;

        QLineEdit               *fAddressText, *fDataText;
        QLedIndicatorWithLabel  *fAddressModLED[N_AM], *fIRQLED[N_IRQ];
        QLedIndicatorWithLabel  *fASLED, *fIACKLED, *fWriteLED, *fLwordLED, *fDS1LED, *fDS2LED;
        QLedIndicatorWithLabel  *fBreqLED, *fBgntLED, *fSresLED, *fDTKLED, *fBERRLED;
        SButton                 *fUpdateButton;

    protected :
        void CreateDisplay();

    public :
        Display( V2718Window *controller, QWidget *parent = nullptr );
        ~Display();

        void Update( const CVDisplay &display );
};
