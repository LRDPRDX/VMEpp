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

void DeviceV895::OnControllerDisconnect( bool status )
{
    if( not status )
    {
        Disconnect();
    }
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
