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
