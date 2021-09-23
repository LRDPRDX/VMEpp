#pragma once

#include "modules/V6533N.h"
#include "SlaveWindow.h"


class QAction;
class QPushButton;
class QComboBox;
class QSpinBox;
class QTabWidget;
class QVBoxLayout;
class QCheckBox;
class QLineEdit;
class QTimer;

class SLedIndicatorWithLabel;
class SLedIndicator;

class V6533NMonitor;

using namespace vmeplus;


class V6533NWindow : public SlaveWindow
{
    Q_OBJECT

    protected :
        // Auxiliary
        static constexpr uint8_t N_CH = V6533N::GetChNumber(); // 6 channels

    protected :
        // Widgets
        V6533NMonitor     *fMonitor;

        QSpinBox    *fVoltSpin[N_CH], *fCurSpin[N_CH], *fUpSpin[N_CH], *fDownSpin[N_CH], *fSWMaxSpin[N_CH];
        QComboBox   *fOffCombo[N_CH];
        QPushButton *fOnButton[N_CH], *fOffButton[N_CH], *fKillButton;
        QComboBox   *fIMonCombo[N_CH];

    protected :
        // Creational member functions
        void CreateActions();
        void CreateCentralWidget();
        void CreateDockWidget();

    public slots :
        void WriteConfig() override;
        void ReadConfig() override;
        void UpdateMonitor();
        void ChannelOn();
        void ChannelOff();

        QVariant CollectConfig() override;
        void SpreadConfig( const QVariant& qConfig ) override;

    public :
        V6533NWindow( uint32_t address, V2718Window *parent );
        ~V6533NWindow() override = default;
};

class V6533NMonitor : public QWidget
{
    Q_OBJECT

    protected :
        static constexpr uint8_t N_CH = V6533N::GetChNumber(); // 6 channels
        static constexpr uint8_t N_LED = 14;

        V6533NWindow    *fContainer;
        QTimer          *fTimer;

    protected :
        QLineEdit               *fVoltText, *fCurText;
        SLedIndicatorWithLabel  *fAlarmLED[N_CH];
        QPushButton             *fUpdateButton, *fStartButton, *fStopButton;
        QLineEdit               *fVoltMonText[N_CH], *fCurMonText[N_CH], *fTempMonText[N_CH];
        SLedIndicator           *fChStatusLED[N_CH][N_LED];
        SLedIndicator           *fPowFailLED, *fOvPowLED, *fMaxVUncLED, *fMaxIUncLED;

        QVBoxLayout             *fLayout;

    protected :
        void CreateWidgets();
        void CreateGeneralFrame();
        void CreateChannelFrame();
        void CreateTimer();

    protected slots :
        void StartTimer();
        void StopTimer();

    public :
        V6533NMonitor( V6533NWindow* parentWindow, QWidget* parent = nullptr );
        virtual ~V6533NMonitor();

        void Update( const V6533N::MonitorData& data );

        friend void V6533NWindow::UpdateMonitor();
};