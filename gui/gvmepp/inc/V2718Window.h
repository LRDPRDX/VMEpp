#pragma once

#include <array>

#include <QMainWindow>
#include "SlaveWindow.h"

#include "modules/V2718.h"

class QAction;
class SButton;
class QComboBox;
class QMenu;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;
class SLedIndicatorWithLabel;
class QLineEdit;

class Display;

using namespace vmepp;

class V2718Window : public DeviceWindow
{
    Q_OBJECT

    public :
        static constexpr uint8_t N_INS = V2718::GetInNumber();
        static constexpr uint8_t N_OUTS = V2718::GetOutNumber();
        static constexpr uint8_t N_PULSERS = 2;

    /* Constructive methods */
    private :
        void CreateMenu();
        void CreateCentralWidget();
        void CreateDockWidget();
        void CreateIOTab();
        void CreatePulserTab();

    private :
        V2718 fController;

        Display *fDisplay;

        QMenu   *fAddMenu;
        QAction *fAddDeviceAction;

        QTabWidget *fMainTab;

        // Inputs and Outputs tab
        QComboBox *fOutSrcCombo[N_OUTS], *fOutPolCombo[N_OUTS], *fOutLedCombo[N_OUTS];
        QComboBox *fInPolCombo[N_INS], *fInLedCombo[N_INS];

        //Pulsers and Scaler tab
        QSpinBox  *fPulFreqSpin[N_PULSERS], *fPulDutySpin[N_PULSERS], *fPulNSpin[N_PULSERS];
        QComboBox *fPulStartCombo[N_PULSERS], *fPulStopCombo[N_PULSERS];
        SButton *fPulStartButton[N_PULSERS], *fPulStopButton[N_PULSERS];
        QSpinBox  *fScalLimitSpin;
        QComboBox *fScalHitCombo, *fScalGateCombo, *fScalResetCombo;
        QCheckBox *fScalAutoCheck;
        SButton *fScalGateButton, *fScalStopButton, *fScalResetButton;

    protected :
        void closeEvent( QCloseEvent *event ) override;
        void DoOnError( const VException& error ) override;

    private slots :
        void OpenConnectDialog();
        void OpenDeviceDialog();
        void PulserSlot();

    public slots :
        void WriteConfig() override;
        void ReadConfig() override;
        QVariant CollectConfig() override;
        void SpreadConfig( const QVariant& config ) override;
        void Disconnect() override;
        void Connect() override;
        void Connect( short link, short conet );

        void UpdateDisplay();
        void StartPulser( V2718::Pulser_t p );
        void StopPulser( V2718::Pulser_t p );
        void StartScaler();
        void StopScaler();
        void ResetScaler();

    public :
        V2718Window( QWidget *parent = nullptr );
        ~V2718Window() override = default;

    public :
        friend class Connection;
        friend void SlaveWindow::Connect();
};


class Display : public QWidget
{
    Q_OBJECT

    protected :
        static const size_t N_LED = 24;
        static const std::array<QString, N_LED> fLEDNames;
        static size_t GetLED( const QString& name );

    protected :
        V2718Window  *fController;

        QLineEdit               *fAddressText, *fDataText;
        SLedIndicatorWithLabel  *fLED[N_LED];
        SButton                 *fUpdateButton;

    protected :
        void CreateDisplay();

    public :
        Display( V2718Window *controller, QWidget *parent = nullptr );
        virtual ~Display();

        void Update( const CVDisplay &display );
};
