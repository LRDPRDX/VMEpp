#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QString>

#include "Style.h"

class QLedIndicator : public QLabel
{
    Q_OBJECT

    protected :
        SColor_t fColor;
        
    public:
        QLedIndicator( int size = 14, QWidget *parent = 0 );
        virtual ~QLedIndicator();
        
    public slots:
        void SetState( bool state );
        void SetColor( SColor_t color ) { fColor = color; }
};

class QLedIndicatorWithLabel : public QWidget
{
    private :
        QLedIndicator *fLED;

    public :
        QLedIndicatorWithLabel( QString text = "", bool leftAlign = true, int size = 14, QWidget *parent = nullptr );
        virtual ~QLedIndicatorWithLabel();

        void SetChecked( bool status ) { fLED->SetState( status ); }
        void SetColor( SColor_t color ) { fLED->SetColor( color ); }
};

#endif // QLEDLABEL_H
