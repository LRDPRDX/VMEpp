#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QString>

class QLedIndicator : public QLabel
{
    Q_OBJECT
        
    public:
        QLedIndicator( int size = 14, QWidget *parent = 0 );
        virtual ~QLedIndicator();
        
    public slots:
        void setState( bool state );
};

class QLedIndicatorWithLabel : public QWidget
{
    private :
        QLedIndicator *fLED;

    public :
        QLedIndicatorWithLabel( QString text = "", bool leftAlign = true, int size = 14, QWidget *parent = nullptr );
        virtual ~QLedIndicatorWithLabel();

        void SetChecked( bool status ) { fLED->setState( status ); }
};

#endif // QLEDLABEL_H
