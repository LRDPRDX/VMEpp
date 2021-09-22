#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QVariant>
#include <QMessageBox>

#include "UConfigurable.h"
#include "modules/V895.h"
#include "modules/V6533N.h"

class SButton;
class SFrame;
class QAction;
class QMenu;

class V2718Window;


using namespace vmeplus;

Q_DECLARE_METATYPE(UConfig<V895>)
Q_DECLARE_METATYPE(UConfig<V6533N>)

class DeviceWindow : public QMainWindow
{
    Q_OBJECT

    private :
        V2718Window *fParent;

    protected :
        VSlave *fDevice;

    protected :
        void closeEvent( QCloseEvent *event ) override;
        void CreateFileMenu();

    protected :
        QMenu           *fViewMenu, *fConfigMenu;
        QAction         *fConnectAction, *fDisconnectAction, *fExitAction;
        QAction         *fSaveConfigAction, *fLoadConfigAction;
        QAction         *fViewStatusBarAction;

        SButton         *fProgramButton, *fReadButton;
        SFrame          *fBottomFrame;

    public slots :
        void Connect();
        void Disconnect();
        void OnControllerDisconnect( bool );
        void ToggleStatusBar();
        virtual void Program() = 0;
        virtual void ReadConfig() = 0;
        virtual QVariant CollectConfig() = 0;
        virtual void SpreadConfig( const QVariant& config ) = 0;

    public :
        template<typename T>
        void SaveConfig();

        template<typename T>
        void LoadConfig();


    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceWindow( V2718Window *parent );
        ~DeviceWindow() override;
};

template <typename T>
void DeviceWindow::SaveConfig()
{
    QString fileName = QFileDialog::getSaveFileName( this, "Save config file", "./", "Text files (*.json)" );

    if( fileName.isEmpty() )
    {
        return;
    }
    else
    {
        UConfig<T> cfg = qvariant_cast< UConfig<T> >( this->CollectConfig() );

        try
        {
            WriteConfigToFile(  cfg, fileName.toStdString() );
        }
        catch( const VException& e )
        {
            QMessageBox::warning( this,
                                  e.what(),
                                  e.GetInfo().c_str(),
                                  QMessageBox::Ok );
        }
    }
}

template <typename T>
void DeviceWindow::LoadConfig()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Load config file", "./", "Text files (*.json)" );

    if( fileName.isEmpty() )
    {
        return;
    }
    else
    {
        try
        {
            UConfig<T> cfg;
            ReadConfigFromFile( cfg, fileName.toStdString() );
            QVariant qv;
            qv.setValue( cfg );
            this->SpreadConfig( qv );
        }
        catch( const VException& e )
        {
            QMessageBox::warning( this,
                                  e.what(),
                                  e.GetInfo().c_str(),
                                  QMessageBox::Ok );
        }
    }
}
