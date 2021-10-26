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
}

void V1785NWindow::CreateChannelsTab()
{
    QWidget* tab = new QWidget();
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
