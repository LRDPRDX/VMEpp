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
        fZeroSuppTypeCombo->addItem( "x16", qVariantFromValue( V1785N::ZeroSupp_t::Tx2) );
        fZeroSuppTypeCombo->addItem( "x2", qVariantFromValue( V1785N::ZeroSupp_t::Tx16 ) );

    suppLayout->addWidget( fZeroSuppCheck, 0, 0 );
    suppLayout->addWidget( fOverSuppCheck, 1, 0 );
    suppLayout->addWidget( zeroSuppTypeLabel, 0, 1, Qt::AlignRight );
    suppLayout->addWidget( fZeroSuppTypeCombo, 0, 2 );

    suppGroup->setLayout( suppLayout );

    QGroupBox *irqGroup = new QGroupBox( "Interrupt" );
    QGridLayout *irqLayout = new QGridLayout();

    QLabel* irqLevelLabel = new QLabel( "Level:" );
    fIRQLevelCombo = new QComboBox();
        fIRQLevelCombo->addItem( "IRQ1", cvIRQ1 );
        fIRQLevelCombo->addItem( "IRQ2", cvIRQ2 );
        fIRQLevelCombo->addItem( "IRQ3", cvIRQ3 );
        fIRQLevelCombo->addItem( "IRQ4", cvIRQ4 );
        fIRQLevelCombo->addItem( "IRQ5", cvIRQ5 );
        fIRQLevelCombo->addItem( "IRQ6", cvIRQ6 );
        fIRQLevelCombo->addItem( "IRQ7", cvIRQ7 );
        fIRQLevelCombo->addItem( "DISABLED", 0 );

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
        fChannelTab->addTab( tab, QString( "Ch %1" ).arg( ch ) );
    }

    vLayout->addWidget( fChannelTab );
    tab->setLayout( vLayout );

    fMainTab->addTab( tab, "Channels" );
}

void V1785NWindow::CreatePlotTab()
{
    QWidget* tab = new QWidget();
    fMainTab->addTab( tab, "Plots" );
}

void V1785NWindow::WriteConfig()
{
}

void V1785NWindow::ReadConfig()
{
}

QVariant V1785NWindow::CollectConfig()
{
    return QVariant{};
}

void V1785NWindow::SpreadConfig( const QVariant& qConfig )
{
}
