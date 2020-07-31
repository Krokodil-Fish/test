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

MODELDEF Setup1
{
PORT electrical: A;
PORT electrical: B;
PORT electrical: C;
PORT ROTATIONAL_V: ROT1;
PORT ROTATIONAL_V: ROT2;
PORT REAL OUT ANGLE[deg]: Pos = VM_Pos.V;
PORT REAL IN ANGLE[deg]: IniPos = 0;

INTERN  R        Ra  N1:=A, N2:=N0_1  ( R:=11.6381 ); 
INTERN  R        Rb  N1:=B, N2:=N0_2  ( R:=11.6381 ); 
INTERN  R        Rc  N1:=C, N2:=N0_3  ( R:=11.6381 ); 
INTERN  L        La  N1:=N0_1, N2:=N0_4  ( L:=0.0254582, I0:=0 ); 
INTERN  L        Lb  N1:=N0_2, N2:=N0_5  ( L:=0.0254582, I0:=0 ); 
INTERN  L        Lc  N1:=N0_3, N2:=N0_6  ( L:=0.0254582, I0:=0 ); 
UMODEL Transformation_3To2 AB0 NAP:=N0_4, NAN:=C, NBP:=N0_5, NBN:=A, NCP:=N0_6, NCN:=B, NALPHA:=alpha, NBETA:=beta, NZERO:=zero () SRC: DLL( File:="Transformation.dll");

INTERN  EV       Speed  N1:=NM_1, N2:=GND  ( QUANT:=Torq.V, FACT:=1 ); 
INTERN  II       Torq  N1:=NM_6, N2:=NM_5  ( QUANT:=Speed.I, FACT:=1 ); 
UMODEL D2D D2D1 N1:=NM_5, N2:=ROT1 ( NATURE_1:="electrical", NATURE_2:="Rotational_V" ) SRC: DLL( File:="Domains.dll");
UMODEL D2D D2D2 N1:=NM_6, N2:=ROT2 ( NATURE_1:="electrical", NATURE_2:="Rotational_V" ) SRC: DLL( File:="Domains.dll");
INTERN  VM       VM_Pos  N1:=NM_2, N2:=GND  ; 
MODEL Setup1_AB0 model1 N_1:=alpha, N_2:=beta, N_3:=zero, N_4:=NM_1, N_5:=NM_2 ( pos0:=IniPos );


// BEGIN ANSOFT HEADER
//         Left: N_1 N_2 N_3
//        Right: N_4 N_5
// END ANSOFT HEADER

MODELDEF Setup1_AB0
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
INTERN  E        BVmd  N1:=N_31, N2:=N_25  ( EMF:=0.148527 * N_4.V * VImq.I, PARTDERIV:=1 ); 
INTERN  L        Lad  N1:=N_31, N2:=N_33  ( L:=0.0742633, I0:=0 ); 
INTERN  AM       VImd  N1:=N_33, N2:=GND  ; 
INTERN  I        BI1q  N1:=GND, N2:=N_24  ( IS:=VIa.I*sin(N_8.V) - VIb.I*cos(N_8.V), PARTDERIV:=1 ); 
INTERN  AM       VI1q  N1:=N_24, N2:=N_26  ; 
INTERN  E        BVmq  N1:=N_32, N2:=N_26  ( EMF:=-0.148527 * N_4.V * VImd.I, PARTDERIV:=1 ); 
INTERN  L        Laq  N1:=N_32, N2:=N_34  ( L:=0.0742633, I0:=0 ); 
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
.MODEL Setup1_ke_table pwl TABLE=( 361, -2.87979, -2.86234, -2.84489, -2.82743,\
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
 3.29867, 3.31613, 3.33358, 3.35103, 3.36849, 3.38594, 3.40339, 3, -0.698161,\
 -0.748908, -0.811604, -0.89484, -0.97505, -1.02746, -1.05586, -1.07164,\
 -1.08141, -1.08809, -1.09297, -1.09674, -1.09979, -1.10241, -1.10481,\
 -1.10717, -1.10967, -1.1125, -1.11589, -1.12015, -1.12572, -1.13326, -1.14397,\
 -1.16024, -1.18762, -1.23892, -1.33636, -1.48655, -1.63824, -1.74081,\
 -1.80327, -1.85397, -1.91662, -1.9998, -2.07995, -2.13228, -2.16058, -2.17623,\
 -2.18586, -2.19234, -2.19698, -2.20043, -2.20308, -2.2052, -2.20694, -2.20847,\
 -2.20989, -2.21132, -2.21286, -2.21468, -2.21695, -2.21998, -2.22431,\
 -2.23104, -2.24283, -2.26589, -2.31104, -2.38113, -2.44968, -2.48964,\
 -2.50143, -2.48972, -2.44985, -2.38139, -2.31141, -2.26638, -2.24348,\
 -2.23188, -2.22538, -2.22135, -2.21869, -2.21689, -2.21568, -2.21491,\
 -2.21447, -2.21433, -2.21447, -2.21491, -2.21568, -2.21689, -2.21869,\
 -2.22135, -2.22538, -2.23188, -2.24348, -2.26638, -2.31141, -2.38139,\
 -2.44985, -2.48972, -2.50143, -2.48964, -2.44968, -2.38113, -2.31104,\
 -2.26589, -2.24283, -2.23104, -2.22431, -2.21998, -2.21695, -2.21468,\
 -2.21286, -2.21132, -2.20989, -2.20847, -2.20694, -2.2052, -2.20308, -2.20043,\
 -2.19698, -2.19234, -2.18586, -2.17623, -2.16058, -2.13228, -2.07995, -1.9998,\
 -1.91662, -1.85397, -1.80327, -1.74081, -1.63824, -1.48655, -1.33636,\
 -1.23892, -1.18762, -1.16024, -1.14397, -1.13326, -1.12572, -1.12015,\
 -1.11589, -1.1125, -1.10967, -1.10717, -1.10481, -1.10241, -1.09979, -1.09674,\
 -1.09297, -1.08809, -1.08141, -1.07164, -1.05586, -1.02746, -0.97505,\
 -0.89484, -0.811604, -0.748908, -0.698161, -0.635666, -0.533059, -0.381324,\
 -0.231095, -0.13361, -0.0822534, -0.0548081, -0.038451, -0.0276422,\
 -0.0199715, -0.0142461, -0.00977841, -0.00611967, -0.00294595, 0, 0.00294595,\
 0.00611967, 0.00977841, 0.0142461, 0.0199715, 0.0276422, 0.038451, 0.0548081,\
 0.0822534, 0.13361, 0.231095, 0.381324, 0.533059, 0.635666, 0.698161,\
 0.748908, 0.811604, 0.89484, 0.97505, 1.02746, 1.05586, 1.07164, 1.08141,\
 1.08809, 1.09297, 1.09674, 1.09979, 1.10241, 1.10481, 1.10717, 1.10967,\
 1.1125, 1.11589, 1.12015, 1.12572, 1.13326, 1.14397, 1.16024, 1.18762,\
 1.23892, 1.33636, 1.48655, 1.63824, 1.74081, 1.80327, 1.85397, 1.91662,\
 1.9998, 2.07995, 2.13228, 2.16058, 2.17623, 2.18586, 2.19234, 2.19698,\
 2.20043, 2.20308, 2.2052, 2.20694, 2.20847, 2.20989, 2.21132, 2.21286,\
 2.21468, 2.21695, 2.21998, 2.22431, 2.23104, 2.24283, 2.26589, 2.31104,\
 2.38113, 2.44968, 2.48964, 2.50143, 2.48972, 2.44985, 2.38139, 2.31141,\
 2.26638, 2.24348, 2.23188, 2.22538, 2.22135, 2.21869, 2.21689, 2.21568,\
 2.21491, 2.21447, 2.21433, 2.21447, 2.21491, 2.21568, 2.21689, 2.21869,\
 2.22135, 2.22538, 2.23188, 2.24348, 2.26638, 2.31141, 2.38139, 2.44985,\
 2.48972, 2.50143, 2.48964, 2.44968, 2.38113, 2.31104, 2.26589, 2.24283,\
 2.23104, 2.22431, 2.21998, 2.21695, 2.21468, 2.21286, 2.21132, 2.20989,\
 2.20847, 2.20694, 2.2052, 2.20308, 2.20043, 2.19698, 2.19234, 2.18586,\
 2.17623, 2.16058, 2.13228, 2.07995, 1.9998, 1.91662, 1.85397, 1.80327,\
 1.74081, 1.63824, 1.48655, 1.33636, 1.23892, 1.18762, 1.16024, 1.14397,\
 1.13326, 1.12572, 1.12015, 1.11589, 1.1125, 1.10967, 1.10717, 1.10481,\
 1.10241, 1.09979, 1.09674, 1.09297, 1.08809, 1.08141, 1.07164, 1.05586,\
 1.02746, 0.97505, 0.89484, 0.811604, 0.748908, 0.698161, 0.635666, 0.533059,\
 0.381324, 0.231095, 0.13361, 0.0822534, 0.0548081, 0.038451, 0.0276422,\
 0.0199715, 0.0142461, 0.00977841, 0.00611967, 0.00294595, 0, -0.00294595,\
 -0.00611967, -0.00977841, -0.0142461, -0.0199715, -0.0276422, -0.038451,\
 -0.0548081, -0.0822534, -0.13361, -0.231095, -0.381324, -0.533059, -0.635666,\
 -0.698161, 2.48532, 2.4425, 2.36026, 2.23315, 2.10604, 2.02379, 1.98094,\
 1.95844, 1.94529, 1.93678, 1.93089, 1.92664, 1.92349, 1.92108, 1.91919,\
 1.91767, 1.91639, 1.91525, 1.91419, 1.91313, 1.91199, 1.91071, 1.90918,\
 1.90729, 1.90487, 1.9017, 1.89744, 1.89153, 1.883, 1.86982, 1.84728, 1.8044,\
 1.72209, 1.5949, 1.4677, 1.38535, 1.34239, 1.31974, 1.3064, 1.29767, 1.29149,\
 1.28687, 1.28324, 1.28024, 1.27758, 1.27506, 1.27248, 1.26964, 1.26631,\
 1.26219, 1.25689, 1.24979, 1.2398, 1.22481, 1.19992, 1.15393, 1.06744,\
 0.93443, 0.798801, 0.703391, 0.638034, 0.572676, 0.47726, 0.341621, 0.208602,\
 0.122086, 0.0760735, 0.0511552, 0.0361148, 0.026078, 0.0189031, 0.0135173,\
 0.00929517, 0.00582461, 0.002806, 0, -0.002806, -0.00582461, -0.00929517,\
 -0.0135173, -0.0189031, -0.026078, -0.0361148, -0.0511552, -0.0760735,\
 -0.122086, -0.208602, -0.341621, -0.47726, -0.572676, -0.638034, -0.703391,\
 -0.798801, -0.93443, -1.06744, -1.15393, -1.19992, -1.22481, -1.2398,\
 -1.24979, -1.25689, -1.26219, -1.26631, -1.26964, -1.27248, -1.27506,\
 -1.27758, -1.28024, -1.28324, -1.28687, -1.29149, -1.29767, -1.3064, -1.31974,\
 -1.34239, -1.38535, -1.4677, -1.5949, -1.72209, -1.8044, -1.84728, -1.86982,\
 -1.883, -1.89153, -1.89744, -1.9017, -1.90487, -1.90729, -1.90918, -1.91071,\
 -1.91199, -1.91313, -1.91419, -1.91525, -1.91639, -1.91767, -1.91919,\
 -1.92108, -1.92349, -1.92664, -1.93089, -1.93678, -1.94529, -1.95844,\
 -1.98094, -2.02379, -2.10604, -2.23315, -2.36026, -2.4425, -2.48532, -2.50779,\
 -2.52089, -2.52933, -2.53514, -2.53928, -2.54231, -2.54454, -2.54621,\
 -2.54745, -2.54838, -2.54906, -2.54955, -2.54987, -2.55006, -2.55012,\
 -2.55006, -2.54987, -2.54955, -2.54906, -2.54838, -2.54745, -2.54621,\
 -2.54454, -2.54231, -2.53928, -2.53514, -2.52933, -2.52089, -2.50779,\
 -2.48532, -2.4425, -2.36026, -2.23315, -2.10604, -2.02379, -1.98094, -1.95844,\
 -1.94529, -1.93678, -1.93089, -1.92664, -1.92349, -1.92108, -1.91919,\
 -1.91767, -1.91639, -1.91525, -1.91419, -1.91313, -1.91199, -1.91071,\
 -1.90918, -1.90729, -1.90487, -1.9017, -1.89744, -1.89153, -1.883, -1.86982,\
 -1.84728, -1.8044, -1.72209, -1.5949, -1.4677, -1.38535, -1.34239, -1.31974,\
 -1.3064, -1.29767, -1.29149, -1.28687, -1.28324, -1.28024, -1.27758, -1.27506,\
 -1.27248, -1.26964, -1.26631, -1.26219, -1.25689, -1.24979, -1.2398, -1.22481,\
 -1.19992, -1.15393, -1.06744, -0.93443, -0.798801, -0.703391, -0.638034,\
 -0.572676, -0.47726, -0.341621, -0.208602, -0.122086, -0.0760735, -0.0511552,\
 -0.0361148, -0.026078, -0.0189031, -0.0135173, -0.00929517, -0.00582461,\
 -0.002806, 0, 0.002806, 0.00582461, 0.00929517, 0.0135173, 0.0189031,\
 0.026078, 0.0361148, 0.0511552, 0.0760735, 0.122086, 0.208602, 0.341621,\
 0.47726, 0.572676, 0.638034, 0.703391, 0.798801, 0.93443, 1.06744, 1.15393,\
 1.19992, 1.22481, 1.2398, 1.24979, 1.25689, 1.26219, 1.26631, 1.26964,\
 1.27248, 1.27506, 1.27758, 1.28024, 1.28324, 1.28687, 1.29149, 1.29767,\
 1.3064, 1.31974, 1.34239, 1.38535, 1.4677, 1.5949, 1.72209, 1.8044, 1.84728,\
 1.86982, 1.883, 1.89153, 1.89744, 1.9017, 1.90487, 1.90729, 1.90918, 1.91071,\
 1.91199, 1.91313, 1.91419, 1.91525, 1.91639, 1.91767, 1.91919, 1.92108,\
 1.92349, 1.92664, 1.93089, 1.93678, 1.94529, 1.95844, 1.98094, 2.02379,\
 2.10604, 2.23315, 2.36026, 2.4425, 2.48532, 2.50779, 2.52089, 2.52933,\
 2.53514, 2.53928, 2.54231, 2.54454, 2.54621, 2.54745, 2.54838, 2.54906,\
 2.54955, 2.54987, 2.55006, 2.55012, 2.55006, 2.54987, 2.54955, 2.54906,\
 2.54838, 2.54745, 2.54621, 2.54454, 2.54231, 2.53928, 2.53514, 2.52933,\
 2.52089, 2.50779, 2.48532, -0.411758, -0.395182, -0.338781, -0.241952,\
 -0.142981, -0.079297, -0.0468979, -0.0304926, -0.0212971, -0.0155954,\
 -0.011834, -0.00928923, -0.00757809, -0.00647999, -0.00586621, -0.00566872,\
 -0.00586621, -0.00647999, -0.00757809, -0.00928923, -0.011834, -0.0155954,\
 -0.0212971, -0.0304926, -0.0468979, -0.079297, -0.142981, -0.241952,\
 -0.338781, -0.395182, -0.411758, -0.395106, -0.338625, -0.241709, -0.142636,\
 -0.0788318, -0.0462865, -0.029701, -0.0202808, -0.0142966, -0.0101777,\
 -0.0071781, -0.00488532, -0.0030393, -0.00145795, 0, 0.00145795, 0.0030393,\
 0.00488532, 0.0071781, 0.0101777, 0.0142966, 0.0202808, 0.029701, 0.0462865,\
 0.0788318, 0.142636, 0.241709, 0.338625, 0.395106, 0.411758, 0.395182,\
 0.338781, 0.241952, 0.142981, 0.079297, 0.0468979, 0.0304926, 0.0212971,\
 0.0155954, 0.011834, 0.00928923, 0.00757809, 0.00647999, 0.00586621,\
 0.00566872, 0.00586621, 0.00647999, 0.00757809, 0.00928923, 0.011834,\
 0.0155954, 0.0212971, 0.0304926, 0.0468979, 0.079297, 0.142981, 0.241952,\
 0.338781, 0.395182, 0.411758, 0.395106, 0.338625, 0.241709, 0.142636,\
 0.0788318, 0.0462865, 0.029701, 0.0202808, 0.0142966, 0.0101777, 0.0071781,\
 0.00488532, 0.0030393, 0.00145795, 0, -0.00145795, -0.0030393, -0.00488532,\
 -0.0071781, -0.0101777, -0.0142966, -0.0202808, -0.029701, -0.0462865,\
 -0.0788318, -0.142636, -0.241709, -0.338625, -0.395106, -0.411758, -0.395182,\
 -0.338781, -0.241952, -0.142981, -0.079297, -0.0468979, -0.0304926,\
 -0.0212971, -0.0155954, -0.011834, -0.00928923, -0.00757809, -0.00647999,\
 -0.00586621, -0.00566872, -0.00586621, -0.00647999, -0.00757809, -0.00928923,\
 -0.011834, -0.0155954, -0.0212971, -0.0304926, -0.0468979, -0.079297,\
 -0.142981, -0.241952, -0.338781, -0.395182, -0.411758, -0.395106, -0.338625,\
 -0.241709, -0.142636, -0.0788318, -0.0462865, -0.029701, -0.0202808,\
 -0.0142966, -0.0101777, -0.0071781, -0.00488532, -0.0030393, -0.00145795, 0,\
 0.00145795, 0.0030393, 0.00488532, 0.0071781, 0.0101777, 0.0142966, 0.0202808,\
 0.029701, 0.0462865, 0.0788318, 0.142636, 0.241709, 0.338625, 0.395106,\
 0.411758, 0.395182, 0.338781, 0.241952, 0.142981, 0.079297, 0.0468979,\
 0.0304926, 0.0212971, 0.0155954, 0.011834, 0.00928923, 0.00757809, 0.00647999,\
 0.00586621, 0.00566872, 0.00586621, 0.00647999, 0.00757809, 0.00928923,\
 0.011834, 0.0155954, 0.0212971, 0.0304926, 0.0468979, 0.079297, 0.142981,\
 0.241952, 0.338781, 0.395182, 0.411758, 0.395106, 0.338625, 0.241709,\
 0.142636, 0.0788318, 0.0462865, 0.029701, 0.0202808, 0.0142966, 0.0101777,\
 0.0071781, 0.00488532, 0.0030393, 0.00145795, 0, -0.00145795, -0.0030393,\
 -0.00488532, -0.0071781, -0.0101777, -0.0142966, -0.0202808, -0.029701,\
 -0.0462865, -0.0788318, -0.142636, -0.241709, -0.338625, -0.395106, -0.411758,\
 -0.395182, -0.338781, -0.241952, -0.142981, -0.079297, -0.0468979, -0.0304926,\
 -0.0212971, -0.0155954, -0.011834, -0.00928923, -0.00757809, -0.00647999,\
 -0.00586621, -0.00566872, -0.00586621, -0.00647999, -0.00757809, -0.00928923,\
 -0.011834, -0.0155954, -0.0212971, -0.0304926, -0.0468979, -0.079297,\
 -0.142981, -0.241952, -0.338781, -0.395182, -0.411758, -0.395106, -0.338625,\
 -0.241709, -0.142636, -0.0788318, -0.0462865, -0.029701, -0.0202808,\
 -0.0142966, -0.0101777, -0.0071781, -0.00488532, -0.0030393, -0.00145795, 0,\
 0.00145795, 0.0030393, 0.00488532, 0.0071781, 0.0101777, 0.0142966, 0.0202808,\
 0.029701, 0.0462865, 0.0788318, 0.142636, 0.241709, 0.338625, 0.395106,\
 0.411758, 0.395182, 0.338781, 0.241952, 0.142981, 0.079297, 0.0468979,\
 0.0304926, 0.0212971, 0.0155954, 0.011834, 0.00928923, 0.00757809, 0.00647999,\
 0.00586621, 0.00566872, 0.00586621, 0.00647999, 0.00757809, 0.00928923,\
 0.011834, 0.0155954, 0.0212971, 0.0304926, 0.0468979, 0.079297, 0.142981,\
 0.241952, 0.338781, 0.395182, 0.411758, 0.395106, 0.338625, 0.241709,\
 0.142636, 0.0788318, 0.0462865, 0.029701, 0.0202808, 0.0142966, 0.0101777,\
 0.0071781, 0.00488532, 0.0030393, 0.00145795, 0, -0.00145795, -0.0030393,\
 -0.00488532, -0.0071781, -0.0101777, -0.0142966, -0.0202808, -0.029701,\
 -0.0462865, -0.0788318, -0.142636, -0.241709, -0.338625, -0.395106, -0.411758)\
 PERIODIC NOSPLINE" );
}

}
