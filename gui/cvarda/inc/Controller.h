#pragma once

#include <QMainWindow>

#include "modules/V2718.h"

#include "DeviceWindow.h"

class QAction;
class QPushButton;
class QComboBox;
class QMenu;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;
class QLedIndicatorWithLabel;
class QLineEdit;

class Display;

Q_DECLARE_METATYPE(CVIOPolarity)   
Q_DECLARE_METATYPE(CVLEDPolarity)   
Q_DECLARE_METATYPE(CVIOSources)   

class Controller : public QMainWindow
{
    Q_OBJECT

    public :
        static constexpr uint8_t N_INS = vmeplus::V2718::GetInNumber();
        static constexpr uint8_t N_OUTS = vmeplus::V2718::GetOutNumber();
        static constexpr uint8_t N_PULSERS = 2;

    private :
        vmeplus::V2718 fController;

        Display *fDisplay;

        QMenu   *fViewMenu, *fConfigMenu;
        QAction *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction *fSaveConfigAction, *fLoadConfigAction;
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
        void Program();
        void PulserSlot();
        void ScalerSlot();

    public slots :
        void UpdateDisplay();
        vmeplus::UConfig<vmeplus::V2718> CollectConfig();
        void SpreadConfig( const vmeplus::UConfig<vmeplus::V2718>& cfg );
        void SaveConfig();
        void LoadConfig();
        void StartPulser( CVPulserSelect p );
        void StopPulser( CVPulserSelect p );

    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        Controller( QWidget *parent = nullptr );
        ~Controller();

    public :
        void HandleError( const vmeplus::VException& e );

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
        Controller  *fController;

        QLineEdit               *fAddressText, *fDataText;
        QLedIndicatorWithLabel  *fAddressModLED[N_AM], *fIRQLED[N_IRQ];
        QLedIndicatorWithLabel  *fASLED, *fIACKLED, *fWriteLED, *fLwordLED, *fDS1LED, *fDS2LED;
        QLedIndicatorWithLabel  *fBreqLED, *fBgntLED, *fSresLED, *fDTKLED, *fBERRLED;
        QPushButton             *fUpdateButton;

    protected :
        void CreateDisplay();

    public :
        Display( Controller *controller, QWidget *parent = nullptr );
        ~Display();

        void Update( const CVDisplay &display );
};
