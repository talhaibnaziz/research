function [result] = mymultisvm(TrainingSet,GroupTrain,TestSet)

% Multisvm
% Author: Talha Ibn Aziz
% Islamic University of Technology
% 30 July 2017
s = size(TrainingSet);
disp(s);
result = zeros(1000,1);
for i=1:10
    trainset = TrainingSet(i:10:5000,:);
    trainres = GroupTrain(i:10:5000,:);
    %disp(trainset);
    svm(i) = fitcsvm(trainset,trainres);
end
%disp(TestSet(1,:,:));
s = size(TestSet);
result = zeros(s(1,1), 1);
classes = zeros(s(1,1), 10);
for i=1:10
    classes(:, i) = predict(svm(i),TestSet);
end
disp(classes);