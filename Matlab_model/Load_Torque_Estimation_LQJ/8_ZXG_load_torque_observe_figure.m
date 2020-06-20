figure(1)
plot(Ta.time,Ta.signals.values,'-k',Te.time,Te.signals.values,':k','linewidth',2);
grid off
xlabel('\itt\rm/s','FontSize',16,'Fontname', 'Times New Roman');
ylabel('\itT\rm/N\cdotm','FontSize',16,'Fontname', 'Times New Roman');
ylim([0,120]);
xlim([0.2,0.3]);
set(gca,'FontSize',16,'Fontname', 'Times New Roman') 