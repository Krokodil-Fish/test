// BEGIN ANSOFT HEADER
//         Left: A B C
//        Right: ROT1 ROT2 Pos

// A           : Phase A
// B           : Phase B
// C           : Phase C
// ROT1        : Mechanical plus
// ROT2        : Mechanical minus
// Pos         : Position
// END ANSOFT HEADER

MODELDEF Setup2
{
PORT electrical: A;
PORT electrical: B;
PORT electrical: C;
PORT ROTATIONAL_V: ROT1;
PORT ROTATIONAL_V: ROT2;
PORT REAL OUT ANGLE[deg]: Pos = VM_Pos.V;
PORT REAL IN ANGLE[deg]: IniPos = 0;

INTERN  R        Ra  N1:=A, N2:=N0_1  ( R:=18.7043 ); 
INTERN  R        Rb  N1:=B, N2:=N0_2  ( R:=18.7043 ); 
INTERN  R        Rc  N1:=C, N2:=N0_3  ( R:=18.7043 ); 
INTERN  L        La  N1:=N0_1, N2:=N0_4  ( L:=0.0401874, I0:=0 ); 
INTERN  L        Lb  N1:=N0_2, N2:=N0_5  ( L:=0.0401874, I0:=0 ); 
INTERN  L        Lc  N1:=N0_3, N2:=N0_6  ( L:=0.0401874, I0:=0 ); 
UMODEL Transformation_3To2 AB0 NAP:=N0_4, NAN:=C, NBP:=N0_5, NBN:=A, NCP:=N0_6, NCN:=B, NALPHA:=alpha, NBETA:=beta, NZERO:=zero () SRC: DLL( File:="Transformation.dll");

INTERN  EV       Speed  N1:=NM_1, N2:=GND  ( QUANT:=Torq.V, FACT:=1 ); 
INTERN  II       Torq  N1:=NM_6, N2:=NM_5  ( QUANT:=Speed.I, FACT:=1 ); 
UMODEL D2D D2D1 N1:=NM_5, N2:=ROT1 ( NATURE_1:="electrical", NATURE_2:="Rotational_V" ) SRC: DLL( File:="Domains.dll");
UMODEL D2D D2D2 N1:=NM_6, N2:=ROT2 ( NATURE_1:="electrical", NATURE_2:="Rotational_V" ) SRC: DLL( File:="Domains.dll");
INTERN  VM       VM_Pos  N1:=NM_2, N2:=GND  ; 
MODEL Setup2_AB0 model1 N_1:=alpha, N_2:=beta, N_3:=zero, N_4:=NM_1, N_5:=NM_2 ( pos0:=IniPos );


// BEGIN ANSOFT HEADER
//         Left: N_1 N_2 N_3
//        Right: N_4 N_5
// END ANSOFT HEADER

MODELDEF Setup2_AB0
{
PORT electrical: N_1;
PORT electrical: N_4;
PORT electrical: N_2;
PORT electrical: N_5;
PORT electrical: N_3;
PORT REAL IN: pos0 = 0;

INTERN  AM       VR1a  N1:=N_1, N2:=N_17  ; 
INTERN  E        BEma  N1:=N_19, N2:=N_17  ( EMF:=N_4.V * N_10.V, PARTDERIV:=1 ); 
INTERN  E        BVa  N1:=N_21, N2:=N_19  ( EMF:=N_23.V*cos(N_8.V) + N_24.V*sin(N_8.V), PARTDERIV:=1 ); 
INTERN  AM       VIa  N1:=N_21, N2:=GND  ; 
INTERN  VM       VM_Ea  N1:=N_17, N2:=GND  ; 
INTERN  AM       VR1b  N1:=N_2, N2:=N_18  ; 
INTERN  E        BEmb  N1:=N_20, N2:=N_18  ( EMF:=N_4.V * N_11.V, PARTDERIV:=1 ); 
INTERN  E        BVb  N1:=N_22, N2:=N_20  ( EMF:=N_23.V*sin(N_8.V) - N_24.V*cos(N_8.V), PARTDERIV:=1 ); 
INTERN  AM       VIb  N1:=N_22, N2:=GND  ; 
INTERN  VM       VM_Eb  N1:=N_18, N2:=GND  ; 
INTERN  AM       VR0  N1:=N_3, N2:=N_14  ; 
INTERN  E        BEm0  N1:=GND, N2:=N_14  ( EMF:=N_4.V * N_12.V, PARTDERIV:=1 ); 
INTERN  I        BI1d  N1:=GND, N2:=N_23  ( IS:=VIa.I*cos(N_8.V) + VIb.I*sin(N_8.V), PARTDERIV:=1 ); 
INTERN  AM       VI1d  N1:=N_23, N2:=N_25  ; 
INTERN  E        BVmd  N1:=N_31, N2:=N_25  ( EMF:=0.248338 * N_4.V * VImq.I, PARTDERIV:=1 ); 
INTERN  L        Lad  N1:=N_31, N2:=N_33  ( L:=0.124169, I0:=0 ); 
INTERN  AM       VImd  N1:=N_33, N2:=GND  ; 
INTERN  I        BI1q  N1:=GND, N2:=N_24  ( IS:=VIa.I*sin(N_8.V) - VIb.I*cos(N_8.V), PARTDERIV:=1 ); 
INTERN  AM       VI1q  N1:=N_24, N2:=N_26  ; 
INTERN  E        BVmq  N1:=N_32, N2:=N_26  ( EMF:=-0.248338 * N_4.V * VImd.I, PARTDERIV:=1 ); 
INTERN  L        Laq  N1:=N_32, N2:=N_34  ( L:=0.124169, I0:=0 ); 
INTERN  AM       VImq  N1:=N_34, N2:=GND  ; 
INTERN  I        BTm  N1:=GND, N2:=N_4  ( IS:=(VIa.I*N_10.V + VIb.I*N_11.V), PARTDERIV:=1 ); 
INTERN  R        RT  N1:=N_4, N2:=GND  ( R:=1e12 ); 
INTERN  VM       VM_Speed  N1:=N_4, N2:=GND  ; 
INTERN  IV       Gx  N1:=GND, N2:=N_7  ( QUANT:=VM_Speed.V, FACT:=1 ); 
INTERN  C        Cx  N1:=N_7, N2:=GND  ( C:=1, V0:=pos0*0.0174533 ); 
INTERN  VM       VM_Pos  N1:=N_7, N2:=GND  ; 
INTERN  EV       Ex1  N1:=GND, N2:=N_8  ( QUANT:=VM_Pos.V, FACT:=2 ); 
INTERN  VM       VM_Ex1  N1:=N_8, N2:=GND  ; 
INTERN  EV       Ex2  N1:=GND, N2:=N_5  ( QUANT:=VM_Pos.V, FACT:=57.2958 ); 
INTERN  VM       VM_kea  N1:=N_10, N2:=GND  ; 
INTERN  VM       VM_keb  N1:=N_11, N2:=GND  ; 
INTERN  VM       VM_ke0  N1:=N_12, N2:=GND  ; 
INTERN  NDSRC    Pke N0:=N_10, N1:=GND, N2:=N_11, N3:=GND, N4:=N_12, N5:=GND  ( QUANT:={N_8.V}, SRC:={vsrc, vsrc, vsrc}, TableData:="\
.MODEL Setup2_ke_table pwl TABLE=( 361, -2.87979, -2.86234, -2.84489, -2.82743,\
 -2.80998, -2.79253, -2.77507, -2.75762, -2.74017, -2.72271, -2.70526,\
 -2.68781, -2.67035, -2.6529, -2.63545, -2.61799, -2.60054, -2.58309, -2.56563,\
 -2.54818, -2.53073, -2.51327, -2.49582, -2.47837, -2.46091, -2.44346,\
 -2.42601, -2.40855, -2.3911, -2.37365, -2.35619, -2.33874, -2.32129, -2.30383,\
 -2.28638, -2.26893, -2.25147, -2.23402, -2.21657, -2.19911, -2.18166,\
 -2.16421, -2.14675, -2.1293, -2.11185, -2.0944, -2.07694, -2.05949, -2.04204,\
 -2.02458, -2.00713, -1.98968, -1.97222, -1.95477, -1.93732, -1.91986,\
 -1.90241, -1.88496, -1.8675, -1.85005, -1.8326, -1.81514, -1.79769, -1.78024,\
 -1.76278, -1.74533, -1.72788, -1.71042, -1.69297, -1.67552, -1.65806,\
 -1.64061, -1.62316, -1.6057, -1.58825, -1.5708, -1.55334, -1.53589, -1.51844,\
 -1.50098, -1.48353, -1.46608, -1.44862, -1.43117, -1.41372, -1.39626,\
 -1.37881, -1.36136, -1.3439, -1.32645, -1.309, -1.29154, -1.27409, -1.25664,\
 -1.23918, -1.22173, -1.20428, -1.18682, -1.16937, -1.15192, -1.13446,\
 -1.11701, -1.09956, -1.0821, -1.06465, -1.0472, -1.02974, -1.01229, -0.994838,\
 -0.977384, -0.959931, -0.942478, -0.925025, -0.907571, -0.890118, -0.872665,\
 -0.855211, -0.837758, -0.820305, -0.802851, -0.785398, -0.767945, -0.750492,\
 -0.733038, -0.715585, -0.698132, -0.680678, -0.663225, -0.645772, -0.628319,\
 -0.610865, -0.593412, -0.575959, -0.558505, -0.541052, -0.523599, -0.506145,\
 -0.488692, -0.471239, -0.453786, -0.436332, -0.418879, -0.401426, -0.383972,\
 -0.366519, -0.349066, -0.331613, -0.314159, -0.296706, -0.279253, -0.261799,\
 -0.244346, -0.226893, -0.20944, -0.191986, -0.174533, -0.15708, -0.139626,\
 -0.122173, -0.10472, -0.0872665, -0.0698132, -0.0523599, -0.0349066,\
 -0.0174533, 0, 0.0174533, 0.0349066, 0.0523599, 0.0698132, 0.0872665, 0.10472,\
 0.122173, 0.139626, 0.15708, 0.174533, 0.191986, 0.20944, 0.226893, 0.244346,\
 0.261799, 0.279253, 0.296706, 0.314159, 0.331613, 0.349066, 0.366519,\
 0.383972, 0.401426, 0.418879, 0.436332, 0.453786, 0.471239, 0.488692,\
 0.506145, 0.523599, 0.541052, 0.558505, 0.575959, 0.593412, 0.610865,\
 0.628319, 0.645772, 0.663225, 0.680678, 0.698132, 0.715585, 0.733038,\
 0.750492, 0.767945, 0.785398, 0.802851, 0.820305, 0.837758, 0.855211,\
 0.872665, 0.890118, 0.907571, 0.925025, 0.942478, 0.959931, 0.977384,\
 0.994838, 1.01229, 1.02974, 1.0472, 1.06465, 1.0821, 1.09956, 1.11701,\
 1.13446, 1.15192, 1.16937, 1.18682, 1.20428, 1.22173, 1.23918, 1.25664,\
 1.27409, 1.29154, 1.309, 1.32645, 1.3439, 1.36136, 1.37881, 1.39626, 1.41372,\
 1.43117, 1.44862, 1.46608, 1.48353, 1.50098, 1.51844, 1.53589, 1.55334,\
 1.5708, 1.58825, 1.6057, 1.62316, 1.64061, 1.65806, 1.67552, 1.69297, 1.71042,\
 1.72788, 1.74533, 1.76278, 1.78024, 1.79769, 1.81514, 1.8326, 1.85005, 1.8675,\
 1.88496, 1.90241, 1.91986, 1.93732, 1.95477, 1.97222, 1.98968, 2.00713,\
 2.02458, 2.04204, 2.05949, 2.07694, 2.0944, 2.11185, 2.1293, 2.14675, 2.16421,\
 2.18166, 2.19911, 2.21657, 2.23402, 2.25147, 2.26893, 2.28638, 2.30383,\
 2.32129, 2.33874, 2.35619, 2.37365, 2.3911, 2.40855, 2.42601, 2.44346,\
 2.46091, 2.47837, 2.49582, 2.51327, 2.53073, 2.54818, 2.56563, 2.58309,\
 2.60054, 2.61799, 2.63545, 2.6529, 2.67035, 2.68781, 2.70526, 2.72271,\
 2.74017, 2.75762, 2.77507, 2.79253, 2.80998, 2.82743, 2.84489, 2.86234,\
 2.87979, 2.89725, 2.9147, 2.93215, 2.94961, 2.96706, 2.98451, 3.00197,\
 3.01942, 3.03687, 3.05433, 3.07178, 3.08923, 3.10669, 3.12414, 3.14159,\
 3.15905, 3.1765, 3.19395, 3.21141, 3.22886, 3.24631, 3.26377, 3.28122,\
 3.29867, 3.31613, 3.33358, 3.35103, 3.36849, 3.38594, 3.40339, 3, -0.875019,\
 -0.938056, -1.01645, -1.12167, -1.22305, -1.2885, -1.32367, -1.34323,\
 -1.35537, -1.36364, -1.36964, -1.37422, -1.3779, -1.38102, -1.38387, -1.38667,\
 -1.38965, -1.39304, -1.39714, -1.40234, -1.40919, -1.41857, -1.43194,\
 -1.45228, -1.48631, -1.5499, -1.67167, -1.86165, -2.05355, -2.18177, -2.25928,\
 -2.32228, -2.40063, -2.50578, -2.6071, -2.67247, -2.70753, -2.72696, -2.73893,\
 -2.74699, -2.75271, -2.75694, -2.76017, -2.76271, -2.7648, -2.76662, -2.76832,\
 -2.77003, -2.7719, -2.77412, -2.77694, -2.78073, -2.78617, -2.79463, -2.80931,\
 -2.8379, -2.89434, -2.98306, -3.06984, -3.11975, -3.1343, -3.11983, -3.07001,\
 -2.98332, -2.89471, -2.83841, -2.80998, -2.79551, -2.78732, -2.78221,\
 -2.77883, -2.77656, -2.77504, -2.77406, -2.77352, -2.77334, -2.77352,\
 -2.77406, -2.77504, -2.77656, -2.77883, -2.78221, -2.78732, -2.79551,\
 -2.80998, -2.83841, -2.89471, -2.98332, -3.07001, -3.11983, -3.1343, -3.11975,\
 -3.06984, -2.98306, -2.89434, -2.8379, -2.80931, -2.79463, -2.78617, -2.78073,\
 -2.77694, -2.77412, -2.7719, -2.77003, -2.76832, -2.76662, -2.7648, -2.76271,\
 -2.76017, -2.75694, -2.75271, -2.74699, -2.73893, -2.72696, -2.70753,\
 -2.67247, -2.6071, -2.50578, -2.40063, -2.32228, -2.25928, -2.18177, -2.05355,\
 -1.86165, -1.67167, -1.5499, -1.48631, -1.45228, -1.43194, -1.41857, -1.40919,\
 -1.40234, -1.39714, -1.39304, -1.38965, -1.38667, -1.38387, -1.38102, -1.3779,\
 -1.37422, -1.36964, -1.36364, -1.35537, -1.34323, -1.32367, -1.2885, -1.22305,\
 -1.12167, -1.01645, -0.938056, -0.875019, -0.797471, -0.669211, -0.477272,\
 -0.287242, -0.165427, -0.101772, -0.0676644, -0.0472393, -0.0337472,\
 -0.0242266, -0.0171817, -0.0117374, -0.00731973, -0.00351599, 0, 0.00351599,\
 0.00731973, 0.0117374, 0.0171817, 0.0242266, 0.0337472, 0.0472393, 0.0676644,\
 0.101772, 0.165427, 0.287242, 0.477272, 0.669211, 0.797471, 0.875019,\
 0.938056, 1.01645, 1.12167, 1.22305, 1.2885, 1.32367, 1.34323, 1.35537,\
 1.36364, 1.36964, 1.37422, 1.3779, 1.38102, 1.38387, 1.38667, 1.38965,\
 1.39304, 1.39714, 1.40234, 1.40919, 1.41857, 1.43194, 1.45228, 1.48631,\
 1.5499, 1.67167, 1.86165, 2.05355, 2.18177, 2.25928, 2.32228, 2.40063,\
 2.50578, 2.6071, 2.67247, 2.70753, 2.72696, 2.73893, 2.74699, 2.75271,\
 2.75694, 2.76017, 2.76271, 2.7648, 2.76662, 2.76832, 2.77003, 2.7719, 2.77412,\
 2.77694, 2.78073, 2.78617, 2.79463, 2.80931, 2.8379, 2.89434, 2.98306,\
 3.06984, 3.11975, 3.1343, 3.11983, 3.07001, 2.98332, 2.89471, 2.83841,\
 2.80998, 2.79551, 2.78732, 2.78221, 2.77883, 2.77656, 2.77504, 2.77406,\
 2.77352, 2.77334, 2.77352, 2.77406, 2.77504, 2.77656, 2.77883, 2.78221,\
 2.78732, 2.79551, 2.80998, 2.83841, 2.89471, 2.98332, 3.07001, 3.11983,\
 3.1343, 3.11975, 3.06984, 2.98306, 2.89434, 2.8379, 2.80931, 2.79463, 2.78617,\
 2.78073, 2.77694, 2.77412, 2.7719, 2.77003, 2.76832, 2.76662, 2.7648, 2.76271,\
 2.76017, 2.75694, 2.75271, 2.74699, 2.73893, 2.72696, 2.70753, 2.67247,\
 2.6071, 2.50578, 2.40063, 2.32228, 2.25928, 2.18177, 2.05355, 1.86165,\
 1.67167, 1.5499, 1.48631, 1.45228, 1.43194, 1.41857, 1.40919, 1.40234,\
 1.39714, 1.39304, 1.38965, 1.38667, 1.38387, 1.38102, 1.3779, 1.37422,\
 1.36964, 1.36364, 1.35537, 1.34323, 1.32367, 1.2885, 1.22305, 1.12167,\
 1.01645, 0.938056, 0.875019, 0.797471, 0.669211, 0.477272, 0.287242, 0.165427,\
 0.101772, 0.0676644, 0.0472393, 0.0337472, 0.0242266, 0.0171817, 0.0117374,\
 0.00731973, 0.00351599, 0, -0.00351599, -0.00731973, -0.0117374, -0.0171817,\
 -0.0242266, -0.0337472, -0.0472393, -0.0676644, -0.101772, -0.165427,\
 -0.287242, -0.477272, -0.669211, -0.797471, -0.875019, 3.11399, 3.06088,\
 2.95809, 2.79725, 2.6364, 2.53359, 2.48046, 2.45246, 2.43599, 2.42532,\
 2.41796, 2.41269, 2.40881, 2.40587, 2.4036, 2.40178, 2.40027, 2.39894, 2.3977,\
 2.39645, 2.39512, 2.39361, 2.39178, 2.3895, 2.38657, 2.38267, 2.37739,\
 2.37001, 2.35932, 2.34282, 2.31478, 2.26161, 2.15874, 1.99782, 1.83689,\
 1.73397, 1.68071, 1.65255, 1.63587, 1.62494, 1.61725, 1.61156, 1.60714,\
 1.6035, 1.60032, 1.59731, 1.59423, 1.59083, 1.58681, 1.5818, 1.57529, 1.56649,\
 1.55405, 1.53535, 1.50444, 1.44744, 1.33937, 1.17116, 0.999635, 0.880348,\
 0.799206, 0.718061, 0.598769, 0.427233, 0.259012, 0.150919, 0.093898,\
 0.062955, 0.0442073, 0.0317122, 0.0228359, 0.0162331, 0.0111084, 0.0069357,\
 0.00333384, -1.57009e-16, -0.00333384, -0.0069357, -0.0111084, -0.0162331,\
 -0.0228359, -0.0317122, -0.0442073, -0.062955, -0.093898, -0.150919,\
 -0.259012, -0.427233, -0.598769, -0.718061, -0.799206, -0.880348, -0.999635,\
 -1.17116, -1.33937, -1.44744, -1.50444, -1.53535, -1.55405, -1.56649,\
 -1.57529, -1.5818, -1.58681, -1.59083, -1.59423, -1.59731, -1.60032, -1.6035,\
 -1.60714, -1.61156, -1.61725, -1.62494, -1.63587, -1.65255, -1.68071,\
 -1.73397, -1.83689, -1.99782, -2.15874, -2.26161, -2.31478, -2.34282,\
 -2.35932, -2.37001, -2.37739, -2.38267, -2.38657, -2.3895, -2.39178, -2.39361,\
 -2.39512, -2.39645, -2.3977, -2.39894, -2.40027, -2.40178, -2.4036, -2.40587,\
 -2.40881, -2.41269, -2.41796, -2.42532, -2.43599, -2.45246, -2.48046,\
 -2.53359, -2.6364, -2.79725, -2.95809, -3.06088, -3.11399, -3.14196, -3.15838,\
 -3.16898, -3.17625, -3.18141, -3.18515, -3.18789, -3.18992, -3.19143,\
 -3.19255, -3.19336, -3.19394, -3.19433, -3.19455, -3.19462, -3.19455,\
 -3.19433, -3.19394, -3.19336, -3.19255, -3.19143, -3.18992, -3.18789,\
 -3.18515, -3.18141, -3.17625, -3.16898, -3.15838, -3.14196, -3.11399,\
 -3.06088, -2.95809, -2.79725, -2.6364, -2.53359, -2.48046, -2.45246, -2.43599,\
 -2.42532, -2.41796, -2.41269, -2.40881, -2.40587, -2.4036, -2.40178, -2.40027,\
 -2.39894, -2.3977, -2.39645, -2.39512, -2.39361, -2.39178, -2.3895, -2.38657,\
 -2.38267, -2.37739, -2.37001, -2.35932, -2.34282, -2.31478, -2.26161,\
 -2.15874, -1.99782, -1.83689, -1.73397, -1.68071, -1.65255, -1.63587,\
 -1.62494, -1.61725, -1.61156, -1.60714, -1.6035, -1.60032, -1.59731, -1.59423,\
 -1.59083, -1.58681, -1.5818, -1.57529, -1.56649, -1.55405, -1.53535, -1.50444,\
 -1.44744, -1.33937, -1.17116, -0.999635, -0.880348, -0.799206, -0.718061,\
 -0.598769, -0.427233, -0.259012, -0.150919, -0.093898, -0.062955, -0.0442073,\
 -0.0317122, -0.0228359, -0.0162331, -0.0111084, -0.0069357, -0.00333384,\
 1.57009e-16, 0.00333384, 0.0069357, 0.0111084, 0.0162331, 0.0228359,\
 0.0317122, 0.0442073, 0.062955, 0.093898, 0.150919, 0.259012, 0.427233,\
 0.598769, 0.718061, 0.799206, 0.880348, 0.999635, 1.17116, 1.33937, 1.44744,\
 1.50444, 1.53535, 1.55405, 1.56649, 1.57529, 1.5818, 1.58681, 1.59083,\
 1.59423, 1.59731, 1.60032, 1.6035, 1.60714, 1.61156, 1.61725, 1.62494,\
 1.63587, 1.65255, 1.68071, 1.73397, 1.83689, 1.99782, 2.15874, 2.26161,\
 2.31478, 2.34282, 2.35932, 2.37001, 2.37739, 2.38267, 2.38657, 2.3895,\
 2.39178, 2.39361, 2.39512, 2.39645, 2.3977, 2.39894, 2.40027, 2.40178, 2.4036,\
 2.40587, 2.40881, 2.41269, 2.41796, 2.42532, 2.43599, 2.45246, 2.48046,\
 2.53359, 2.6364, 2.79725, 2.95809, 3.06088, 3.11399, 3.14196, 3.15838,\
 3.16898, 3.17625, 3.18141, 3.18515, 3.18789, 3.18992, 3.19143, 3.19255,\
 3.19336, 3.19394, 3.19433, 3.19455, 3.19462, 3.19455, 3.19433, 3.19394,\
 3.19336, 3.19255, 3.19143, 3.18992, 3.18789, 3.18515, 3.18141, 3.17625,\
 3.16898, 3.15838, 3.14196, 3.11399, -0.517208, -0.496727, -0.426254,\
 -0.303649, -0.178333, -0.0986991, -0.0584932, -0.0380234, -0.0264326,\
 -0.0192043, -0.0144346, -0.0112166, -0.00906028, -0.00768065, -0.00691114,\
 -0.00666381, -0.00691114, -0.00768065, -0.00906028, -0.0112166, -0.0144346,\
 -0.0192043, -0.0264326, -0.0380234, -0.0584932, -0.0986991, -0.178333,\
 -0.303649, -0.426254, -0.496727, -0.517208, -0.496648, -0.426091, -0.303393,\
 -0.177967, -0.0982021, -0.0578347, -0.0371631, -0.0253176, -0.0177652,\
 -0.0125809, -0.00882994, -0.00598529, -0.00371248, -0.0017776, 0, 0.0017776,\
 0.00371248, 0.00598529, 0.00882994, 0.0125809, 0.0177652, 0.0253176,\
 0.0371631, 0.0578347, 0.0982021, 0.177967, 0.303393, 0.426091, 0.496648,\
 0.517208, 0.496727, 0.426254, 0.303649, 0.178333, 0.0986991, 0.0584932,\
 0.0380234, 0.0264326, 0.0192043, 0.0144346, 0.0112166, 0.00906028, 0.00768065,\
 0.00691114, 0.00666381, 0.00691114, 0.00768065, 0.00906028, 0.0112166,\
 0.0144346, 0.0192043, 0.0264326, 0.0380234, 0.0584932, 0.0986991, 0.178333,\
 0.303649, 0.426254, 0.496727, 0.517208, 0.496648, 0.426091, 0.303393,\
 0.177967, 0.0982021, 0.0578347, 0.0371631, 0.0253176, 0.0177652, 0.0125809,\
 0.00882994, 0.00598529, 0.00371248, 0.0017776, 0, -0.0017776, -0.00371248,\
 -0.00598529, -0.00882994, -0.0125809, -0.0177652, -0.0253176, -0.0371631,\
 -0.0578347, -0.0982021, -0.177967, -0.303393, -0.426091, -0.496648, -0.517208,\
 -0.496727, -0.426254, -0.303649, -0.178333, -0.0986991, -0.0584932,\
 -0.0380234, -0.0264326, -0.0192043, -0.0144346, -0.0112166, -0.00906028,\
 -0.00768065, -0.00691114, -0.00666381, -0.00691114, -0.00768065, -0.00906028,\
 -0.0112166, -0.0144346, -0.0192043, -0.0264326, -0.0380234, -0.0584932,\
 -0.0986991, -0.178333, -0.303649, -0.426254, -0.496727, -0.517208, -0.496648,\
 -0.426091, -0.303393, -0.177967, -0.0982021, -0.0578347, -0.0371631,\
 -0.0253176, -0.0177652, -0.0125809, -0.00882994, -0.00598529, -0.00371248,\
 -0.0017776, 0, 0.0017776, 0.00371248, 0.00598529, 0.00882994, 0.0125809,\
 0.0177652, 0.0253176, 0.0371631, 0.0578347, 0.0982021, 0.177967, 0.303393,\
 0.426091, 0.496648, 0.517208, 0.496727, 0.426254, 0.303649, 0.178333,\
 0.0986991, 0.0584932, 0.0380234, 0.0264326, 0.0192043, 0.0144346, 0.0112166,\
 0.00906028, 0.00768065, 0.00691114, 0.00666381, 0.00691114, 0.00768065,\
 0.00906028, 0.0112166, 0.0144346, 0.0192043, 0.0264326, 0.0380234, 0.0584932,\
 0.0986991, 0.178333, 0.303649, 0.426254, 0.496727, 0.517208, 0.496648,\
 0.426091, 0.303393, 0.177967, 0.0982021, 0.0578347, 0.0371631, 0.0253176,\
 0.0177652, 0.0125809, 0.00882994, 0.00598529, 0.00371248, 0.0017776, 0,\
 -0.0017776, -0.00371248, -0.00598529, -0.00882994, -0.0125809, -0.0177652,\
 -0.0253176, -0.0371631, -0.0578347, -0.0982021, -0.177967, -0.303393,\
 -0.426091, -0.496648, -0.517208, -0.496727, -0.426254, -0.303649, -0.178333,\
 -0.0986991, -0.0584932, -0.0380234, -0.0264326, -0.0192043, -0.0144346,\
 -0.0112166, -0.00906028, -0.00768065, -0.00691114, -0.00666381, -0.00691114,\
 -0.00768065, -0.00906028, -0.0112166, -0.0144346, -0.0192043, -0.0264326,\
 -0.0380234, -0.0584932, -0.0986991, -0.178333, -0.303649, -0.426254,\
 -0.496727, -0.517208, -0.496648, -0.426091, -0.303393, -0.177967, -0.0982021,\
 -0.0578347, -0.0371631, -0.0253176, -0.0177652, -0.0125809, -0.00882994,\
 -0.00598529, -0.00371248, -0.0017776, 0, 0.0017776, 0.00371248, 0.00598529,\
 0.00882994, 0.0125809, 0.0177652, 0.0253176, 0.0371631, 0.0578347, 0.0982021,\
 0.177967, 0.303393, 0.426091, 0.496648, 0.517208, 0.496727, 0.426254,\
 0.303649, 0.178333, 0.0986991, 0.0584932, 0.0380234, 0.0264326, 0.0192043,\
 0.0144346, 0.0112166, 0.00906028, 0.00768065, 0.00691114, 0.00666381,\
 0.00691114, 0.00768065, 0.00906028, 0.0112166, 0.0144346, 0.0192043,\
 0.0264326, 0.0380234, 0.0584932, 0.0986991, 0.178333, 0.303649, 0.426254,\
 0.496727, 0.517208, 0.496648, 0.426091, 0.303393, 0.177967, 0.0982021,\
 0.0578347, 0.0371631, 0.0253176, 0.0177652, 0.0125809, 0.00882994, 0.00598529,\
 0.00371248, 0.0017776, 0, -0.0017776, -0.00371248, -0.00598529, -0.00882994,\
 -0.0125809, -0.0177652, -0.0253176, -0.0371631, -0.0578347, -0.0982021,\
 -0.177967, -0.303393, -0.426091, -0.496648, -0.517208) PERIODIC NOSPLINE" );
}

}
