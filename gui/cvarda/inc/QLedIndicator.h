#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QString>

class QLedIndicator : public QLabel
{
    Q_OBJECT

    public:
        explicit QLedIndicator( int size, QWidget *parent = 0 );
        
    public slots:
        void setState( bool state );
};

class QLedIndicatorWithLabel : public QWidget
{
    private :
        QLedIndicator *fLED;

    public :
        QLedIndicatorWithLabel( int size = 14, QString text = "", bool leftAlign = true, QWidget *parent = nullptr );
        void SetChecked( bool status ) { fLED->setState( status ); }
};

#endif // QLEDLABEL_H
