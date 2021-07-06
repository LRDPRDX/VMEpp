#include "Controller.h"
#include "V895Window.h"
#include "modules/V895.h"

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

V895Window::V895Window( uint32_t address, Controller *parent ) :
    DeviceWindow( parent )
{
    fDevice = new vmeplus::V895( address );

    setWindowTitle( "V895" );
    CreateActions();
    CreateCentralWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

V895Window::~V895Window()
{
}

void V895Window::CreateActions()
{
    // Main actions
    fConnectAction = new QAction( "&Connect", this );
        connect( this, &V895Window::Connected, fConnectAction, &QAction::setDisabled );
    fDisconnectAction = new QAction( "&Disconnect", this );
        connect( this, &V895Window::Connected, fDisconnectAction, &QAction::setEnabled );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );
    connect( fConnectAction, &QAction::triggered, this, &V895Window::Connect );
    connect( fDisconnectAction, &QAction::triggered, this, &V895Window::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &V895Window::close );
}

void V895Window::CreateCentralWidget()
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
        connect( this, &V895Window::Connected, fTestButton, &QPushButton::setEnabled );
        connect( fTestButton, &QPushButton::clicked, this, &V895Window::SendTest );

    commLayout->addWidget( majLabel, 1, Qt::AlignRight );
    commLayout->addWidget( fMajLevelSpin, 10 );
    commLayout->addWidget( fTestButton, 1 );
    commonGroup->setLayout( commLayout );
    vLayout->addWidget( commonGroup );

    vLayout->addWidget( fProgramButton );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V895Window::SendTest()
{
    try
    {
        static_cast<vmeplus::V895*>(fDevice)->SendTest();
    }
    catch( vmeplus::VException &e )
    {
    }
}

