function [m, t] = script_qd2mt(u1, u2, theta)
m = u2*sin(theta) + u1*cos(theta);
t = -u1*sin(theta) + u2*cos(theta);
% im = im * sqrt(3/2);    % 转换为有效值
% it = it * sqrt(3/2);