#include "QLedIndicator.h"
#include "Style.h"

#include <QHBoxLayout>
#include <QDebug>

static const int SIZE = 20;

QLedIndicator::QLedIndicator( int size, QWidget *parent ) :
    QLabel(parent)
{
    setState( false );
    setFixedSize( size, size );
}

void QLedIndicator::setState( bool state )
{
    setStyleSheet( state ? style::led::on : style::led::off );
}


QLedIndicatorWithLabel::QLedIndicatorWithLabel( int size, QString text, bool leftAlign, QWidget *parent ) :
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
