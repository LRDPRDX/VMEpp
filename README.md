# VME++

A C++ library (based on [CAENVMElib](https://www.caen.it/products/caenvmelib-library/))
which provides a higher-level UI for CAEN VME modules

## Prerequisites

 - GCC compiler with c++11 support (>= 4.8.5)
 - [CAENVMElib](https://www.caen.it/products/caenvmelib-library/) (>= 3.2.0)
 - [Cereal](https://uscilab.github.io/cereal/)

## Installation

```
git clone git@github.com:LRDPRDX/VMEpp.git
cd VMEpp 
make
sudo make install
```

## Supported modules

The following modules are represented in the library:

 - V2718 (VME controller)
 - V1190B (TDC)
 - V895 (Discriminator)
 - V6533N (High Voltage Source)
 - V1785N (ADC)

## Run the first example

```
cd template
make
./Main
```

For other examples see the `test` directory.

# TODO

- [ ] Documentation 
- [ ] Classes for other modules

# Disclaimer
It is not stable. Usage of this product is under your own responsibility. 
