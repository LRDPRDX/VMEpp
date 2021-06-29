#include "Controller.h"
#include "Devices.h"

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

#include "VException.h"
#include "qnamespace.h"

DeviceV895::DeviceV895( uint32_t address, Controller *parent ) :
    QMainWindow( parent ),
    fParent( parent ),
    fDevice( address )
{
    resize( 300, 150 );
    setWindowTitle( "V895" );

    connect( fParent, &Controller::Connected, this, &DeviceV895::OnControllerDisconnect );

    CreateActions();
    CreateCentralWidget();

    emit Connected( false );
}

DeviceV895::~DeviceV895()
{
}

void DeviceV895::CreateActions()
{
    // Main actions
    fConnectAction = new QAction( "&Connect", this );
        connect( this, &DeviceV895::Connected, fConnectAction, &QAction::setDisabled );
    fDisconnectAction = new QAction( "&Disconnect", this );
        connect( this, &DeviceV895::Connected, fDisconnectAction, &QAction::setEnabled );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );
    connect( fConnectAction, &QAction::triggered, this, &DeviceV895::Connect );
    connect( fDisconnectAction, &QAction::triggered, this, &DeviceV895::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &DeviceV895::close );
}

void DeviceV895::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    QHBoxLayout *chLayout = new QHBoxLayout();
    QGroupBox *chGroup = new QGroupBox( "Channels" );

    for( int nG = 0; nG < N_GROUPS; ++nG )
    {
        QGroupBox *groupOfEight = new QGroupBox( QString::number(nG * N_CH_IN_GROUP) +
                                                 "-" + QString::number((nG + 1) * N_CH_IN_GROUP - 1) );
        QGridLayout *gridLayout = new QGridLayout();
        for( uint8_t i = 0; i < N_CH_IN_GROUP; ++i )
        {
            uint8_t ch = i + N_CH_IN_GROUP * nG;
            QLabel* thrLabel = new QLabel( tr("Threshold :") );
            fThrSpin[ch] = new QSpinBox();
                fThrSpin[ch]->setRange( 0, 0xFF );

            fEnableCheck[ch] = new QCheckBox( "Enable" );

            gridLayout->addWidget( thrLabel, i, 0, Qt::AlignRight );
            gridLayout->addWidget( fThrSpin[ch], i, 1 );
            gridLayout->addWidget( fEnableCheck[ch], i, 2 );
            if( i == N_CH_IN_GROUP - 1 )
            {
                QLabel* widthLabel = new QLabel( "Width :" );
                fWidthSpin[nG] = new QSpinBox();
                    fWidthSpin[nG]->setRange( 4, 40 );
                gridLayout->addWidget( widthLabel, i, 3, Qt::AlignRight );
                gridLayout->addWidget( fWidthSpin[nG], i, 4 );
            }
        }
        groupOfEight->setLayout( gridLayout );
        chLayout->addWidget( groupOfEight );
    }
    chGroup->setLayout( chLayout );
    vLayout->addWidget( chGroup );

    QGroupBox *commonGroup = new QGroupBox( "Common" );
    QHBoxLayout *commLayout = new QHBoxLayout();

    QLabel *majLabel = new QLabel( "Majority level: " );

    fMajLevelSpin = new QSpinBox();
        fMajLevelSpin->setRange( 0, N_CH );

    fTestButton = new QPushButton( "TEST" );
        connect( this, &DeviceV895::Connected, fTestButton, &QPushButton::setEnabled );
        connect( fTestButton, &QPushButton::clicked, this, &DeviceV895::SendTest );

    commLayout->addWidget( majLabel, 1, Qt::AlignRight );
    commLayout->addWidget( fMajLevelSpin, 10 );
    commLayout->addWidget( fTestButton, 1 );
    commonGroup->setLayout( commLayout );
    vLayout->addWidget( commonGroup );

    fProgramButton = new QPushButton( "PROGRAM" );
        connect( this, &DeviceV895::Connected, fProgramButton, &QPushButton::setEnabled );

    vLayout->addWidget( fProgramButton );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void DeviceV895::OnControllerDisconnect( bool status )
{
    if( not status )
    {
        Disconnect();
    }
}

void DeviceV895::SendTest()
{
    fDevice.SendTest();
}

void DeviceV895::Connect()
{
    bool success = true;
    try
    {
        fParent->fController.RegisterSlave( &fDevice );
    }
    catch( const vmeplus::VException &e )
    {
        success = false;

        QMessageBox::warning( this,
                              tr( "Connection FAILED" ),
                              tr( e.what() ),
                              QMessageBox::Ok );
    }

    statusBar()->showMessage( success ? "Connected..." : "Disconnected..." );
    emit Connected( success );
}

void DeviceV895::Disconnect()
{
    fDevice.Release();

    statusBar()->showMessage( "Disconnected..." );
    emit Connected( false );
}

void DeviceV895::closeEvent( QCloseEvent *event )
{
    const QMessageBox::StandardButton ret =
        QMessageBox::warning( this,
                              tr( "Exit" ),
                              tr( "Are you sure?" ),
                              QMessageBox::Ok | QMessageBox::Cancel );
    switch( ret )
    {
        case( QMessageBox::Ok ) :
            Disconnect();
            event->accept();
            break;
        default :
            event->ignore();
            break;
    }
}
