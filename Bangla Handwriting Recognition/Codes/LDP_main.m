%I = imread('sample1.png');
%[ldpCode, his] = LDP_Edit(I);
%disp(his);
%name='new';
%for i=0:9
%    for j=0:9
%        name = strcat('bn00',char(i)+48);
%        name = strcat(name,char(j)+48);
%        disp(name);
%    end
%end
trainset = zeros(100,1);
trainres = zeros(100,1);
testres = zeros(100,3);
for i=1:100
    trainset(i,1) = mod(i,10);
    if(trainset(i,1)>3 && trainset(i,1)<8)
        trainres(i,1) = 0;
        testres(i,1) = 0;
    else
        trainres(i,1) = 1;
        testres(i,1) = 1;
    end
end
knn = fitcknn(trainset, trainres);
naive = fitcnb(trainset, trainres);
testres(:,2) = predict(knn, trainset);
testres(:,3) = predict(naive, trainset);
disp(testres);