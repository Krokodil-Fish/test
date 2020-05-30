function [H] =S(i)
i=[-20:5:20];
H=2./(exp(-0.2*i)+1)-1;

figure(1)
plot(H,i);
grid on
xlabel('i-alpha');
ylabel('H-alpha');