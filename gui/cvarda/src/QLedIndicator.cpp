#include "QLedIndicator.h"
#include "Style.h"

#include <QHBoxLayout>
#include <QDebug>

QLedIndicator::QLedIndicator( int size, QWidget *parent ) :
    QLabel(parent),
    fColor( SColor_t::RED )
{
    SetState( false );
    setFixedSize( size, size );
}

QLedIndicator::~QLedIndicator()
{
}

void QLedIndicator::SetState( bool state )
{
    SColor c( fColor );
    QString on =       QString( "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2 ); border-radius: 5px; border: 1px solid #000000" ).arg( c.light, c.dark );
    QString off =      QString( "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2); border-radius: 5px; border: 1px solid #000000" ).arg( style::greyLight, style::grey );
    
    setStyleSheet( state ? on : off );
}


QLedIndicatorWithLabel::QLedIndicatorWithLabel( QString text, bool leftAlign, int size, QWidget *parent ) :
    QWidget( parent )
{
    QGridLayout *gLayout = new QGridLayout();

    fLED = new QLedIndicator( size );
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

QLedIndicatorWithLabel::~QLedIndicatorWithLabel()
{
}
