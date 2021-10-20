#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmepp;

typedef typename V1190B::EdgeDetect_t       Edge_t;
typedef typename V1190B::TrLeadLSB          EdgeRes;
typedef typename V1190B::PairRes            PairRes;
typedef typename V1190B::ResLeadEdgeTime    LeadRes;
typedef typename V1190B::ResPulseWidth      WidthRes;
typedef typename V1190B::DeadTime           DTime;

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        std::cout << "Test begins...\n";
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        // Detection Mode 
        Edge_t edge;

        tdc.WriteDetection( Edge_t::PAIR );
        edge = tdc.ReadDetection();
        assert( edge == Edge_t::PAIR );

        tdc.WriteDetection( Edge_t::TRAILING );
        edge = tdc.ReadDetection();
        assert( edge == Edge_t::TRAILING );

        tdc.WriteDetection( Edge_t::LEADING );
        edge = tdc.ReadDetection();
        assert( edge == Edge_t::LEADING );

        tdc.WriteDetection( Edge_t::TRAILINGLEADING );
        edge = tdc.ReadDetection();
        assert( edge == Edge_t::TRAILINGLEADING );

        // Dead Time
        DTime dTime;

        tdc.WriteDeadTime( DTime::ns5 );
        dTime = tdc.ReadDeadTime();
        assert( dTime == DTime::ns5 );

        tdc.WriteDeadTime( DTime::ns10 );
        dTime = tdc.ReadDeadTime();
        assert( dTime == DTime::ns10 );

        tdc.WriteDeadTime( DTime::ns30 );
        dTime = tdc.ReadDeadTime();
        assert( dTime == DTime::ns30 );

        tdc.WriteDeadTime( DTime::ns100 );
        dTime = tdc.ReadDeadTime();
        assert( dTime == DTime::ns100 );

        // Resolution
        // // PAIR
        PairRes pair;

        tdc.WriteDetection( Edge_t::PAIR );

        tdc.WritePairRes( PairRes( LeadRes::ps1600, WidthRes::ns25 ) );
        tdc.ReadPairRes( pair );

        assert( pair.edgeTime == LeadRes::ps1600 );
        assert( pair.width == WidthRes::ns25 );

        tdc.WritePairRes( PairRes( LeadRes::ps6250, WidthRes::INVALID_1 ) );
        tdc.ReadPairRes( pair );

        assert( pair.edgeTime == LeadRes::ps6250 );
        assert( pair.width == WidthRes::INVALID_1 );
        
        // // EDGE
        EdgeRes edgeRes; 

        tdc.WriteDetection( Edge_t::TRAILING );
        tdc.WriteEdgeRes( EdgeRes::ps100 );
        edgeRes = tdc.ReadEdgeRes();
        assert( edgeRes == EdgeRes::ps100 );

        tdc.WriteDetection( Edge_t::LEADING );
        tdc.WriteEdgeRes( EdgeRes::ps200 );
        edgeRes = tdc.ReadEdgeRes();
        assert( edgeRes == EdgeRes::ps200 );

        tdc.WriteDetection( Edge_t::TRAILINGLEADING );
        tdc.WriteEdgeRes( EdgeRes::ps800 );
        edgeRes = tdc.ReadEdgeRes();
        assert( edgeRes == EdgeRes::ps800 );

        tdc.WriteDetection( Edge_t::LEADING );
        tdc.WriteEdgeRes( EdgeRes::UNUSED );
        edgeRes = tdc.ReadEdgeRes();
        assert( edgeRes == EdgeRes::UNUSED );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
