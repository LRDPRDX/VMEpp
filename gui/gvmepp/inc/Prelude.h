#pragma once

#include "CAENVMElib.h"
#include "modules/V2718.h"
#include "modules/V895.h"
#include "modules/V6533N.h"

using namespace vmeplus;

/* CAENVMElib types */
Q_DECLARE_METATYPE(CVIOPolarity)   
Q_DECLARE_METATYPE(CVLEDPolarity)   
Q_DECLARE_METATYPE(CVIOSources)   

/* VMEpp types */
Q_DECLARE_METATYPE(UConfig<V2718>)
Q_DECLARE_METATYPE(UConfig<V895>)
Q_DECLARE_METATYPE(UConfig<V6533N>)
Q_DECLARE_METATYPE(V6533N::IMonRange_t)   


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
