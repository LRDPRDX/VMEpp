{
    gSystem->Load( "/usr/lib/libCAENVME.so" );
    gSystem->Load( "/usr/lib/libVMEplus.so" );
    gSystem->Load( "/usr/lib/libVMEplusM.so" );
    gSystem->AddIncludePath( "-I/home/csi/Tools/CAEN/VME/inc");
    gROOT->LoadMacro( "Histo.cpp+" );
    //gROOT->LoadMacro( "../src/ROOT/Drawer/Drawer.cpp+" );
}
