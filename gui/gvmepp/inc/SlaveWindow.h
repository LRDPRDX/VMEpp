#pragma once

#include "DeviceWindow.h"

class V2718Window;

class SlaveWindow : public DeviceWindow
{
    Q_OBJECT

    private :
        V2718Window     *fParent;

        void CreateMenu();

    protected :
        VSlave          *fDevice;

    protected :
        void DoOnError( const VException& error ) override;

    public slots :
        void OnControllerDisconnect( bool );
        void Connect() override;
        void Disconnect() override;

    public :
        SlaveWindow( V2718Window* parent );
        ~SlaveWindow();
};
