#include "Style.h"

#include <QHBoxLayout>


//********************
//****** SFRAME ******
//********************
SFrame::SFrame( SColor_t color, QWidget *parent ) :
    QFrame( parent )
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum );
    setObjectName( "SFrame" );
}

//***********************
//****** SGROUPBOX ******
//***********************
SGroupBox::SGroupBox( const QString& title, QWidget* parent ) :
    QGroupBox( title, parent )
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum );
}

//***************************
//****** SLEDINDICATOR ******
//***************************
SLedIndicator::SLedIndicator( int size, QWidget *parent ) :
    QLabel(parent),
    fColor( style::red ),
    fSize( size )
{
    setObjectName( "SLedIndicator" );
    SetState( false );
    setFixedSize( size, size );
}

void SLedIndicator::SetState( bool state )
{
    QString on =       QString( "#SLedIndicator {color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2 ); border-radius: %3px; border: 1px solid #000000; }" ).arg( fColor.lighter().name(), fColor.darker().name(), QString::number( fSize / 5 ) );
    QString off =      QString( "#SLedIndicator {color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2); border-radius: %3px; border: 1px solid #000000; }" ).arg( QColor( style::gray ).lighter().name(), style::gray, QString::number( fSize / 5 ) );

    setStyleSheet( state ? on : off );
}

//************************************
//****** SLEDINDICATORWITHLABEL ******
//************************************
SLedIndicatorWithLabel::SLedIndicatorWithLabel( QString text, bool leftAlign, int size, QWidget *parent ) :
    QWidget( parent )
{
    QGridLayout *gLayout = new QGridLayout();

    fLED = new SLedIndicator( size );
    QLabel *label = new QLabel( text );
        label->setStyleSheet( QString( "QLabel {font-size:%1px}" ).arg( size ) );

	if( leftAlign )
	{
        gLayout->addWidget( fLED, 0, 0, Qt::AlignLeft );
        gLayout->addWidget( label, 0, 1, Qt::AlignRight );
	}
	else
	{
        gLayout->addWidget( label, 0, 0, Qt::AlignLeft );
        gLayout->addWidget( fLED, 0, 1, Qt::AlignRight );
	}

    setLayout( gLayout );
}
