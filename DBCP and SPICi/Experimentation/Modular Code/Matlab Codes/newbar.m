fileID = fopen('New Results/dbcp_new.txt','r');
x1 = zeros(1,4);
y1 = zeros(1,4);
input1 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:5
    input1(i) = input(idx+1);
    y1(i) = input(idx+2);
    x1(i) = input(idx+3);
    idx = idx + 3;
end
ys = [11 18 42 153 350];
yb = [11 18 42 2907 153 350 1028];
fileID = fopen('New Results/l-dbcp_new.txt','r');
x2 = zeros(1,5);
y2 = zeros(1,5);
input2 = zeros(1,5);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:5
    input2(i) = input(idx+1);
    y2(i) = input(idx+2);
    x2(i) = input(idx+3);
    idx = idx + 3;
end
fileID = fopen('New Results/w-dbcp_new.txt','r');
x3 = zeros(1,7);
y3 = zeros(1,7);
input3 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    input3(i) = input(idx+1);
    y3(i) = input(idx+2);
    x3(i) = input(idx+3);
    idx = idx + 3;
end
fileID = fopen('New Results/lw-dbcp_new.txt','r');
x4 = zeros(1,7);
y4 = zeros(1,7);
input4 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    input4(i) = input(idx+1);
    y4(i) = input(idx+2);
    x4(i) = input(idx+3);
    idx = idx + 3;
end
fileID = fopen('New Results/ispici_m.txt','r');
x5 = zeros(1,7);
y5 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    y5(i) = input(idx+1);
    x5(i) = input(idx+2);
    idx = idx + 2;
end
fileID = fopen('New Results/gspici_m.txt','r');
x6 = zeros(1,7);
y6 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    y6(i) = input(idx+1);
    x6(i) = input(idx+2);
    idx = idx + 2;
end
y = [0 0 x3(4) x4(4) x5(4) x6(4); x1(4) x2(4) x3(5) x4(5) x5(5) x6(5); x1(5) x2(5) x3(6) x4(6) x5(6) x6(6); 0 0 x3(7) x4(7) x5(7) x6(7)];
bar(y);
xlabel('Scenarios');
ylabel('m_{latency}');
legend('DBCP','L-DBCP','W-DBCP','LW-DBCP','ISPICi', 'GSPICi');
print('new bar','-dpng');