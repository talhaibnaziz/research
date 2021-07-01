fileID = fopen('dbcp_new.txt','r');
x1 = zeros(1,7);
k1 = zeros(1,7);
input1 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    input1(i) = input(idx+1);
    k1(i) = input(idx+2);
    x1(i) = input(idx+3);
    idx = idx + 3;
end
y1 = [40 50 60 70 80 90 100];
fileID = fopen('lmspici_new.txt','r');
x2 = zeros(1,7);
y2 = zeros(1,7);
k2 = zeros(1,7);
input2 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    input2(i) = input(idx+1);
    y2(i) = input(idx+2);
    y2(i) = input(idx+3);
    k2(i) = input(idx+4);
    x2(i) = input(idx+5);
    idx = idx + 5;
end
fileID = fopen('forceddbcp_new.txt','r');
x3 = zeros(1,7);
y3 = zeros(1,7);
input3 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    input3(i) = input(idx+1);
    x3(i) = input(idx+2);
    x3(i) = input(idx+3);
    idx = idx + 3;
end
fileID = fopen('gspici_m_metric.txt','r');
x4 = zeros(1,7);
k4 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    k4(i) = input(idx+1);
    x4(i) = input(idx+2);
    idx = idx + 2;
end
fileID = fopen('ispici_m_metric.txt','r');
x5 = zeros(1,7);
k5 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    k5(i) = input(idx+1);
    x5(i) = input(idx+2);
    idx = idx + 2;
end
fileID = fopen('wdbcp_new.txt','r');
x6 = zeros(1,7);
k6 = zeros(1,7);
input6 = zeros(1,7);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:7
    input6(i) = input(idx+1);
    k6(i) = input(idx+2);
    x6(i) = input(idx+3);
    idx = idx + 3;
end
figure;
plot(y1,x2,y1,x4,y1,x5);
xlabel('Number of Switches');
ylabel('m_{latency}');
legend('M-SPICI','G-SPICi','I-SPICi');
print('wnewgraph','-dpng');
figure;
plot(y1,x1,y1,x2,y1,x3,y1,x6);
xlabel('Number of Switches');
ylabel('m_{latency}');
legend('DBCP','M-SPICI','Forced-DBCP','W-DBCP');
print('wnewspici','-dpng');

figure;
k1 = k1'; k5 = k5';
k2 = k2'; k6 = k6';
k4 = k4';
y = [k1 k2 k4 k5 k6];
bar(y1,y);
xlabel('Number of Switches');
ylabel('k (controller)');
legend('DBCP','M-SPICI','G-SPICi','I-SPICi','W-DBCP');
print('wnewbar','-dpng');