#pragma once

#include <QDialog>

#include "Controller.h"

class QPushButton;
class QComboBox;
class QSpinBox;
class QTextEdit;

class Connection : public QDialog
{
    Q_OBJECT

    private :
        Controller  *fParent;

        QPushButton *fConnectButton, *fCancelButton;
        QComboBox   *fTypeCombo;
        QSpinBox    *fLinkSpin, *fConetSpin;

    public :
        Connection( Controller *parent );

    private slots :
        void Connect();
        void Cancel();
};

class DeviceDialog : public QDialog
{
    Q_OBJECT

    public :
        enum Device { V895, V6533N };

    private :
        Controller  *fParent;

        QComboBox   *fDeviceCombo;
        QSpinBox    *fAddressSpin;
        QPushButton *fAddButton, *fCancelButton;
        QTextEdit   *fInfoText;

    public :
        DeviceDialog( Controller *parent );

    private slots :
        void Cancel();
        void Add();
};
