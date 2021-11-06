#include "Dialogs.h"
#include "Style.h"

#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "VException.h"
#include "V895Window.h"
#include "V6533NWindow.h"
#include "V1785NWindow.h"

using namespace vmepp;

Connection::Connection( V2718Window *parent ) :
    QDialog( parent ),

    fParent( parent )
{
    setModal( true );
    setAttribute( Qt::WA_DeleteOnClose, true );
    setWindowTitle( "Connection" );

    // Upper layout (connection settings)
    auto *typeLabel = new QLabel( "Type:" );
    QStringList types = { "V2718", "V1718" };

    fTypeCombo = new QComboBox();
        fTypeCombo->addItems( types );

    auto *linkLabel = new QLabel( "Link:" );
    fLinkSpin = new QSpinBox();
        fLinkSpin->setRange( 0, 10 );

    auto *conetLabel = new QLabel( "Conet:" );
    fConetSpin = new QSpinBox();
        fConetSpin->setRange( 0, 10 );

    auto *settingsLayout = new QHBoxLayout();
        settingsLayout->addWidget( typeLabel );
        settingsLayout->addWidget( fTypeCombo );
        settingsLayout->addWidget( linkLabel );
        settingsLayout->addWidget( fLinkSpin );
        settingsLayout->addWidget( conetLabel );
        settingsLayout->addWidget( fConetSpin );

    // Lower layout (buttons)
    fConnectButton = new QPushButton( "Connect" );
    connect( fConnectButton, &QPushButton::clicked, this, &Connection::Connect );

    fCancelButton = new QPushButton( "Cancel" );
    connect( fCancelButton, &QPushButton::clicked, this, &Connection::Cancel );

    auto *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget( fConnectButton );
        buttonsLayout->addWidget( fCancelButton );

    auto *extLayout = new QVBoxLayout();
        extLayout->addLayout( settingsLayout );
        extLayout->addLayout( buttonsLayout );

    setLayout( extLayout );
}

void Connection::Connect()
{
    try
    {
        fParent->Connect( fLinkSpin->value(), fConetSpin->value() );
        this->close();
    }
    catch( const VException& e )
    {
        ErrorMessageBox* msg = new ErrorMessageBox( e, this );
        msg->show();
    }
}

void Connection::Cancel()
{
    this->close();
}

/***************************
 ******* DEVICE DIALOG *****
 ***************************/
DeviceDialog::DeviceDialog( V2718Window *parent ) :
    QDialog( parent ),

    fParent( parent )
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    setModal( true );
    setWindowTitle( "Device connection" );

    // Upper layout (connection settings)
    auto *devLabel = new QLabel( "Device:" );

    fDeviceCombo = new QComboBox();
        fDeviceCombo->addItem( "V895", Device::V895 );
        fDeviceCombo->addItem( "V6533N", Device::V6533N );
        fDeviceCombo->addItem( "V1785N", Device::V1785N );

    auto *addressLabel = new QLabel( "Address [31:16]:" );
    fAddressSpin = new QSpinBox();
        fAddressSpin->setPrefix( "0x" );
        fAddressSpin->setSuffix( " 0000" );
        fAddressSpin->setDisplayIntegerBase( 16 );
        fAddressSpin->setRange( 0, 0xffff );
        QFont font( "monospace" );
        fAddressSpin->setFont( font );

    QHBoxLayout *settingsLayout = new QHBoxLayout();
        settingsLayout->addWidget( devLabel, Qt::AlignRight );
        settingsLayout->addWidget( fDeviceCombo );
        settingsLayout->addWidget( addressLabel, Qt::AlignRight );
        settingsLayout->addWidget( fAddressSpin );
        settingsLayout->setStretchFactor( fAddressSpin, 1  );

    // Lower layout (buttons)
    fAddButton = new QPushButton( "Add" );
        ColorButton( fAddButton, style::yellow );
        connect( fAddButton, &QPushButton::clicked, this, &DeviceDialog::Add );

    fCancelButton = new QPushButton( "Cancel" );
        ColorButton( fCancelButton, style::pink );
        connect( fCancelButton, &QPushButton::clicked, this, &DeviceDialog::Cancel );

    auto *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget( fAddButton );
        buttonsLayout->addWidget( fCancelButton );

    auto *extLayout = new QVBoxLayout();
        extLayout->addLayout( settingsLayout );
        extLayout->addLayout( buttonsLayout );

    setLayout( extLayout );
}

void DeviceDialog::Add()
{
    uint32_t address = fAddressSpin->value() << 16U;
    DeviceWindow *dw = nullptr;
    switch( fDeviceCombo->currentData().toInt() )
    {
        case( Device::V895 ) :
            dw = new V895Window( address, fParent );
            break;
        case( Device::V6533N ) :
            dw = new V6533NWindow( address, fParent );
            break;
        case( Device::V1785N ) :
            dw = new V1785NWindow( address, fParent );
            break;
    }
    if( dw ) { dw->show(); }
    else
    {
        qInfo() << "ERROR :: DeviceDialog::Add() :: The pointer is nullptr";
    }
    this->close();
}

void DeviceDialog::Cancel()
{
    this->close();
}

/*****************************/
/****** ErrorMessageBox ******/
/*****************************/
ErrorMessageBox::ErrorMessageBox( const VException& e, QWidget *parent ) :
    QMessageBox( parent )
{
        setWindowTitle( "Error" );
        setIcon( QMessageBox::Critical );
        setText( QString::fromStdString( e.GetErrorMessage() ) );
        setDetailedText( QString( "%1\n%2").arg( QString::fromStdString( e.GetInfo() ),
                                                 QString::fromStdString( e.GetHint() ) ) );
        setStandardButtons( QMessageBox::Ok );

        setModal( true );
        setAttribute( Qt::WA_DeleteOnClose, true );
}
