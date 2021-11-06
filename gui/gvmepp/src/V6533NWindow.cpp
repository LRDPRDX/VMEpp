#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>
#include <QDockWidget>
#include <QLineEdit>
#include <QSignalMapper>
#include <QTimer>
#include "qnamespace.h"

#include "V2718Window.h"
#include "V6533NWindow.h"
#include "VException.h"
#include "modules/V6533N.h"

#include "Style.h"
#include "Prelude.h"

using namespace vmepp;


V6533NWindow::V6533NWindow( uint32_t address, V2718Window *parent ) :
    SlaveWindow( parent )
{
    fDevice = new V6533N( address );

    setWindowTitle( "V6533N" );
    CreateMenu();
    CreateCentralWidget();
    CreateDockWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

//**********************************
//****** CONSTRUCTIVE METHODS ******
//**********************************
void V6533NWindow::CreateMenu()
{
    connect( fSaveConfigAction, &QAction::triggered, this, &DeviceWindow::SaveConfig<V6533N> );
    connect( fLoadConfigAction, &QAction::triggered, this, &DeviceWindow::LoadConfig<V6533N> );

    fOffAllAction = new QAction( "&Off all channels", this );
        connect( fOffAllAction, &QAction::triggered, this, &V6533NWindow::OffAll );

    fActionMenu = menuBar()->addMenu( "&Action" ); fActionMenu->addAction( fOffAllAction );
}

void V6533NWindow::CreateDockWidget()
{
    QDockWidget *dock = new QDockWidget( "V6533N::Monitor", this );
    fMonitor = new V6533NMonitor( this, dock );
    dock->setWidget( fMonitor );
    addDockWidget( Qt::RightDockWidgetArea, dock );
    dock->setFeatures( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable );
    fViewMenu->addAction( dock->toggleViewAction() );
    dock->setVisible( false );
}

void V6533NWindow::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        SGroupBox *chGroup = new SGroupBox( QString( "Channel %1" ).arg( ch ) );
        QGridLayout *gridLayout = new QGridLayout();

        QLabel *voltLabel = new QLabel( "U (V):" );
        fVoltSpin[ch] = new QSpinBox();
            fVoltSpin[ch]->setRange( 0, 6000 );
            fVoltSpin[ch]->setToolTip( "Voltage" );

        QLabel *curLabel = new QLabel( "I (uA):" );
        fCurSpin[ch] = new QSpinBox();
            fCurSpin[ch]->setRange( 0, 3000 );
            fCurSpin[ch]->setValue( 100 );
            fCurSpin[ch]->setToolTip( "Max current supplied" );

        QLabel *upLabel = new QLabel( "Up (V/s):" );
        fUpSpin[ch] = new QSpinBox();
            fUpSpin[ch]->setRange( 0, 400 );
            fUpSpin[ch]->setValue( 100 );
            fUpSpin[ch]->setToolTip( "Ramp up" );

        QLabel *downLabel = new QLabel( "Down (V/s):" );
        fDownSpin[ch] = new QSpinBox();
            fDownSpin[ch]->setRange( 0, 400 );
            fDownSpin[ch]->setValue( 100 );

        QLabel *swMaxLabel = new QLabel( "Max (V):" );
        fSWMaxSpin[ch] = new QSpinBox();
            fSWMaxSpin[ch]->setRange( 0, 6000 );
            fSWMaxSpin[ch]->setValue( 100 );

        QLabel *offLabel = new QLabel( "PW Down:" );
        fOffCombo[ch] = new QComboBox();
            fOffCombo[ch]->addItem( "Ramp Down", 0 );
            fOffCombo[ch]->addItem( "KILL", 1 );

        QLabel *tripLabel = new QLabel( "Time (s):");
        fTripSpin[ch] = new QDoubleSpinBox();
            fTripSpin[ch]->setRange( 0, 999.9 );
            fTripSpin[ch]->setSingleStep( 0.1 );
            fTripSpin[ch]->setDecimals( 1 );
            fTripSpin[ch]->setValue( 0.1 );
            fTripSpin[ch]->setToolTip( "Max. time an \"overcurrent\" is allowed to last.\nOutput voltage will drop to zero either at the Ramp-Down rate\nor at the fastest available rate, depending on the Power Down setting." );
            fTripSpin[ch]->setToolTipDuration( 10000 );

        QLabel *iMonLabel = new QLabel( "Imon:" );
        fIMonCombo[ch] = new QComboBox();
            fIMonCombo[ch]->addItem( "300uA", QVariant::fromValue( V6533N::IMonRange_t::RANGE_LOW ) );
            fIMonCombo[ch]->addItem( "3000uA", QVariant::fromValue( V6533N::IMonRange_t::RANGE_HIGH ) );
            fIMonCombo[ch]->setToolTip( "IMPORTANT: if 300uA is selected, and the channel\ndraws a current larger than 300uA\nthen OVERCURRENT is signalled." );
            fIMonCombo[ch]->setToolTipDuration( 10000 );

        SFrame *buttonFrame = new SFrame( SColor_t::VIOLET );
        QHBoxLayout *buttonLayout = new QHBoxLayout();


        fOnButton[ch] = new QPushButton( "ON" );
            connect( this, &DeviceWindow::Connected, fOnButton[ch], &QPushButton::setEnabled );
            connect( fOnButton[ch], &QPushButton::clicked, this, &V6533NWindow::ChannelOn );
            ColorButton( fOnButton[ch], style::yellow );
        fOffButton[ch] = new QPushButton( "OFF" );
            connect( this, &DeviceWindow::Connected, fOffButton[ch], &QPushButton::setEnabled );
            connect( fOffButton[ch], &QPushButton::clicked, this, &V6533NWindow::ChannelOff );
            ColorButton( fOffButton[ch], style::red );

        buttonLayout->addWidget( fOnButton[ch] );
        buttonLayout->addWidget( fOffButton[ch] );
        buttonFrame->setLayout( buttonLayout );

        gridLayout->addWidget( voltLabel, 0, 0, Qt::AlignRight );
        gridLayout->addWidget( fVoltSpin[ch], 0, 1 );
        gridLayout->addWidget( curLabel, 0, 2, Qt::AlignRight );
        gridLayout->addWidget( fCurSpin[ch], 0, 3 );
        gridLayout->addWidget( swMaxLabel, 0, 4, Qt::AlignRight );
        gridLayout->addWidget( fSWMaxSpin[ch], 0, 5 );
        gridLayout->addWidget( upLabel, 1, 0, Qt::AlignRight );
        gridLayout->addWidget( fUpSpin[ch], 1, 1 );
        gridLayout->addWidget( downLabel, 1, 2, Qt::AlignRight );
        gridLayout->addWidget( fDownSpin[ch], 1, 3 );
        gridLayout->addWidget( iMonLabel, 1, 4, Qt::AlignRight );
        gridLayout->addWidget( fIMonCombo[ch], 1, 5 );
        gridLayout->addWidget( offLabel, 1, 6, Qt::AlignRight );
        gridLayout->addWidget( fOffCombo[ch], 1, 7 );
        gridLayout->addWidget( tripLabel, 1, 8, Qt::AlignRight );
        gridLayout->addWidget( fTripSpin[ch], 1, 9 );

        gridLayout->addWidget( buttonFrame, 0, 6, 1, 4 );


        chGroup->setLayout( gridLayout );
        vLayout->addWidget( chGroup );
    }

    vLayout->addWidget( fBottomFrame );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

//*******************
//****** SLOTS ******
//*******************
void V6533NWindow::WriteConfig()
{
    try
    {
        UConfig<V6533N> cfg = qvariant_cast< UConfig<V6533N> >( this->CollectConfig() );
        static_cast<V6533N*>(fDevice)->WriteConfig( cfg );
        emit Programmed( true );
    }
    catch( const VException& e )
    {
        emit Programmed( false );
        emit Error( e );
    }
}

void V6533NWindow::ReadConfig()
{
    UConfig<V6533N> cfg;

    try
    {
        static_cast<V6533N*>(fDevice)->ReadConfig( cfg );
        QVariant qv; qv.setValue( cfg );
        SpreadConfig( qv );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V6533NWindow::UpdateMonitor()
{
    V6533N* hv = static_cast<V6533N*>( fDevice );
    try
    {
        V6533N::MonitorData md = hv->ReadMonitor();
        fMonitor->Update( md );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V6533NWindow::SpreadConfig( const QVariant& qConfig )
{
    UConfig<V6533N> cfg = qvariant_cast< UConfig<V6533N> >( qConfig );
    for( uint8_t i = 0; i < N_CH; ++i )
    {
        fVoltSpin[i]->setValue( cfg.CHANNELS.at( i ).V_SET );
        fCurSpin[i]->setValue( cfg.CHANNELS.at( i ).I_SET );
        fSWMaxSpin[i]->setValue( cfg.CHANNELS.at( i ).SW_MAX );
        fUpSpin[i]->setValue( cfg.CHANNELS.at( i ).RAMP_UP );
        fDownSpin[i]->setValue( cfg.CHANNELS.at( i ).RAMP_DOWN );
        fTripSpin[i]->setValue( cfg.CHANNELS.at( i ).TRIP_TIME );
        ChangeCombo( fOffCombo[i], cfg.CHANNELS.at( i ).PW_DOWN );
        ChangeCombo( fIMonCombo[i], cfg.CHANNELS.at( i ).IMON_RANGE );
    }
}

QVariant V6533NWindow::CollectConfig()
{
    UConfig<V6533N> cfg;

    for( uint8_t i = 0; i < N_CH; ++i )
    {
        cfg.CHANNELS.at( i ).V_SET = fVoltSpin[i]->value();
        cfg.CHANNELS.at( i ).I_SET = fCurSpin[i]->value();
        cfg.CHANNELS.at( i ).SW_MAX = fSWMaxSpin[i]->value();
        cfg.CHANNELS.at( i ).RAMP_UP = fUpSpin[i]->value();
        cfg.CHANNELS.at( i ).RAMP_DOWN = fDownSpin[i]->value();
        cfg.CHANNELS.at( i ).TRIP_TIME = fTripSpin[i]->value();
        cfg.CHANNELS.at( i ).PW_DOWN = fOffCombo[i]->currentData().value<bool>();
        cfg.CHANNELS.at( i ).IMON_RANGE = fIMonCombo[i]->currentData().value<V6533N::IMonRange_t>();
    }

    QVariant qv; qv.setValue( cfg );

    return qv;
}

void V6533NWindow::ChannelOn()
{
    QPushButton *button = qobject_cast<QPushButton*>( sender() );
    if( button == nullptr )
    {
        return;
    }

    uint8_t ch;
    bool found = false;
    for( uint8_t i = 0; i < N_CH; ++i )
    {
        if( button == fOnButton[i] )
        {
            found = true;
            ch = i;
            break;
        }
    }

    if( not found )
    {
        return;
    }

    try
    {
        V6533N* hv = static_cast<V6533N*>( fDevice );
        hv->WriteVoltage( ch, fVoltSpin[ch]->value() );
        hv->WriteCurrent( ch, fCurSpin[ch]->value() );
        hv->WriteSWVMax( ch, fSWMaxSpin[ch]->value() );
        hv->WriteRampUp( ch, fUpSpin[ch]->value() );
        hv->WriteRampDown( ch, fDownSpin[ch]->value() );
        hv->WritePWDown( ch, fOffCombo[ch]->currentData().value<bool>() );
        hv->WriteIMonRange( ch, fIMonCombo[ch]->currentData().value<V6533N::IMonRange_t>() );
        hv->WriteEnable( ch, true );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V6533NWindow::ChannelOff()
{
    QPushButton *button = qobject_cast<QPushButton*>( sender() );
    if( button == nullptr )
    {
        return;
    }

    uint8_t ch;
    bool found = false;
    for( uint8_t i = 0; i < N_CH; ++i )
    {
        if( button == fOffButton[i] )
        {
            found = true;
            ch = i;
            break;
        }
    }

    if( not found )
    {
        return;
    }

    try
    {
        static_cast<V6533N*>( fDevice )->WriteEnable( ch, false );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

void V6533NWindow::OffAll()
{
    try
    {
        V6533N* hv = static_cast<V6533N*>( fDevice );
        for( uint8_t ch = 0; ch < N_CH; ++ch )
        {
            hv->WriteEnable( ch, false ); // off
        }
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

//*********************
//****** MONITOR ******
//*********************
V6533NMonitor::V6533NMonitor( V6533NWindow* container, QWidget* parent ) :
    QWidget( parent ),
    fContainer( container )
{
    fLayout = new QVBoxLayout();

    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    CreateTimer();
    CreateWidgets();

    setLayout( fLayout );
}

void V6533NMonitor::CreateWidgets()
{
    CreateGeneralFrame();
    CreateChannelFrame();

    fUpdateButton = new QPushButton( "UPDATE" );
        ColorButton( fUpdateButton, style::yellow );
        connect( fContainer, &V6533NWindow::Connected, fUpdateButton, &QPushButton::setEnabled );
        connect( fUpdateButton, &QPushButton::clicked, fContainer, &V6533NWindow::UpdateMonitor );
    fStartButton = new QPushButton( "START" );
        ColorButton( fStartButton, style::yellow );
        connect( fContainer, &V6533NWindow::Connected, fStartButton, &QPushButton::setEnabled );
        connect( fStartButton, &QPushButton::clicked, this, &V6533NMonitor::StartTimer );
    fStopButton = new QPushButton( "STOP" );
        ColorButton( fStopButton, style::red );
        connect( fContainer, &V6533NWindow::Connected, fStopButton, &QPushButton::setEnabled );
        connect( fStopButton, &QPushButton::clicked, this, &V6533NMonitor::StopTimer );

    SFrame *buttonFrame = new SFrame( SColor_t::VIOLET );
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget( fUpdateButton );
    buttonLayout->addWidget( fStartButton );
    buttonLayout->addWidget( fStopButton );

    buttonFrame->setLayout( buttonLayout );
    fLayout->addWidget( buttonFrame );
}

void V6533NMonitor::CreateGeneralFrame()
{
    SFrame *generalFrame = new SFrame( SColor_t::VIOLET );
    QGridLayout *gLayout = new QGridLayout();

    QLabel *vLabel = new QLabel( "Max Voltage:" );
    QLabel *vqLabel = new QLabel( "V" );
    fVoltText = new QLineEdit();
        fVoltText->setReadOnly( true );
        fVoltText->setFixedWidth( 60 );

    QLabel *cLabel = new QLabel( "Max Current:" );
    QLabel *cqLabel = new QLabel( "uA" );
    fCurText = new QLineEdit();
        fCurText->setReadOnly( true );
        fCurText->setFixedWidth( 60 );

    fPowFailLED = new SLedIndicator( 18 );
        fPowFailLED->setToolTip( "POWER FAILED" );
    fOvPowLED = new SLedIndicator( 18 );
        fOvPowLED->setToolTip( "OVER POWER" );
    fMaxVUncLED = new SLedIndicator( 18 );
        fMaxVUncLED->setToolTip( "MAX V UNCALIBRATED" );
    fMaxIUncLED = new SLedIndicator( 18 );
        fMaxIUncLED->setToolTip( "MAX I UNCALIBRATED" );

    gLayout->addWidget( vLabel, 0, 0, Qt::AlignRight );
    gLayout->addWidget( fVoltText, 0, 1 );
    gLayout->addWidget( vqLabel, 0, 2, Qt::AlignLeft );
    gLayout->addWidget( fMaxVUncLED, 0, 3, Qt::AlignHCenter );
    gLayout->addWidget( fPowFailLED, 0, 4, Qt::AlignHCenter );
    gLayout->addWidget( cLabel, 1, 0, Qt::AlignRight );
    gLayout->addWidget( fCurText, 1, 1 );
    gLayout->addWidget( cqLabel, 1, 2, Qt::AlignLeft );
    gLayout->addWidget( fMaxIUncLED, 1, 3, Qt::AlignHCenter );
    gLayout->addWidget( fOvPowLED, 1, 4, Qt::AlignHCenter );


    generalFrame->setLayout( gLayout );
    fLayout->addWidget( generalFrame );
}

void V6533NMonitor::CreateChannelFrame()
{
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        SGroupBox *chGroup = new SGroupBox( QString( "Channel %1" ).arg( ch ) );
        QGridLayout *gridLayout = new QGridLayout();

        QLabel *voltLabel = new QLabel( "V" );
        fVoltMonText[ch] = new QLineEdit();
            fVoltMonText[ch]->setReadOnly( true );
            fVoltMonText[ch]->setFixedWidth( 50 );

        QLabel *curLabel = new QLabel( "uA" );
        fCurMonText[ch] = new QLineEdit();
            fCurMonText[ch]->setReadOnly( true );
            fCurMonText[ch]->setFixedWidth( 50 );

        QLabel *tempLabel = new QLabel( "\u00b0C" );
        fTempMonText[ch] = new QLineEdit();
            fTempMonText[ch]->setReadOnly( true );
            fTempMonText[ch]->setFixedWidth( 50 );

        gridLayout->addWidget( fVoltMonText[ch], 0, 0 );
        gridLayout->addWidget( voltLabel, 0, 1, Qt::AlignLeft );
        gridLayout->addWidget( fCurMonText[ch], 0, 2 );
        gridLayout->addWidget( curLabel, 0, 3, Qt::AlignLeft );

        SFrame* ledFrame = new SFrame( SColor_t::RED );
        QGridLayout* ledLayout = new QGridLayout();
            ledLayout->setSpacing( 1 );
        QString ledNames[N_LED] = { "ON", "RAMP UP", "RAMP DOWN", "OVER CURRENT", "OVER VOLTAGE", "UNDER VOLTAGE",
                                   "MAX V", "MAX I", "TRIP", "OVER POWER", "OVER TEMPERATURE", "DISABLED",
                                   "INTERLOCK", "UNCALIBRATED" };
        for( uint8_t l = 0; l < N_LED; ++l )
        {
            fChStatusLED[ch][l] = new SLedIndicator( 10 );
            fChStatusLED[ch][l]->setToolTip( ledNames[l] );
            ledLayout->addWidget( fChStatusLED[ch][l], 0, l );
        }
        fChStatusLED[ch][0]->SetColor( style::green );// the ON LED should be green

        ledFrame->setLayout( ledLayout );

        gridLayout->addWidget( ledFrame, 0, 4 );

        gridLayout->addWidget( fTempMonText[ch], 0, 5 );
        gridLayout->addWidget( tempLabel, 0, 6, Qt::AlignLeft );

        chGroup->setLayout( gridLayout );

        fLayout->addWidget( chGroup );
    }
}

void V6533NMonitor::CreateTimer()
{
    fTimer = new QTimer( this );
        fTimer->setInterval( 1000 );
    connect( fTimer, &QTimer::timeout, fContainer, &V6533NWindow::UpdateMonitor );
    connect( fContainer, &V6533NWindow::Connected, this, &V6533NMonitor::StopTimer );
    connect( fContainer, &V6533NWindow::Error, this, &V6533NMonitor::StopTimer );
}

void V6533NMonitor::Update( const V6533N::MonitorData& m )
{
    fVoltText->setText( QString().sprintf( "%.1f", m.V_MAX ) );
    fCurText->setText( QString().sprintf( "%.1f", m.I_MAX ) );

    fMaxVUncLED->SetState( m.STATUS & (1 << 10) );
    fMaxIUncLED->SetState( m.STATUS & (1 << 11) );
    fPowFailLED->SetState( m.STATUS & (1 << 8) );
    fOvPowLED->SetState(   m.STATUS & (1 << 9) );

    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        fVoltMonText[ch]->setText( QString().sprintf("%.1f", m.CHANNELS.at(ch).VOLTAGE ) );
        fCurMonText[ch]->setText( QString().sprintf("%.1f", m.CHANNELS.at(ch).CURRENT ) );
        fTempMonText[ch]->setText( QString::number( m.CHANNELS.at(ch).TEMP ) );

        for( uint8_t l = 0; l < N_LED; ++l )
        {
            fChStatusLED[ch][l]->SetState( m.CHANNELS.at(ch).STATUS & (1 << l) );
        }
    }
}

void V6533NMonitor::StartTimer()
{
    if( fTimer->isActive() )
    {
        return;
    }
    fTimer->start();
}

void V6533NMonitor::StopTimer()
{
    if( fTimer->isActive() )
    {
        fTimer->stop();
    }
}
