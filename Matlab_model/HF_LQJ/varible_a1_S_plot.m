x=[-20:0.01:20];
y=2./(exp(-0.2*x)+1)-1;
y1=2./(exp(-1*x)+1)-1;
y2=2./(exp(-5*x)+1)-1;
plot(x,y,'b',x,y1,'r',x,y2,'g');
grid on
xlabel('i-alpha');
ylabel('H-alpha');
