figure(1)
plot(wm.time,wm.signals.values,'-k',wm1.time,wm1.signals.values,':r','linewidth',0.05);
grid off
xlabel('\itt\rm/s','FontSize',16,'Fontname', 'Times New Roman');
ylabel('\itn\rm/r\cdotmin^{-1}','FontSize',16,'Fontname', 'Times New Roman');
xlim([0,0.06]);
ylim([0,1200]);
set(gca,'FontSize',16,'Fontname', 'Times New Roman') 
legend('actual speed','estimated speed','FontSize',16,'Fontname', 'Times New Roman');

figure(2)
plot(Te.time,Te.signals.values,'-k');
grid off
xlabel('\itt\rm/s','FontSize',16,'Fontname', 'Times New Roman');
ylabel('\itTe\rm/N\cdotm','FontSize',16,'Fontname', 'Times New Roman');

figure(3)
plot(thea.time,thea.signals.values,'-k',thee.time,thee.signals.values,':r');
grid off
xlabel('\itt\rm/s','FontSize',16,'Fontname', 'Times New Roman');
ylabel('\itRotor position \Theta\rm/rad','FontSize',16,'Fontname', 'Times New Roman');
xlim([0,0.06]);
legend('actual angle of rotor position','estimated angle of rotor position','FontSize',16,'Fontname', 'Times New Roman');

figure(5)
plot(the_e.time,the_e.signals.values);
grid off
xlabel('t/s');
ylabel('position estimation error/(rad)');

