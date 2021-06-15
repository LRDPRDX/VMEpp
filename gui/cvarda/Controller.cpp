#include "Controller.h"

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

Controller::Controller( QWidget *parent ) :
    QMainWindow( parent )
{
    auto *connectAction = new QAction( "&Connect", this );
    auto *disconnectAction = new QAction( "&Disconnect", this );
    auto *exitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( connectAction );
        fileMenu->addAction( disconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( exitAction );

    connect( connectAction, &QAction::triggered, this, &Controller::Connect ); 
    connect( disconnectAction, &QAction::triggered, this, &Controller::Disconnect ); 
    connect( exitAction, &QAction::triggered, qApp, &QApplication::quit );

    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    QMenu *viewMenu = menuBar()->addMenu( "&View" );
        viewMenu->addAction( fViewStatusBarAction );

    connect( fViewStatusBarAction, &QAction::triggered, this, &Controller::ToggleStatusBar );

    statusBar()->showMessage( "Ready..." );
}

Controller::~Controller() {}

void Controller::Connect()
{
    Connection *con = new Connection( this ); 
    con->resize( 300, 150 );
    con->setWindowTitle( "Connection dialog" );
    con->show();
}

void Controller::Disconnect()
{
    qInfo() << "Disconnecting...";
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


Connection::Connection( QWidget *parent ) :
    QDialog( parent )
{
    auto *typeLabel = new QLabel( "Type:", this );
    QStringList types = { "V2718", "V1718" };

    fTypeCombo = new QComboBox();
        fTypeCombo->addItems( types );

    auto *settingsLayout = new QHBoxLayout();
        settingsLayout->addWidget( typeLabel );
        settingsLayout->addWidget( fTypeCombo );

    fConnectButton = new QPushButton( "Connect", this );
    fCancelButton = new QPushButton( "Cancel", this );

    auto *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget( fConnectButton );
        buttonsLayout->addWidget( fCancelButton );

    auto *extLayout = new QVBoxLayout( this );
        extLayout->addLayout( settingsLayout );
        extLayout->addLayout( buttonsLayout ); 

    setLayout( extLayout );
}

void Connection::Connect()
{
    qInfo() << "Connecting...";
};

void Connection::Cancel()
{
    qInfo() << "Connection canceled...";
}
