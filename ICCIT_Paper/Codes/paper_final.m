lbpdataset = zeros(6000, 900);
datares = zeros(6000, 1);
experiment_result = zeros(6000, 1);

for thou=0:5
    for loop=0:9
        for i=0:9
            for j=0:9
                name = strcat('bn0',char(thou)+48);
                name = strcat(name,char(loop)+48);
                name = strcat(name,char(i)+48);
                name = strcat(name,char(j)+48);
                name = strcat('E:\Work\Research\ICCIT Paper\CMATERdb 3.1.1\BengaliBMP\',name);
                name = strcat(name, '.bmp');
                img = imread(char(name));
                lbpdataset(thou*1000+loop*100+i*10+j+1,:) = LBP(img, 1);
                datares(thou*1000+loop*100+i*10+j+1, 1) = j;
            end
        end
    end
end

lbptrainset = zeros(5000, 900);
trainres = zeros(5000, 1);
lbptestset = zeros(1000, 900);
testres = zeros(1000, 1);
correct = 0; incorrect = 0;

%6 Cross Fold Validation
for i=1:1000:5001
    lbptestset = lbpdataset(i:i+999, :);
    testres = datares(i:i+999, :);
    if(i==1)
        %train from 1001 to 6000 test from 1 to 1000
        lbptrainset = lbpdataset(1001:6000, :);
        trainres = datares(1001:6000, :);
    elseif(i==5001)
        %train from 1 to 5000 test from 5001 to 6000
        lbptrainset = lbpdataset(1:5000, :);
        trainres = datares(1:5000, :);
    else
        %train from both sides of i and i+999
        lbptrainset(1:i-1, :) = lbpdataset(1:i-1, :);
        lbptrainset(i:5000, :) = lbpdataset(i+1000:6000, :);
        trainres(1:i-1, :) = datares(1:i-1, :);
        trainres(i:5000, :) = datares(i+1000:6000, :);
    end
    lbpknn = fitcknn(lbptrainset, trainres, 'NumNeighbors', 3);
    experiment_result(i:i+999, :) = predict(lbpknn, lbptestset);
end

for i=1:6000
    if(datares(i,1)==experiment_result(i, 1))
        correct = correct + 1;
    else
        incorrect = incorrect + 1;
    end
end
disp(correct/60);
beep;
