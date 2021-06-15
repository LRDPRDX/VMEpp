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
#include <QMessageBox>
#include <QCloseEvent>

Controller::Controller( QWidget *parent ) :
    QMainWindow( parent )
{
    CreateActions();

    statusBar()->showMessage( "Ready..." );
}

Controller::~Controller() {}

void Controller::Connect()
{
    Connection *con = new Connection( this );
        con->setModal( true );
        con->resize( 300, 150 );
        con->setWindowTitle( "Connection dialog" );
    con->show();
}

/****** Constructive methods ******/
void Controller::CreateActions()
{
    fConnectAction = new QAction( "&Connect", this );
    fDisconnectAction = new QAction( "&Disconnect", this );
    fExitAction = new QAction( "&Exit", this );

    QMenu *fileMenu = menuBar()->addMenu( "&File" );
        fileMenu->addAction( fConnectAction );
        fileMenu->addAction( fDisconnectAction );
        fileMenu->addSeparator();
        fileMenu->addAction( fExitAction );

    connect( fConnectAction, &QAction::triggered, this, &Controller::Connect );
    connect( fDisconnectAction, &QAction::triggered, this, &Controller::Disconnect );
    connect( fExitAction, &QAction::triggered, this, &Controller::close );

    fViewStatusBarAction = new QAction( "&View statusbar" );
        fViewStatusBarAction->setCheckable( true );
        fViewStatusBarAction->setChecked( true );

    QMenu *viewMenu = menuBar()->addMenu( "&View" );
        viewMenu->addAction( fViewStatusBarAction );

    connect( fViewStatusBarAction, &QAction::triggered, this, &Controller::ToggleStatusBar );
}

void Controller::Disconnect()
{
    qInfo() << "Disconnecting...";
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
