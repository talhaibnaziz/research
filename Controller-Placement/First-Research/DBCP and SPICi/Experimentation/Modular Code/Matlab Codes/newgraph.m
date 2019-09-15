fileID = fopen('New Results/dbcp_new.txt','r');
x1 = zeros(1,4);
y1 = zeros(1,4);
input1 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:4
    input1(i) = input(idx+1);
    y1(i) = input(idx+2);
    x1(i) = input(idx+3);
    idx = idx + 3;
end
y1 = [10 15 34 100];
fileID = fopen('New Results/l-dbcp_new.txt','r');
x2 = zeros(1,4);
y2 = zeros(1,4);
input2 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:4
    input2(i) = input(idx+1);
    y2(i) = input(idx+2);
    x2(i) = input(idx+3);
    idx = idx + 3;
end
fileID = fopen('New Results/w-dbcp_new.txt','r');
x3 = zeros(1,4);
y3 = zeros(1,4);
input3 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:3
    input3(i) = input(idx+1);
    y3(i) = input(idx+2);
    x3(i) = input(idx+3);
    idx = idx + 3;
end
%for input graph 5
x3(4) = input(idx+3+3);
fileID = fopen('New Results/lw-dbcp_new.txt','r');
x4 = zeros(1,4);
y4 = zeros(1,4);
input4 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:3
    input4(i) = input(idx+1);
    y4(i) = input(idx+2);
    x4(i) = input(idx+3);
    idx = idx + 3;
end
%for input graph 5
x4(4) = input(idx+3+3);
fileID = fopen('New Results/ispici_m.txt','r');
x5 = zeros(1,4);
y5 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:3
    y5(i) = input(idx+1);
    x5(i) = input(idx+2);
    idx = idx + 2;
end
%for input graph 5
x5(4) = input(idx+2+2);
fileID = fopen('New Results/gspici_m.txt','r');
x6 = zeros(1,4);
y6 = zeros(1,4);
idx = 0;
input = fscanf(fileID,'%f');
for i=1:3
    y6(i) = input(idx+1);
    x6(i) = input(idx+2);
    idx = idx + 2;
end
%for input graph 5
x6(4) = input(idx+2+2);
plot(y1,x1,y1,x2,y1,x3,y1,x4,y1,x5,y1,x6);
xlabel('Number of Switches');
ylabel('m_{latency}');
legend('DBCP','L-DBCP','W-DBCP','LW-DBCP','ISPICi', 'GSPICi');
print('new graph','-dpng');