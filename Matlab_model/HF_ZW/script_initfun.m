%% add path 
clc;clear
p1 = mfilename('fullpath');
i = strfind(p1,'\');
p1=p1(1:i(end));
addpath(genpath(p1));

%% paras

Udc = 311;  % 

Ts = -1;
Ts_model = 1e-5;
Ts_torqueloop = 1e-4;
Ts_speedloop = 1e-3;


Ld = 0.0052;
Lq = 0.0174;
R = 0.33;
kf = 0.646;
p = 2; poles = p;

F = 0;
J = 0.008;