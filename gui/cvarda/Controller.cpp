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
#include <QCheckBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>

#include "VException.h"
#include "qnamespace.h"

Controller::Controller( QWidget *parent ) :
    QMainWindow( parent )
{
    CreateActions();
    CreateCentralWidget();

    emit Connected( false );
    statusBar()->showMessage( "Ready..." );
}

Controller::~Controller() {}

/****** Constructive methods ******/
void Controller::CreateActions()
{
    // Main actions
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

    // View actions
    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    QMenu *viewMenu = menuBar()->addMenu( "&View" );
        viewMenu->addAction( fViewStatusBarAction );
    connect( fViewStatusBarAction, &QAction::triggered, this, &Controller::ToggleStatusBar );

    // Add actions
    fAddDeviceAction = new QAction( "Device" );
        //connect( this, &Controller::Connected, fAddDeviceAction, &QAction::setEnabled );
    QMenu *addMenu = menuBar()->addMenu( "&Add" );
        addMenu->addAction( fAddDeviceAction );
    connect( fAddDeviceAction, &QAction::triggered, this, &Controller::OpenDeviceDialog );
}

void Controller::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    fMainTab = new QTabWidget();

    CreateIOTab();
    CreatePulserTab();

    fProgramButton = new QPushButton( "PROGRAM" );
        connect( this, &Controller::Connected, fProgramButton, &QPushButton::setEnabled );

    vLayout->addWidget( fMainTab );
    vLayout->addWidget( fProgramButton );
    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}


void Controller::CreateIOTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, tr("Inputs && Outputs") );

    QVBoxLayout *vLayout = new QVBoxLayout();

    QGroupBox *outGroup = new QGroupBox( tr("Outputs") );
    QGridLayout *outLayout = new QGridLayout();
    for( uint8_t i = 0; i < N_OUTS; ++i )
    {
        QLabel* srcLabel = new QLabel( tr("Source:") );
        fOutSrcCombo[i] = new QComboBox();
            fOutSrcCombo[i]->addItem( "VME", cvVMESignals );
            fOutSrcCombo[i]->addItem( "Coincidence", cvCoincidence );
            fOutSrcCombo[i]->addItem( "P & S", cvMiscSignals );
            fOutSrcCombo[i]->addItem( "SW", cvManualSW );

        QLabel* polLabel = new QLabel( tr("Polarity:") );
        fOutPolCombo[i] = new QComboBox();
            fOutPolCombo[i]->addItem( "Direct", cvDirect );
            fOutPolCombo[i]->addItem( "Inverted", cvInverted );

        QLabel* ledLabel = new QLabel( tr("LED polarity:") );
        fOutLedCombo[i] = new QComboBox();
            fOutLedCombo[i]->addItem( "Active high", cvActiveHigh );
            fOutLedCombo[i]->addItem( "Active low", cvActiveLow );

        outLayout->addWidget( srcLabel, i + 1, 0, Qt::AlignRight );
        outLayout->addWidget( fOutSrcCombo[i], i + 1, 1 );
        outLayout->addWidget( polLabel, i + 1, 2, Qt::AlignRight );
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
            fInPolCombo[i]->addItem( "Direct", cvDirect  );
            fInPolCombo[i]->addItem( "Inverted", cvInverted  );

        QLabel* ledLabel = new QLabel( tr("LED polarity:") );
        fInLedCombo[i] = new QComboBox();
            fInLedCombo[i]->addItem( "Active high", cvActiveHigh );
            fInLedCombo[i]->addItem( "Active low", cvActiveLow );

        inLayout->addWidget( polLabel, i, 0, Qt::AlignRight );
        inLayout->addWidget( fInPolCombo[i], i, 1 );
        inLayout->addWidget( ledLabel, i, 2, Qt::AlignRight );
        inLayout->addWidget( fInLedCombo[i], i, 3 );
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

    // Pulsers
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
            fPulStartCombo[i]->addItem( "SW", cvManualSW );
            fPulStartCombo[i]->addItem( "Input 0", cvInputSrc0 );
            fPulStartCombo[i]->addItem( "Input 1", cvInputSrc1 );
            fPulStartCombo[i]->addItem( "Coincidence", cvCoincidence );

        QLabel* stopLabel = new QLabel( tr("Stop source:") );
        fPulStopCombo[i] = new QComboBox();
            fPulStopCombo[i]->addItem( "SW", cvManualSW );
            fPulStopCombo[i]->addItem( "Input " + QString::number( i ), (i == 0) ? cvInputSrc0 : cvInputSrc1 );

        fPulStartButton[i] = new QPushButton( "START" ); 
            connect( this, &Controller::Connected, fPulStartButton[i], &QPushButton::setEnabled );
        fPulStopButton[i] = new QPushButton( "STOP" );
            connect( this, &Controller::Connected, fPulStopButton[i], &QPushButton::setEnabled );

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
        pulLayout->addWidget( fPulStartButton[i], 1, 4 );
        pulLayout->addWidget( fPulStopButton[i], 1, 5 );

        pulserGroup->setLayout( pulLayout );

        vLayout->addWidget( pulserGroup );
    }
    // Scaler
    QGroupBox *scalerGroup = new QGroupBox( "Scaler" );
    QGridLayout *scalLayout = new QGridLayout();

    QLabel *limitLabel = new QLabel( "Limit:" );
    fScalLimitSpin = new QSpinBox();
        fScalLimitSpin->setRange( 0, 1023 );
    fScalAutoCheck = new QCheckBox( "Auto reset" );    
    fScalGateButton = new QPushButton( "GATE" );
        connect( this, &Controller::Connected, fScalGateButton, &QPushButton::setEnabled );
    fScalStartButton = new QPushButton( "START" );
        connect( this, &Controller::Connected, fScalStartButton, &QPushButton::setEnabled );
    fScalResetButton = new QPushButton( "RESET" );
        connect( this, &Controller::Connected, fScalResetButton, &QPushButton::setEnabled );

    QLabel *hitLabel = new QLabel( "Hit source:" );
    fScalHitCombo = new QComboBox();
        fScalHitCombo->addItem( "Input 0", cvInputSrc0 );
        fScalHitCombo->addItem( "Coincidence", cvCoincidence );
    QLabel *gateLabel = new QLabel( "Gate source:" );
    fScalGateCombo = new QComboBox();
        fScalGateCombo->addItem( "SW", cvManualSW );
        fScalGateCombo->addItem( "Input 1", cvInputSrc1 );
    QLabel *resetLabel = new QLabel( "Reset source:" );
    fScalResetCombo = new QComboBox();
        fScalResetCombo->addItem( "SW", cvManualSW );
        fScalResetCombo->addItem( "Input 1", cvInputSrc1 );

    scalLayout->addWidget( limitLabel, 0, 0, Qt::AlignRight );
    scalLayout->addWidget( fScalLimitSpin, 0, 1 );
    scalLayout->addWidget( fScalGateButton, 0, 3 );
    scalLayout->addWidget( fScalStartButton, 0, 4 );
    scalLayout->addWidget( fScalResetButton, 0, 5 );
    scalLayout->addWidget( hitLabel, 1, 0, Qt::AlignRight );
    scalLayout->addWidget( fScalHitCombo, 1, 1 );
    scalLayout->addWidget( gateLabel, 1, 2, Qt::AlignRight );
    scalLayout->addWidget( fScalGateCombo, 1, 3 );
    scalLayout->addWidget( resetLabel, 1, 4, Qt::AlignRight );
    scalLayout->addWidget( fScalResetCombo, 1, 5 );
    scalLayout->addWidget( fScalAutoCheck, 2, 0, 1, 6, Qt::AlignRight );

    scalerGroup->setLayout( scalLayout );

    vLayout->addWidget( scalerGroup );

    tab->setLayout( vLayout );
}

/****** SLOTS ******/
void Controller::OpenConnectDialog()
{
    Connection *cDialog = new Connection( this );
        cDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        cDialog->setModal( true );
        cDialog->resize( 300, 150 );
        cDialog->setWindowTitle( "Connection dialog" );
    cDialog->show();
}

void Controller::OpenDeviceDialog()
{
    DeviceDialog *devDialog = new DeviceDialog( this );
        devDialog->setAttribute( Qt::WA_DeleteOnClose, true );
        devDialog->setModal( true );
        devDialog->setWindowTitle( "Device dialog" );
    devDialog->show();
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
