#include "Controller.h"
#include "V6533NWindow.h"
#include "modules/V6533N.h"

#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDebug>
#include <QStatusBar>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTabWidget>

#include "VException.h"
#include "qnamespace.h"

V6533NWindow::V6533NWindow( uint32_t address, Controller *parent ) :
    DeviceWindow( parent )
{
    fDevice = new vmeplus::V6533N( address );

    setWindowTitle( "V6533N" );
    CreateActions();
    CreateCentralWidget();

    emit Connected( false );

    statusBar()->showMessage( "Ready..." );
}

V6533NWindow::~V6533NWindow()
{
}

void V6533NWindow::CreateActions()
{
}

void V6533NWindow::CreateCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout();

    for( uint8_t ch = 0; ch < N_CH; ++ch )
    {
        QGroupBox *chGroup = new QGroupBox( QString::number( ch ) );
        QGridLayout *gridLayout = new QGridLayout();

        QLabel *voltLabel = new QLabel( "U (V):" );
        fVoltSpin[ch] = new QSpinBox();
            fVoltSpin[ch]->setRange( 0, 6000 );

        QLabel *curLabel = new QLabel( "I (uA):" );
        fCurSpin[ch] = new QSpinBox();
            fCurSpin[ch]->setRange( 0, 3000 );

        QLabel *upLabel = new QLabel( "Up (V/s):" );
        fUpSpin[ch] = new QSpinBox();
            fUpSpin[ch]->setRange( 0, 400 );

        QLabel *downLabel = new QLabel( "Up (V/s):" );
        fDownSpin[ch] = new QSpinBox();
            fDownSpin[ch]->setRange( 0, 400 );

        QLabel *swMaxLabel = new QLabel( "Max (V):" );
        fSWMaxSpin[ch] = new QSpinBox();
            fSWMaxSpin[ch]->setRange( 0, 6000 );

        QLabel *offLabel = new QLabel( "OFF:" );
        fOffCombo[ch] = new QComboBox();
            fOffCombo[ch]->addItem( "OFF", 0 );
            fOffCombo[ch]->addItem( "KILL", 1 );

        gridLayout->addWidget( voltLabel, 0, 0, Qt::AlignRight ); 
        gridLayout->addWidget( fVoltSpin[ch], 0, 1 );
        gridLayout->addWidget( curLabel, 0, 2, Qt::AlignRight ); 
        gridLayout->addWidget( fCurSpin[ch], 0, 3 );
        gridLayout->addWidget( swMaxLabel, 0, 4, Qt::AlignRight ); 
        gridLayout->addWidget( fSWMaxSpin[ch], 0, 5 );
        gridLayout->addWidget( upLabel, 1, 0, Qt::AlignRight ); 
        gridLayout->addWidget( fUpSpin[ch], 1, 1 );
        gridLayout->addWidget( downLabel, 1, 2, Qt::AlignRight ); 
        gridLayout->addWidget( fDownSpin[ch], 1, 3 );
        gridLayout->addWidget( offLabel, 1, 4, Qt::AlignRight ); 
        gridLayout->addWidget( fOffCombo[ch], 1, 5 );

        chGroup->setLayout( gridLayout );
        vLayout->addWidget( chGroup );
    }

    vLayout->addWidget( fProgramButton );

    centralWidget->setLayout( vLayout );

    setCentralWidget( centralWidget );
}

void V6533NWindow::Program()
{
    qInfo() << "Programming V895...\n";
    emit Programmed( true );
}
