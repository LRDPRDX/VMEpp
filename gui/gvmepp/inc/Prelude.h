#pragma once

#include "CAENVMElib.h"
#include "modules/V2718.h"
#include "modules/V895.h"
#include "modules/V6533N.h"
#include "modules/V1785N.h"

using namespace vmepp;

/* VMEpp types */
Q_DECLARE_METATYPE(UConfig<V2718>)
Q_DECLARE_METATYPE(UConfig<V895>)
Q_DECLARE_METATYPE(UConfig<V6533N>)
Q_DECLARE_METATYPE(V6533N::IMonRange_t)

Q_DECLARE_METATYPE(V2718::Polarity_t)
Q_DECLARE_METATYPE(V2718::LEDPolarity_t)
Q_DECLARE_METATYPE(V2718::Src_t)

Q_DECLARE_METATYPE(V1785N::ZeroSupp_t)

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
