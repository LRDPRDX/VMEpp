#include "Controller.h"
#include "Dialogs.h"

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
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>

#include "VException.h"
#include "qnamespace.h"

Controller::Controller( QWidget *parent ) :
    QMainWindow( parent )
{
    CreateActions();
    CreateMainTab();

    statusBar()->showMessage( "Ready..." );
}

Controller::~Controller() {}

/****** Constructive methods ******/
void Controller::CreateActions()
{
    fConnectAction = new QAction( "&Connect", this );
        connect( this, &Controller::Connected, fConnectAction, &QAction::setDisabled );
    fDisconnectAction = new QAction( "&Disconnect", this );
        connect( this, &Controller::Connected, fDisconnectAction, &QAction::setEnabled );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );

    connect( fConnectAction, &QAction::triggered, this, &Controller::OpenConnectDialog );
    connect( fDisconnectAction, &QAction::triggered, this, &Controller::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &Controller::close );

    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    QMenu *viewMenu = menuBar()->addMenu( "&View" );
        viewMenu->addAction( fViewStatusBarAction );

    connect( fViewStatusBarAction, &QAction::triggered, this, &Controller::ToggleStatusBar );
}

void Controller::CreateMainTab()
{
    fMainTab = new QTabWidget();

    CreateIOTab();
    CreatePulserTab();

    setCentralWidget( fMainTab );
}

void Controller::CreateIOTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Inputs && Outputs") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    QGroupBox *outGroup = new QGroupBox( tr("Outputs") );
    QGridLayout *outLayout = new QGridLayout();
    QStringList srcs = { "VME", "Coincidence", "P && S", "SW" };
    QStringList pols = { "Direct", "Inverted" };
    QStringList leds = { "Active high", "Active low" };
    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        QLabel* srcLabel = new QLabel( tr("Source ") + QString::number(i) + ":" );
        fOutSrcCombo[i] = new QComboBox();
            fOutSrcCombo[i]->addItems( srcs );

        QLabel* polLabel = new QLabel( tr("Polarity:") );
        fOutPolCombo[i] = new QComboBox();
            fOutPolCombo[i]->addItems( pols );

        QLabel* ledLabel = new QLabel( tr("LED polarity:") );
        fOutLedCombo[i] = new QComboBox();
            fOutLedCombo[i]->addItems( leds );

        outLayout->addWidget( srcLabel, i + 1, 0, Qt::AlignRight );
        outLayout->addWidget( fOutSrcCombo[i], i + 1, 1 );
        outLayout->addWidget( polLabel, i + 1, 2, Qt::AlignRight  );
        outLayout->addWidget( fOutPolCombo[i], i + 1, 3 );
        outLayout->addWidget( ledLabel, i + 1, 4, Qt::AlignRight );
        outLayout->addWidget( fOutLedCombo[i], i + 1, 5 );
    }
    outGroup->setLayout( outLayout );

    QGroupBox *inGroup = new QGroupBox( tr("Inputs") );
    QGridLayout *inLayout = new QGridLayout();
    for( uint8_t i = 0; i < N_INS; ++i )
    {
        QLabel* polLabel = new QLabel( tr("Polarity:") );
        fInPolCombo[i] = new QComboBox();
            fInPolCombo[i]->addItems( pols );

        QLabel* ledLabel = new QLabel( tr("LED polarity:") );
        fInLedCombo[i] = new QComboBox();
            fInLedCombo[i]->addItems( leds );

        inLayout->addWidget( polLabel, i + 1, 0, Qt::AlignRight );
        inLayout->addWidget( fInPolCombo[i], i + 1, 1 );
        inLayout->addWidget( ledLabel, i + 1, 2, Qt::AlignRight );
        inLayout->addWidget( fInLedCombo[i], i + 1, 3 );
    }
    inGroup->setLayout( inLayout );

    vLayout->addWidget( outGroup );
    vLayout->addWidget( inGroup );
    tab->setLayout( vLayout );
}

void Controller::CreatePulserTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Pulser && Scaler") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    QStringList startSrcs = { "SW", "Input 0", "Input 1", "Coincidence" };
    QStringList stopSrcs[N_PULSERS] = { { "SW", "Input 0" }, { "SW", "Input 1" } };
    for( uint8_t i = 0; i < N_PULSERS; ++ i )
    {
        QString pulserName = (i == cvPulserA) ? "Pulser A" : "Pulser B";

        QGroupBox *pulserGroup = new QGroupBox( pulserName );
        QGridLayout *pulLayout = new QGridLayout();

        QLabel* freqLabel = new QLabel( tr("Frequency(Hz):") );
        fPulFreqSpin[i] = new QSpinBox();
            fPulFreqSpin[i]->setRange( 0, 10000000 );

        QLabel* dutyLabel = new QLabel( tr("Duty(%):" ) );
        fPulDutySpin[i] = new QSpinBox();
            fPulDutySpin[i]->setRange( 1, 99 );

        QLabel* nLabel = new QLabel( tr("Number of pulses:" ) );
            nLabel->setToolTip( "0 = infinite" );
        fPulNSpin[i] = new QSpinBox();
            fPulNSpin[i]->setRange( 0, 1023 );
            fPulNSpin[i]->setToolTip( "0 = infinite" );

        QLabel* startLabel = new QLabel( tr("Start source:") );
        fPulStartCombo[i] = new QComboBox();
            fPulStartCombo[i]->addItems( startSrcs );

        QLabel* stopLabel = new QLabel( tr("Stop source:") );
        fPulStopCombo[i] = new QComboBox();
            fPulStopCombo[i]->addItems( stopSrcs[i] );

        pulLayout->addWidget( freqLabel, 0, 0, Qt::AlignRight );
        pulLayout->addWidget( fPulFreqSpin[i], 0, 1 );
        pulLayout->addWidget( dutyLabel, 0, 2, Qt::AlignRight );
        pulLayout->addWidget( fPulDutySpin[i], 0, 3 );
        pulLayout->addWidget( nLabel, 0, 4, Qt::AlignRight );
        pulLayout->addWidget( fPulNSpin[i], 0, 5 );
        pulLayout->addWidget( startLabel, 1, 0, Qt::AlignRight );
        pulLayout->addWidget( fPulStartCombo[i], 1, 1 );
        pulLayout->addWidget( stopLabel, 1, 2, Qt::AlignRight );
        pulLayout->addWidget( fPulStopCombo[i], 1, 3 );

        pulserGroup->setLayout( pulLayout );

        vLayout->addWidget( pulserGroup );
    }
    tab->setLayout( vLayout );
}

void Controller::OpenConnectDialog()
{
    Connection *cDialog = new Connection( this );
        cDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        cDialog->setModal( true );
        cDialog->resize( 300, 150 );
        cDialog->setWindowTitle( "Connection dialog" );
    cDialog->show();
}

void Controller::Connect( short link, short conet )
{
    qInfo() << "Openning controller...";
    //Exception may be thrown here
    fController.Open( link, conet );
    qInfo() << "\n...OK";

    setWindowTitle( "gVME++ (V2718)" );
    statusBar()->showMessage( "Connected..." );

    emit Connected( true );
}

void Controller::Disconnect()
{
    qInfo() << "Disconnecting...";

    try
    {
        fController.Close();
    }
    catch( const vmeplus::VException& e )
    {
        qInfo() << "On disconnection : " << e.what();
    }

    setWindowTitle( "gVME++" );
    statusBar()->showMessage( "Disconnected..." );

    emit Connected( false );
}

void Controller::closeEvent( QCloseEvent *event )
{
    const QMessageBox::StandardButton ret =
        QMessageBox::warning( this,
                              tr( "Exit" ),
                              tr( "Are you sure?" ),
                              QMessageBox::Ok | QMessageBox::Cancel );
    switch( ret )
    {
        case( QMessageBox::Ok ) :
            event->accept();
            break;
        default :
            event->ignore();
            break;
    }
}

void Controller::ToggleStatusBar()
{
    if( fViewStatusBarAction->isChecked() )
    {
        statusBar()->show();
    }
    else
    {
        statusBar()->hide();
    }
}
