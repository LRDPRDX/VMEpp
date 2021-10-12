{
    gSystem->Load( "/usr/lib/libCAENVME.so" );
    gSystem->Load( "/usr/lib/libVMEplus.so" );
    gSystem->Load( "/usr/lib/libVMEplusM.so" );
    gSystem->AddIncludePath( "-I/home/csi/Local/Sikach/projects/VMEpp/inc");
    gROOT->LoadMacro( "Histo.cpp+" );
}
