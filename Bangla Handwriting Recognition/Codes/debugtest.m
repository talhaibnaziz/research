name = cell(100, 1);
trainset = zeros(100, 1024);
result = zeros(100, 3);
for i=0:9
    for j=0:9
        name{i*10+j+1, 1} = strcat('bn001',char(i)+48);
        name{i*10+j+1, 1} = strcat(name{i*10+j+1, 1},char(j)+48);
        name{i*10+j+1, 1} = strcat('C:\Users\Talha\Desktop\ICCIT Paper\CMATERdb 3.1.1\BengaliBMP\',name{i*10+j+1, 1});
        name{i*10+j+1, 1} = strcat(name{i*10+j+1, 1}, '.bmp');
        disp(name);
    end
end