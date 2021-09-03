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

#include "Style.h"

#include "VException.h"
#include "qnamespace.h"


V6533NWindow::V6533NWindow( uint32_t address, V2718Window *parent ) :
    DeviceWindow( parent )
{
    fDevice = new vmeplus::V6533N( address );

    setWindowTitle( "V6533N" );
    CreateActions();
    CreateCentralWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

V6533NWindow::~V6533NWindow()
{
}

void V6533NWindow::CreateActions()
{
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

        fOnButton = new SButton( "ON", SColor_t::VIOLET );
           connect( this, &DeviceWindow::Connected, fOnButton, &QPushButton::setEnabled );
        fOffButton = new SButton( "OFF", SColor_t::RED );
           connect( this, &DeviceWindow::Connected, fOffButton, &QPushButton::setEnabled );

        buttonLayout->addWidget( fOnButton );
        buttonLayout->addWidget( fOffButton );
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
        fVoltSpin[i]->setValue( cfg.CHANNELS.at( i ).VOLTAGE );
        fCurSpin[i]->setValue( cfg.CHANNELS.at( i ).CURRENT );
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
        cfg.CHANNELS.at( i ).VOLTAGE = fVoltSpin[i]->value();    
        cfg.CHANNELS.at( i ).CURRENT = fCurSpin[i]->value();    
        cfg.CHANNELS.at( i ).SW_MAX = fSWMaxSpin[i]->value();    
        cfg.CHANNELS.at( i ).RAMP_UP = fUpSpin[i]->value();    
        cfg.CHANNELS.at( i ).RAMP_DOWN = fDownSpin[i]->value();    
        cfg.CHANNELS.at( i ).PW_DOWN = fOffCombo[i]->currentData().value<bool>();  
    }

    return cfg;
}
