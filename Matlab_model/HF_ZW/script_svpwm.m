%% intro
% u1: ratio, 0~1.155
% u2: theta, -pi~pi,
% us_sv: us in sv

%% function
function [cmpa, cmpb, cmpc, us_sv, ualpha, ubeta, cmp_a_b] = svpwm(ratio, theta)

% us_sv = ratio;

%% 使用电压积分有效值的方法, 得到ratio-us表格
% k = [1.0 1.013 1.029 1.049 1.08 1.175 1.26 1.32 1.38 1.445 1.51 1.58 1.66 1.74 1.83 1.96 2];
% if ratio > 1.15   % ratio:1.01, k:1.013
%     us_sv = k(end);
% elseif ratio > 1.0
%     index = floor(ratio*100 - 100);
%     index_remain = ratio - 1.0 - index*0.01;
%     us_sv = k(index + 1)+ (k(index + 2) - k(index + 1))*index_remain/0.01;
% else
%     us_sv = ratio;   % ratio to Us_sv, make a gain k
% end

%% 使用fft分析基波的方法, 得到ratio-us表格
% ratio: 1.0  1.01  1.02  1.03  1.04 1.05 1.06  1.07  1.08  1.09  1.10
k =     [1.0 1.013 1.029 1.049 1.08 1.175 1.275 1.355 1.445 1.565 1.78  2];
if ratio > 1.103   
    us_sv = k(end);
elseif ratio > 1.0
    index = floor(ratio*100 - 100);
    index_remain = ratio - 1.0 - index*0.01;
    us_sv = k(index + 1)+ (k(index + 2) - k(index + 1))*index_remain/0.01;
else
    us_sv = ratio;   % ratio to Us_sv, make a gain k
end


sector = 0;

ualpha = us_sv * cos(theta);
ubeta = us_sv * sin(theta);

Va = ubeta;
Vb = 0.8660254*ualpha - 0.5*ubeta;
Vc = -Vb - ubeta;

X = ubeta;
Y = 0.8660254*ualpha + 0.5*ubeta;
Z = -0.8660254*ualpha + 0.5*ubeta;

if Va>0
	sector=sector+1;
end
if Vb>0
	sector=sector+2;
end
if Vc>0
sector=sector+4;
end
switch(sector)
    case 0
        t1=0;
        t2=0;
    case 1
        t1=Z;
        t2=Y;
    case 2
        t1=Y;
        t2=-X;

    case 3
        t1=-Z;
        t2=X;
    case 4
        t1=-X;
        t2=Z;
    case 5
        t1=X;
        t2=-Y;
    case 6
        t1=-Y;
        t2=-Z;
    otherwise
        t1=0;
        t2=0;
end
if(t1+t2>1)
    if((t1>t2)&&(t1>1))
        t1=1;
        t2=0;
    elseif((t1<=t2)&&(t2>1))
        t1=0;
        t2=1;
    else
        t1=t1/(t1+t2);
        t2=1-t1;
    end
end

%taon=(1-t1-t2);%五段式SVPWM
taon = (1-t1-t2)*0.5;%七段式SVPWM
tbon = taon + t1;
tcon = tbon + t2;

switch(sector)
	case 0
        Ta=taon;
        Tb=tbon;
        Tc=tcon;
    case 1
        Ta=tbon;
        Tb=taon;
        Tc=tcon;
    case 2
        Ta=taon;
        Tb=tcon;
        Tc=tbon;
    case 3
        Ta=taon;
        Tb=tbon;
        Tc=tcon;
    case 4
        Ta=tcon;
        Tb=tbon;
        Tc=taon;
    case 5
        Ta=tcon;
        Tb=taon;
        Tc=tbon;
    case 6
        Ta=tbon;
        Tb=tcon;
        Tc=taon;
    otherwise
        Ta=0;
        Tb=0;
        Tc=0;
end

cmpa = 1 - Ta;
cmpb = 1 - Tb;
cmpc = 1 - Tc;


cmp_a_b = cmpa - cmpb;

