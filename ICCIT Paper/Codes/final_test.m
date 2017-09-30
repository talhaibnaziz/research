
trainset = zeros(5000, 1024*16);
testset = zeros(1000, 1024*16);
trainres = zeros(5000, 1);
testres = zeros(1000, 3);
for thou=0:4
    for loop=0:9
        for i=0:9
            for j=0:9
                name = strcat('bn0',char(thou)+48);
                name = strcat(name,char(loop)+48);
                name = strcat(name,char(i)+48);
                name = strcat(name,char(j)+48);
                name = strcat('C:\Users\Talha\Desktop\ICCIT Paper\CMATERdb 3.1.1\BengaliBMP\',name);
                name = strcat(name, '.bmp');
                img = imread(char(name));
                trainset(thou*1000+loop*100+i*10+j+1,:) = LDP_Edit(img);
                trainres(thou*1000+loop*100+i*10+j+1, 1) = j;
            end
        end
    end
end
knn = fitcknn(trainset, trainres);
naive = fitcnb(trainset, trainres);

for loop=0:9
    for i=0:9
        for j=0:9
            name = strcat('bn05',char(loop)+48);
            name = strcat(name,char(i)+48);
            name = strcat(name,char(j)+48);
            name = strcat('C:\Users\Talha\Desktop\ICCIT Paper\CMATERdb 3.1.1\BengaliBMP\',name);
            name = strcat(name, '.bmp');
            img = imread(char(name));
            testset(loop*100+i*10+j+1,:) = LDP_Edit(img);
            testres(loop*100+i*10+j+1, 1) = j;
        end
    end
end

testres(:,2) = predict(knn, testset);
testres(:,3) = predict(naive, trainset);

correct = 0;
incorrect = 0;
for i=1:1000
    if(testres(i,1)==testres(i,2))
        correct = correct + 1;
    else
        incorrect = incorrect + 1;
    end
end
disp('KNN Result');
disp(correct);
disp(incorrect);
for i=1:1000
    if(testres(i,1)==testres(i,3))
        correct = correct + 1;
    else
        incorrect = incorrect + 1;
    end
end
disp('Naive-Bayes Result');
disp(correct);
disp(incorrect);