#include "V2718Window.h"
#include "V6533NWindow.h"
#include "modules/V6533N.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>
#include <QDockWidget>
#include <QLineEdit>
#include <QSignalMapper>

#include "Style.h"
#include "QLedIndicator.h"

#include "VException.h"
#include "qnamespace.h"


V6533NWindow::V6533NWindow( uint32_t address, V2718Window *parent ) :
    DeviceWindow( parent )
{
    fDevice = new vmeplus::V6533N( address );

    setWindowTitle( "V6533N" );
    CreateActions();
    CreateCentralWidget();
    CreateDockWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

V6533NWindow::~V6533NWindow()
{
}

void V6533NWindow::CreateActions()
{

}

void V6533NWindow::CreateDockWidget()
{
    QDockWidget *dock = new QDockWidget( "V6533N Monitor", this );
    fMonitor = new Monitor( this, dock );
    dock->setWidget( fMonitor );
    addDockWidget( Qt::RightDockWidgetArea, dock );
    fViewMenu->addAction( dock->toggleViewAction() );
}

void V6533NWindow::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        QGroupBox *chGroup = new QGroupBox( QString( "Channel %1" ).arg( ch ) );
        QGridLayout *gridLayout = new QGridLayout();

        QLabel *voltLabel = new QLabel( "U (V):" );
        fVoltSpin[ch] = new QSpinBox();
            fVoltSpin[ch]->setRange( 0, 6000 );

        QLabel *curLabel = new QLabel( "I (uA):" );
        fCurSpin[ch] = new QSpinBox();
            fCurSpin[ch]->setRange( 0, 3000 );

        QLabel *upLabel = new QLabel( "Up (V/s):" );
        fUpSpin[ch] = new QSpinBox();
            fUpSpin[ch]->setRange( 0, 400 );
            fUpSpin[ch]->setValue( 100 );

        QLabel *downLabel = new QLabel( "Down (V/s):" );
        fDownSpin[ch] = new QSpinBox();
            fDownSpin[ch]->setRange( 0, 400 );
            fDownSpin[ch]->setValue( 100 );

        QLabel *swMaxLabel = new QLabel( "Max (V):" );
        fSWMaxSpin[ch] = new QSpinBox();
            fSWMaxSpin[ch]->setRange( 0, 6000 );

        QLabel *offLabel = new QLabel( "OFF:" );
        fOffCombo[ch] = new QComboBox();
            fOffCombo[ch]->addItem( "OFF", 0 );
            fOffCombo[ch]->addItem( "KILL", 1 );

        SFrame *buttonFrame = new SFrame( SColor_t::VIOLET ); 
        QVBoxLayout *buttonLayout = new QVBoxLayout();


        fOnButton[ch] = new SButton( "ON", SColor_t::VIOLET );
           connect( this, &DeviceWindow::Connected, fOnButton[ch], &QPushButton::setEnabled );
           connect( fOnButton[ch], &SButton::clicked, this, &V6533NWindow::ChannelOn );
        fOffButton[ch] = new SButton( "OFF", SColor_t::RED );
           connect( this, &DeviceWindow::Connected, fOffButton[ch], &QPushButton::setEnabled );
           connect( fOffButton[ch], &SButton::clicked, this, &V6533NWindow::ChannelOff );

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
        gridLayout->addWidget( offLabel, 1, 4, Qt::AlignRight ); 
        gridLayout->addWidget( fOffCombo[ch], 1, 5 );

        gridLayout->addWidget( buttonFrame, 0, 6, 2, 1 );

        chGroup->setLayout( gridLayout );
        vLayout->addWidget( chGroup );
    }

    vLayout->addWidget( fBottomFrame );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V6533NWindow::Program()
{
    try
    {
        static_cast<V6533N*>( fDevice )->WriteConfig( CollectConfig() );
        emit Programmed( true );
    }
    catch( const VException& e )
    {
        emit Programmed( false );
    }
}

void V6533NWindow::ReadConfig()
{
    UConfig<V6533N> cfg;

    try
    {
        static_cast<V6533N*>(fDevice)->ReadConfig( cfg );
        SpreadConfig( cfg );
    }
    catch( const VException& e )
    {
    }
}

void V6533NWindow::UpdateMonitor()
{
    V6533N* hv = static_cast<V6533N*>( fDevice );
    try
    {
        V6533N::MonitorData md;
        hv->ReadMonitor( md );
        fMonitor->Update( md );
    }
    catch( const VException& e )
    {
        qInfo() << e.what() << "\n";
    }
}

void V6533NWindow::SpreadConfig( const UConfig<V6533N>& cfg )
{
    auto changeCombo = []( QComboBox* c, int data ) {
        int index = c->findData( data  );
        if( index >= 0 )
        {
            c->setCurrentIndex( index );
        }
    };

    for( uint8_t i = 0; i < N_CH; ++i )
    {
        fVoltSpin[i]->setValue( cfg.CHANNELS.at( i ).V_SET );
        fCurSpin[i]->setValue( cfg.CHANNELS.at( i ).I_SET );
        fSWMaxSpin[i]->setValue( cfg.CHANNELS.at( i ).SW_MAX );
        fUpSpin[i]->setValue( cfg.CHANNELS.at( i ).RAMP_UP );
        fDownSpin[i]->setValue( cfg.CHANNELS.at( i ).RAMP_DOWN );
        changeCombo( fOffCombo[i], cfg.CHANNELS.at( i ).PW_DOWN );
    }
}

UConfig<V6533N> V6533NWindow::CollectConfig()
{
    UConfig<V6533N> cfg;

    for( uint8_t i = 0; i < N_CH; ++i )
    {
        cfg.CHANNELS.at( i ).V_SET = fVoltSpin[i]->value();    
        cfg.CHANNELS.at( i ).I_SET = fCurSpin[i]->value();    
        cfg.CHANNELS.at( i ).SW_MAX = fSWMaxSpin[i]->value();    
        cfg.CHANNELS.at( i ).RAMP_UP = fUpSpin[i]->value();    
        cfg.CHANNELS.at( i ).RAMP_DOWN = fDownSpin[i]->value();    
        cfg.CHANNELS.at( i ).PW_DOWN = fOffCombo[i]->currentData().value<bool>();  
    }

    return cfg;
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
        static_cast<V6533N*>( fDevice )->WriteEnable( ch, true );
    }
    catch( const VException& e )
    {
        qInfo() << e.what() << "\n";
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
        qInfo() << e.what() << "\n";
    }
}

//*********************
//****** MONITOR ******
//*********************
Monitor::Monitor( V6533NWindow* container, QWidget* parent ) :
    QWidget( parent ),
    fContainer( container )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    CreateGeneralFrame();
}

Monitor::~Monitor()
{
}

void Monitor::CreateGeneralFrame()
{
    QVBoxLayout *layout = new QVBoxLayout();

    SFrame *upperFrame = new SFrame( SColor_t::VIOLET );
    QGridLayout *upperLayout = new QGridLayout();

    QLabel *vLabel = new QLabel( "Max Voltage [V]:" );
    fVoltText = new QLineEdit();
        fVoltText->setReadOnly( true );

    QLabel *cLabel = new QLabel( "Max Current [uA]:" );
    fCurText = new QLineEdit();
        fCurText->setReadOnly( true );

    fPowFailLED = new QLedIndicatorWithLabel( "POWER FAIL", false );
    fOvPowLED = new QLedIndicatorWithLabel( "OVER POWER", false );
    fMaxVUncLED = new QLedIndicatorWithLabel( "UNCALIBRATED", true );
    fMaxIUncLED = new QLedIndicatorWithLabel( "UNCALIBRATED", true );

    upperLayout->addWidget( vLabel, 0, 0, Qt::AlignRight );
    upperLayout->addWidget( fVoltText, 0, 1 );
    upperLayout->addWidget( fMaxVUncLED, 0, 2 );
    upperLayout->addWidget( cLabel, 1, 0, Qt::AlignRight );
    upperLayout->addWidget( fCurText, 1, 1 );
    upperLayout->addWidget( fMaxIUncLED, 1, 2 );

    upperFrame->setLayout( upperLayout );
    layout->addWidget( upperFrame );

    SFrame *lowerFrame = new SFrame( SColor_t::VIOLET );
    QGridLayout *lowerLayout = new QGridLayout();

    for( int i = 0; i < N_CH/2; ++i )
    {
        fAlarmLED[i] = new QLedIndicatorWithLabel( QString( "ALARM CH%1" ).arg( i ), false );
        lowerLayout->addWidget( fAlarmLED[i], i, 0 );
    }

    for( int i = N_CH/2; i < N_CH; ++i )
    {
        fAlarmLED[i] = new QLedIndicatorWithLabel( QString( "ALARM CH%1" ).arg( i ), true );
        lowerLayout->addWidget( fAlarmLED[i], i - N_CH/2, 1 );
    }

    lowerLayout->setSpacing( 1 );
    lowerFrame->setLayout( lowerLayout );
    layout->addWidget( lowerFrame );

    fUpdateButton = new SButton( "UPDATE", SColor_t::VIOLET );
        connect( fContainer, &V6533NWindow::Connected, fUpdateButton, &SButton::setEnabled );
        connect( fUpdateButton, &QPushButton::clicked, fContainer, &V6533NWindow::UpdateMonitor );
    fStartButton = new SButton( "START", SColor_t::VIOLET );
        connect( fContainer, &V6533NWindow::Connected, fStartButton, &SButton::setEnabled );
    fStopButton = new SButton( "STOP", SColor_t::RED );
        connect( fContainer, &V6533NWindow::Connected, fStopButton, &SButton::setEnabled );

    SFrame *buttonFrame = new SFrame( SColor_t::VIOLET ); 
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget( fUpdateButton );
    buttonLayout->addWidget( fStartButton );
    buttonLayout->addWidget( fStopButton );

    buttonFrame->setLayout( buttonLayout );
    layout->addWidget( buttonFrame );

    setLayout( layout );
}

void Monitor::Update( const V6533N::MonitorData& m )
{
    fVoltText->setText( QString::number( m.V_MAX ) );
    fCurText->setText( QString::number( m.I_MAX ) );
    
    fMaxVUncLED->SetChecked( m.STATUS & (1 << 10) );
    fMaxIUncLED->SetChecked( m.STATUS & (1 << 11) );

    for( uint8_t i = 0; i < N_CH; ++i )
    {
        fAlarmLED[i]->SetChecked( m.STATUS & (1 << i) );
    }
}
