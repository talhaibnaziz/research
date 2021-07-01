ldpdataset = zeros(6000, 256*16);
gdpdataset = zeros(6000, 256*36);
svmdataset = zeros(6000, 256*16);
datares = zeros(6000, 1);
experiment_result = zeros(4, 6000, 1);

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
                ldpdataset(thou*1000+loop*100+i*10+j+1,:) = LDP_Edit(img);
                gdpdataset(thou*1000+loop*100+i*10+j+1,:) = GDP(img);
                svmdataset(thou*1000+loop*100+i*10+j+1,:) = LDP_Edit(img);
                datares(thou*1000+loop*100+i*10+j+1, 1) = j;
            end
        end
    end
end

ldptrainset = zeros(5000, 256*16);
gdptrainset = zeros(5000, 256*36);
svmtrainset = zeros(5000, 256*16);
trainres = zeros(5000, 1);
ldptestset = zeros(1000, 256*16);
gdptestset = zeros(1000, 256*36);
svmtestset = zeros(1000, 256*16);
testres = zeros(1000, 1);
correct = 0; incorrect = 0;

%6 Cross Fold Validation
for i=1:1000:5001
    ldptestset = ldpdataset(i:i+999, :);
    gdptestset = gdpdataset(i:i+999, :);
    svmtestset = svmdataset(i:i+999, :);
    testres = datares(i:i+999, :);
    if(i==1)
        %train from 1001 to 6000 test from 1 to 1000
        ldptrainset = ldpdataset(1001:6000, :);
        gdptrainset = gdpdataset(1001:6000, :);
        svmtrainset = svmdataset(1001:6000, :);
        trainres = datares(1001:6000, :);
    elseif(i==5001)
        %train from 1 to 5000 test from 5001 to 6000
        ldptrainset = ldpdataset(1:5000, :);
        gdptrainset = gdpdataset(1:5000, :);
        svmtrainset = svmdataset(1:5000, :);
        trainres = datares(1:5000, :);
    else
        %train from both sides of i and i+999
        ldptrainset(1:i-1, :) = ldpdataset(1:i-1, :);
        ldptrainset(i:5000, :) = ldpdataset(i+1000:6000, :);
        gdptrainset(1:i-1, :) = gdpdataset(1:i-1, :);
        gdptrainset(i:5000, :) = gdpdataset(i+1000:6000, :);
        svmtrainset(1:i-1, :) = svmdataset(1:i-1, :);
        svmtrainset(i:5000, :) = svmdataset(i+1000:6000, :);
        trainres(1:i-1, :) = datares(1:i-1, :);
        trainres(i:5000, :) = datares(i+1000:6000, :);
    end
    gdpknn = fitcknn(gdptrainset, trainres, 'NumNeighbors', 3);
    ldpknn = fitcknn(ldptrainset, trainres, 'NumNeighbors', 3);
    experiment_result(1, i:i+999, :) = predict(gdpknn, gdptestset);
    experiment_result(2, i:i+999, :) = predict(ldpknn, ldptestset);
    experiment_result(3, i:i+999, 1) = multisvm(svmtrainset, trainres, svmtestset);
end

for i=1:6000
    if(experiment_result(2, i, 1)==experiment_result(3, i, 1))
        experiment_result(4, i, 1) = experiment_result(2, i, 1);
    else
        experiment_result(4, i, 1) = experiment_result(1, i, 1);
    end
    if(datares(i,1)==experiment_result(4, i, 1))
        correct = correct + 1;
    else
        incorrect = incorrect + 1;
    end
end
disp(correct/60);
disp(incorrect/60);
count = zeros(4, 10);
for i=1:10
    for j=i:10:6000
        for ii=1:4
            if(experiment_result(ii, j, 1) == datares(j,1))
                count(ii,i) = count(ii,i) + 1;
            end
        end
    end
end
disp(count);
beep;
