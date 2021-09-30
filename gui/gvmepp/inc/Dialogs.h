#pragma once

#include <QDialog>
#include <QMessageBox>

#include "V2718Window.h"

class SButton;
class QComboBox;
class QSpinBox;
class QTextEdit;

namespace vmeplus
{
    class VException;
}


class Connection : public QDialog
{
    Q_OBJECT

    private :
        V2718Window  *fParent;

        SButton     *fConnectButton, *fCancelButton;
        QComboBox   *fTypeCombo;
        QSpinBox    *fLinkSpin, *fConetSpin;

    public :
        Connection( V2718Window *parent );

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
        V2718Window  *fParent;

        QComboBox   *fDeviceCombo;
        QSpinBox    *fAddressSpin;
        SButton     *fAddButton, *fCancelButton;
        QTextEdit   *fInfoText;

    public :
        DeviceDialog( V2718Window *parent );

    private slots :
        void Cancel();
        void Add();
};

class ErrorMessageBox : public QMessageBox
{
    Q_OBJECT

    public :
        ErrorMessageBox( const vmeplus::VException& e, QWidget *parent = nullptr );
};
