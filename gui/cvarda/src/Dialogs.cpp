#include "Dialogs.h"

#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "VException.h"
#include "V895Window.h"

Connection::Connection( Controller *parent ) :
    QDialog( parent ),

    fParent( parent )
{
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
    catch( const vmeplus::VException& e )
    {
        qWarning() << "Connection failed : " << e.what();

        QMessageBox::warning( this,
                              tr( "Connection FAILED" ),
                              tr( e.what() ),
                              QMessageBox::Ok );
    }
}

void Connection::Cancel()
{
    this->close();
}


/****** DEVICE DIALOG ******/

DeviceDialog::DeviceDialog( Controller *parent ) :
    QDialog( parent ),

    fParent( parent )
{
    // Upper layout (connection settings)
    auto *devLabel = new QLabel( "Device:" );

    fDeviceCombo = new QComboBox();
        fDeviceCombo->addItem( "V895", Device::V895 );

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
    connect( fAddButton, &QPushButton::clicked, this, &DeviceDialog::Add );

    fCancelButton = new QPushButton( "Cancel" );
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
    switch( fDeviceCombo->currentData().toInt() )
    {
        case( Device::V895 ) :
            V895Window *d = new V895Window( address, fParent );
                d->show();
            this->close();
            break;
    }
}

void DeviceDialog::Cancel()
{
    this->close();
}
