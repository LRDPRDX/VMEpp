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

    public :
        template<typename T>
        void SaveConfig();


    signals :
        void Connected( bool );
        void Programmed( bool );

    public :
        DeviceWindow( V2718Window *parent );
        virtual ~DeviceWindow();
};

template <typename T>
void DeviceWindow::SaveConfig()
{
    QString fileName = QFileDialog::getSaveFileName( this, "Save Config file", "./", "Text files (*.json)" );

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
                                  "Saving config failed!",
                                  "Couldn't write to the file!",
                                  QMessageBox::Ok );
        }
    }
}
