#include "QLedIndicator.h"
#include "Style.h"

#include <QHBoxLayout>
#include <QDebug>

static const int SIZE = 20;

QLedIndicator::QLedIndicator( QWidget *parent ) :
    QLabel(parent)
{
    setState( false );
    setFixedSize( QLedIndicator::size, QLedIndicator::size );
}

QLedIndicator::~QLedIndicator()
{
}

void QLedIndicator::setState( bool state )
{
    SColor v( SColor_t::VIOLET );
    SColor p( SColor_t::PINK );
    QString on =       QString( "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2 ); border-radius: 4px" ).arg( p.light, p.dark );
    QString off =      QString( "color: white;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 %1, stop:1 %2); border-radius: 4px" ).arg( style::greyLight, style::grey );
    
    setStyleSheet( state ? on : off );
}


QLedIndicatorWithLabel::QLedIndicatorWithLabel( QString text, bool leftAlign, QWidget *parent ) :
    QWidget( parent )
{
    QGridLayout *gLayout = new QGridLayout();

    fLED = new QLedIndicator();
    QLabel *label = new QLabel( text );
        label->setStyleSheet( QString( "QLabel {font-size:%1px}" ).arg( QLedIndicator::size ) );

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
