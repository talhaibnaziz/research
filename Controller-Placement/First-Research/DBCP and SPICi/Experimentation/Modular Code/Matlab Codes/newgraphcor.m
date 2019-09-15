fileID = fopen('wdbcp_new.txt','r');
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
fileID = fopen('mspici_new.txt','r');
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

plot(y1,x1,y1,x2);
xlabel('Number of Switches');
ylabel('m_{latency}');
legend('WDBCP','M-SPICI');
print('newgraph','-dpng');

plot(y1,x1.*k1,y1,x2.*k2);
xlabel('Number of Switches');
ylabel('m_{latency}*k');
legend('WDBCP','M-SPICI');
print('newgraph','-dpng');