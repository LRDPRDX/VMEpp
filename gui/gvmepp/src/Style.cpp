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

//*********************
//****** SBUTTON ******
//*********************
SButton::SButton( const QString& text, SColor_t color, QWidget *parent ) :
    QPushButton( text, parent )
{
    SColor c( color );
    QString s = QString( "QPushButton {background-color:%1; color:%4; border-radius : 4px; border: 1px solid #000000;"
                         "padding: 3px}"
                         "QPushButton:disabled {background-color:%2; color:%5;}"
                         "QPushButton:pressed {background-color:%3; color:%6}" ).arg( c.normal,
                                                                                      c.light,
                                                                                      c.dark,
                                                                                      style::white,
                                                                                      style::white,
                                                                                      style::white );
    setStyleSheet( s );
}

//***************************
//****** SLEDINDICATOR ******
//***************************
SLedIndicator::SLedIndicator( int size, QWidget *parent ) :
    QLabel(parent),
    fColor( SColor_t::RED )
{
    SetState( false );
    setFixedSize( size, size );
}

void SLedIndicator::SetState( bool state )
{
    SColor c( fColor );
    QString on =       QString( "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2 ); border-radius: 5px; border: 1px solid #000000" ).arg( c.light, c.dark );
    QString off =      QString( "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2); border-radius: 5px; border: 1px solid #000000" ).arg( style::greyLight, style::grey );
    
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
