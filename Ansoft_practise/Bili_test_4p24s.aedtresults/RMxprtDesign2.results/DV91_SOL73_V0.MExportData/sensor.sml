// BEGIN ANSOFT HEADER
//         Left: Position PulseWidth
//        Right: Signal
// END ANSOFT HEADER

MODELDEF Sensor
{
PORT electrical: Position;
PORT electrical: PulseWidth;
PORT electrical: Signal;

INTERN  E        Bw  N1:=N_4, N2:=GND  ( EMF:= cos(PulseWidth.V / 114.592), PARTDERIV:=1 ); 
INTERN  E        Bp  N1:=N_4, N2:=N_5  ( EMF:= sin(Position.V / 57.2958), PARTDERIV:=1 ); 
INTERN  E        V1  N1:=N_6, N2:=GND  ( EMF:=-1, PARTDERIV:=1 ); 
INTERN  R        R1  N1:=N_7, N2:=GND  ( R:=1 ); 
INTERN  VM       VM_E1  N1:=N_7, N2:=GND  ; 
INTERN  EV       E1  N1:=Signal, N2:=GND   ( QUANT:=VM_E1.V, FACT:=-1 );
UMODEL CSV  S1 N1:=N_6, N2:=N_7, NC1:=N_5, NC2:=GND   ( RON:=1e-6, ROFF:=1e6, VTHRES:=0, VHYST:=0, CTRL0:=0 ) SRC: DLL( File:="CSV.dll");
}

