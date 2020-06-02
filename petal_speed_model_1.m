t=0:0.001:35;
vd=2;  %a1为vd，目标速度
v=0.106*t;%a2为v，实际速度
vp= ((vd-v)./vd.*cos(4*(vd-v)./vd)+1).*(v<vd & vd-v>0.2)+(0.5*cos(4*(vd-v)./vd)+1).*(v<vd & vd-v<=0.2)+0.*(v>=vd);
plot(t,vp);
grid on;
