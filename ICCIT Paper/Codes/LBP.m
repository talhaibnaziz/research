function output = LBP(Input_Im, R)

% %=======================================================================
% %=======================================================================
% This function computes the LBP transformation of the input image
% Input_Im. 
% The parameters of the LBP operator are (P = 8, R), 
% where P - the number of sampling points in the region with the radius R. 
% Radius R is the input parameter of the function.
% Possible values for R = 1, 2, 3, etc.
% If input image is COLOR, then the grayscale transformation is performed.
% %=======================================================================
% %=======================================================================


if size(Input_Im, 3) == 3
    Input_Im = rgb2gray(Input_Im);
end;

%Gaussian Low Pass Filter Applied Here
% Create a blurring kernel.
kernel = fspecial('Gaussian', 32, 2);
% Blur the image.
Input_Im = imfilter(Input_Im, kernel, 'replicate');

% % Label each blob with 8-connectivity, so we can make measurements of it
% labeledImage = logical(Input_Im);
% % Get all the blob properties.
% blobMeasurements = regionprops(labeledImage, 'Orientation');   
% allOrientations = [blobMeasurements.Orientation];
% % Extract just one orientation (there should only be one in this image).
% angleToRotate = -allOrientations(1);
% % Rotate the image.
% disp(size(Input_Im));
% disp(Input_Im(1,:));
% Input_Im = imrotate(Input_Im, angleToRotate);
% Input_Im = imresize(Input_Im, [32 32]);
% disp(size(Input_Im));
% disp(Input_Im(1,:));

%Main part of code
L = 2*R + 1; %% The size of the LBP label
C = round(L/2);
Input_Im = uint8(Input_Im);
row_max = size(Input_Im,1)-L+1;
col_max = size(Input_Im,2)-L+1;
LBP_Im = zeros(row_max, col_max);
for i = 1:row_max
    for j = 1:col_max
        A = Input_Im(i:i+L-1, j:j+L-1);
        A = A+1-A(C,C);
        A(A>0) = 1;
        LBP_Im(i,j) = A(C,L) + A(L,L)*2 + A(L,C)*4 + A(L,1)*8 + A(C,1)*16 + A(1,1)*32 + A(1,C)*64 + A(1,L)*128;
    end;
    if(i==1)
        output = LBP_Im(1,:);
    else
        output = [output LBP_Im(i,:)];
    end
end;