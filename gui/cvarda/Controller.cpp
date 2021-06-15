#include "Controller.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>

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
    qInfo() << "Connecting...";
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
