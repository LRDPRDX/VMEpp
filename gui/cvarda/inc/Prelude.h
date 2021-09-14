#pragma once

#include "CAENVMElib.h"
#include "modules/V2718.h"
#include "modules/V6533N.h"

Q_DECLARE_METATYPE(V6533N::IMonRange_t)   

Q_DECLARE_METATYPE(CVIOPolarity)   
Q_DECLARE_METATYPE(CVLEDPolarity)   
Q_DECLARE_METATYPE(CVIOSources)   

#include <QComboBox>
#include <QVariant>

template<typename Data_t>
void ChangeCombo( QComboBox* c, Data_t data )
{
    int index = c->findData( QVariant::fromValue( data ) );
    if( index >= 0 )
    {
        c->setCurrentIndex( index );
    }
}
