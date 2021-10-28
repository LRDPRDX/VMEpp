#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QFileDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QDockWidget>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTabWidget>
#include "qnamespace.h"

#include "V2718Window.h"
#include "V1785NWindow.h"
#include "VException.h"
#include "modules/V1785N.h"

#include "Style.h"
#include "Prelude.h"

using namespace vmepp;


V1785NWindow::V1785NWindow( uint32_t address, V2718Window* parent ) :
    SlaveWindow( parent )
{
    fDevice = new V1785N( address );

    setWindowTitle( "V1785N" );

    CreateMenu();
    CreateCentralWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

void V1785NWindow::CreateMenu()
{
    connect( fSaveConfigAction, &QAction::triggered, this, &DeviceWindow::SaveConfig<V1785N> );
    connect( fLoadConfigAction, &QAction::triggered, this, &DeviceWindow::LoadConfig<V1785N> );
}

void V1785NWindow::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    fMainTab = new QTabWidget();

    CreateGeneralTab();
    CreateChannelsTab();
    CreatePlotTab();

    vLayout->addWidget( fMainTab );
    vLayout->addWidget( fBottomFrame );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V1785NWindow::CreateGeneralTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, "General" );

    QVBoxLayout *vLayout = new QVBoxLayout();

    QGroupBox *suppGroup = new QGroupBox( "Suppression" );
    QGridLayout *suppLayout = new QGridLayout();

    fZeroSuppCheck = new QCheckBox( "Zero suppression" );
    fOverSuppCheck = new QCheckBox( "Over suppression" );

    QLabel* zeroSuppTypeLabel = new QLabel( "Zero suppression type:" );
    fZeroSuppTypeCombo = new QComboBox();
        fZeroSuppTypeCombo->addItem( "x16", qVariantFromValue( V1785N::ZeroSupp_t::Tx16) );
        fZeroSuppTypeCombo->addItem( "x2", qVariantFromValue( V1785N::ZeroSupp_t::Tx2 ) );

    suppLayout->addWidget( fZeroSuppCheck, 0, 0 );
    suppLayout->addWidget( fOverSuppCheck, 1, 0 );
    suppLayout->addWidget( zeroSuppTypeLabel, 0, 1, Qt::AlignRight );
    suppLayout->addWidget( fZeroSuppTypeCombo, 0, 2 );

    suppGroup->setLayout( suppLayout );

    QGroupBox *irqGroup = new QGroupBox( "Interrupt" );
    QGridLayout *irqLayout = new QGridLayout();

    QLabel* irqLevelLabel = new QLabel( "Level:" );
    fIRQLevelCombo = new QComboBox();
        fIRQLevelCombo->addItem( "DISABLED", 0 );
        fIRQLevelCombo->addItem( "IRQ1", 1 );
        fIRQLevelCombo->addItem( "IRQ2", 2 );
        fIRQLevelCombo->addItem( "IRQ3", 3 );
        fIRQLevelCombo->addItem( "IRQ4", 4 );
        fIRQLevelCombo->addItem( "IRQ5", 5 );
        fIRQLevelCombo->addItem( "IRQ6", 6 );
        fIRQLevelCombo->addItem( "IRQ7", 7 );

    QLabel* irqVectorLabel = new QLabel( "Vector:" );
    fIRQVectorSpin = new QSpinBox();
        fIRQVectorSpin->setMinimum( 0 );
        fIRQVectorSpin->setMaximum( 0xff );

    QLabel* irqEventsLabel = new QLabel( "Events:" );
    fIRQEventsSpin = new QSpinBox();
        fIRQEventsSpin->setMinimum( 1 );
        fIRQEventsSpin->setMaximum( 31 );

    irqLayout->addWidget( irqLevelLabel, 0, 0, Qt::AlignRight );
    irqLayout->addWidget( fIRQLevelCombo, 0, 1 );
    irqLayout->addWidget( irqVectorLabel, 0, 2, Qt::AlignRight );
    irqLayout->addWidget( fIRQVectorSpin, 0, 3 );
    irqLayout->addWidget( irqEventsLabel, 0, 4, Qt::AlignRight );
    irqLayout->addWidget( fIRQEventsSpin, 0, 5 );

    irqGroup->setLayout( irqLayout );

    vLayout->addWidget( suppGroup );
    vLayout->addWidget( irqGroup );

    tab->setLayout( vLayout );
}

void V1785NWindow::CreateChannelsTab()
{
    QWidget* tab = new QWidget();

    QVBoxLayout* vLayout = new QVBoxLayout();

    fChannelTab = new QTabWidget();
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        QWidget* tab = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();

        QGroupBox* lowGroup = new QGroupBox( "Low range" );
        QGridLayout* lowLayout = new QGridLayout();
        fChEnLowCheck[ch] = new QCheckBox( "Enable" );
        QLabel* lowLabel = new QLabel( "Threshold:" );
        fChThrLowSpin[ch] = new QSpinBox();
            fChThrLowSpin[ch]->setMinimum( 0 );
            fChThrLowSpin[ch]->setMaximum( 255 );
        lowLayout->addWidget( fChEnLowCheck[ch], 0, 0 );
        lowLayout->addWidget( lowLabel, 0, 1, Qt::AlignRight );
        lowLayout->addWidget( fChThrLowSpin[ch], 0, 2 );
        lowGroup->setLayout( lowLayout );

        QGroupBox* highGroup = new QGroupBox( "High range" );
        QGridLayout* highLayout = new QGridLayout();
        fChEnHighCheck[ch] = new QCheckBox( "Enable" );
        QLabel* highLabel = new QLabel( "Threshold:" );
        fChThrHighSpin[ch] = new QSpinBox();
            fChThrHighSpin[ch]->setMinimum( 0 );
            fChThrHighSpin[ch]->setMaximum( 255 );
        highLayout->addWidget( fChEnHighCheck[ch], 0, 0 );
        highLayout->addWidget( highLabel, 0, 1, Qt::AlignRight );
        highLayout->addWidget( fChThrHighSpin[ch], 0, 2 );
        highGroup->setLayout( highLayout );

        vLayout->addWidget( lowGroup ); 
        vLayout->addWidget( highGroup ); 

        tab->setLayout( vLayout );

        fChannelTab->addTab( tab, QString( "Ch %1" ).arg( ch ) );
    }

    vLayout->addWidget( fChannelTab );
    tab->setLayout( vLayout );

    fMainTab->addTab( tab, "Channels" );
}

void V1785NWindow::CreatePlotTab()
{
    QWidget* tab = new QWidget();

    QVBoxLayout* vLayout = new QVBoxLayout();

    QGroupBox* histGroup = new QGroupBox( "Histogram" );
    QGridLayout* histLayout = new QGridLayout();
    
    QLabel* binLabel = new QLabel( "# of bins:" );
    fBinSpin = new QSpinBox();
        fBinSpin->setMinimum( 1 );
        fBinSpin->setMaximum( 100000 );

    QLabel* startLabel = new QLabel( "X<sub>1</sub>:" );
    fStartHistSpin = new QSpinBox();
        fStartHistSpin->setMinimum( -1000000000 );
        fStartHistSpin->setMaximum( 1000000000 );
    QLabel* stopLabel = new QLabel( "X<sub>2</sub>:" );
    fStopHistSpin = new QSpinBox();
        fStopHistSpin->setMinimum( -1000000000 );
        fStopHistSpin->setMaximum( 1000000000 );

    histLayout->addWidget( binLabel, 0, 0, Qt::AlignRight );
    histLayout->addWidget( fBinSpin, 0, 1 );
    histLayout->addWidget( startLabel, 0, 2, Qt::AlignRight );
    histLayout->addWidget( fStartHistSpin, 0, 3 );
    histLayout->addWidget( stopLabel, 0, 4, Qt::AlignRight );
    histLayout->addWidget( fStopHistSpin, 0, 5 );

    histGroup->setLayout( histLayout );
    histGroup->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    vLayout->addWidget( histGroup );

    tab->setLayout( vLayout );
    fMainTab->addTab( tab, "Plots" );
}

void V1785NWindow::WriteConfig()
{
    try
    {
        UConfig<V1785N> cfg = qvariant_cast< UConfig<V1785N> >( this->CollectConfig() );
        dynamic_cast<V1785N*>(fDevice)->WriteConfig( cfg );
        emit Programmed( true );
    }
    catch( const VException& e )
    {
        emit Programmed( false );
        emit Error( e );
    }
}

void V1785NWindow::ReadConfig()
{
    UConfig<V1785N> cfg;

    try
    {
        dynamic_cast<V1785N*>(fDevice)->ReadConfig( cfg );
        QVariant qv; qv.setValue( cfg );
        SpreadConfig( qv );
    }
    catch( const VException& e )
    {
        emit Error( e );
    }
}

QVariant V1785NWindow::CollectConfig()
{
    UConfig<V1785N> cfg;
    UConfig<V1785N>::Threshold thr;
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        thr.LOW.VALUE = fChThrLowSpin[ch]->value();
        thr.LOW.KILL = not fChEnLowCheck[ch]->isChecked();
        thr.HIGH.VALUE = fChThrHighSpin[ch]->value();
        thr.HIGH.KILL = not fChEnHighCheck[ch]->isChecked();
        cfg.THRESHOLDS.at( ch ) = thr;
    }

    cfg.ZERO_SUPP = fZeroSuppCheck->isChecked();
    cfg.OVER_SUPP = fOverSuppCheck->isChecked();
    cfg.ZERO_SUPP_TYPE = fZeroSuppTypeCombo->currentData().value<V1785N::ZeroSupp_t>();

    cfg.IRQ_LEVEL = fIRQLevelCombo->currentData().value<uint8_t>();
    cfg.IRQ_VECTOR = fIRQVectorSpin->value();
    cfg.IRQ_EVENTS = fIRQEventsSpin->value();

    QVariant qv; qv.setValue( cfg );

    return qv;
}

void V1785NWindow::SpreadConfig( const QVariant& qConfig )
{
    UConfig<V1785N> cfg = qvariant_cast< UConfig<V1785N> >( qConfig );

    UConfig<V1785N>::Threshold thr;
    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        thr = cfg.THRESHOLDS.at( ch );
        fChThrLowSpin[ch]->setValue( thr.LOW.VALUE );
        fChEnLowCheck[ch]->setChecked( not thr.LOW.KILL );
        fChThrHighSpin[ch]->setValue( thr.HIGH.VALUE );
        fChEnHighCheck[ch]->setChecked( not thr.HIGH.KILL );
    }

    fZeroSuppCheck->setChecked( cfg.ZERO_SUPP );
    fOverSuppCheck->setChecked( cfg.OVER_SUPP );
    ChangeCombo( fZeroSuppTypeCombo, cfg.ZERO_SUPP_TYPE );

    ChangeCombo( fIRQLevelCombo, cfg.IRQ_LEVEL );
    fIRQVectorSpin->setValue( cfg.IRQ_VECTOR );
    fIRQEventsSpin->setValue( cfg.IRQ_EVENTS );
}
