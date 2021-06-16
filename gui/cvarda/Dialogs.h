#pragma once

#include <QDialog>

#include "Controller.h"

class QPushButton;
class QComboBox;
class QSpinBox;

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
