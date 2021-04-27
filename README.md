# VME++
A C++ library (based on [CAENVMElib](https://www.caen.it/products/caenvmelib-library/))
which provides a UI for CAEN VME modules

# Prerequisites
 - GCC compiler with c++11 support
 - [CAENVMElib](https://www.caen.it/products/caenvmelib-library/)

# Installation
```
cd <root_dir>
make
sudo make install
```

# Run the first example
```
cd <root_dir>/template
make
./Main
```

# TODO
- [ ] Add the *Advanced* OPCODEs' functions
- [x] Add the *Miscellaneous* OPCODEs' functions
- [ ] Consider to add a delay in case of firmware revision <= 0.3 (EEPROM)

# Disclaimer
It is not stable. Do not use it for now.
