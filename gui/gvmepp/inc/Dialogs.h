#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>

#include "V2718Window.h"

class QPushButton;
class QComboBox;
class QSpinBox;
class QTextEdit;

namespace vmepp
{
    class VException;
}


class Connection : public QDialog
{
    Q_OBJECT

    private :
        V2718Window  *fParent;

        QPushButton     *fConnectButton, *fCancelButton;
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
        enum Device { V895, V6533N, V1785N };

    private :
        V2718Window  *fParent;

        QComboBox   *fDeviceCombo;
        QSpinBox    *fAddressSpin;
        QPushButton     *fAddButton, *fCancelButton;
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
        ErrorMessageBox( const vmepp::VException& e, QWidget *parent = nullptr );
};

class OkCancelDialog : public QDialog
{
    Q_OBJECT

    protected :
        QPushButton *fOkButton, *fCancelButton;

    public :
        OkCancelDialog( const QString& message, QWidget* parent = nullptr );
        ~OkCancelDialog() override = default;
};