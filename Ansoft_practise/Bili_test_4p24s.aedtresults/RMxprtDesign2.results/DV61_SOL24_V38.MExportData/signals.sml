MODELDEF Pulse
{
PORT REAL IN: input = time;
PORT REAL IN: period = 0.02;
PORT REAL IN: pulseWidth = 0.01;
PORT REAL IN: delay = 0;
PORT REAL OUT: output = 0;

INTERN EQU {
  Vref := cos(pi*pulseWidth/period);
  Vsine := cos(2*pi*(input-pulseWidth/2-delay)/period);
  output := if(Vsine>Vref, 1, 0);
}
}

MODELDEF PWM
{
PORT REAL IN: input = time;
PORT REAL IN: period = 0.02;
PORT REAL IN: indexModu = 0.9;
PORT REAL IN: timesFreq = 100;
PORT REAL IN ANGLE[deg]: phaseLag = 0;
PORT REAL OUT: output = 0;

INTERN EQU {
  inputStd := rem(input / period * timesFreq);
  IF (inputStd < 0) {inputStd := 1 + inputStd;}
  Vref := if(inputStd<0.5, 4*inputStd-1, 3-4*inputStd);
  Vsine := indexModu * sin(2*pi*input/period - phaseLag*pi/180);
  output := if(Vsine>Vref, 1, -1);
}
}

MODELDEF CCC
{
PORT REAL IN: input = 0;
PORT REAL IN: referrence = 1;
PORT REAL IN: bandWidth = 0.2;
PORT REAL IN: twoway = 1;
PORT REAL OUT: output = 0;

INTERN EQU {
  inputStd := if(twoway>0, abs(input), input);
  IF (inputStd > referrence + bandWidth/2) {output := -1;}
  IF (inputStd < referrence - bandWidth/2) {output := 1;}
}
}

MODELDEF Encoder
{
PORT REAL IN ANGLE[deg]: position = 0;
PORT REAL IN: cycles = 128;
PORT REAL OUT: channelA = 0;
PORT REAL OUT: channelB = 0;

INTERN EQU {
  inputStd := cycles*position/57.2957795;
  signalA := -cos(inputStd);
  signalB := -sin(inputStd);
  channelA := if(signalA>0, 1, 0);
  channelB := if(signalB>0, 1, 0);
}
}

