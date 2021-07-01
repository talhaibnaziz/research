function [result] = extra_GDP(img)
img=imresize(img,[32 32]);
% img=double(img);
img=rgb2gray(img);
[Gx, Gy] = imgradientxy(img,'sobel');
[Gmag, Gdr] = imgradient(Gx, Gy);

dx = [ 0, -1, -1, -1, 0, 1, 1, 1 ];
dy = [ -1, -1, 0, 1, 1, 1, 0, -1 ];

histo =[];
t=28.5;
windowr=6;
windowc=6;
 for i=2:31
    for j=2:31
        sm=0;
        for p=1:8
            if(abs(Gdr(i,j)-Gdr(i+dx(p),j+dy(p)))<=t)
                s=1;
            else s=0;
            end
            sm=sm+s*(2.^(p-1));
        end
        GDPCode(i-1,j-1)=sm;
    end
 end

 r=(30/windowr);
 for m=1:windowr
    for n=1:windowc
       h=zeros(1,256); 
       for i=(m-1)*r+1:m*r  
           if i>30 
               break;
           end
          for j=(n-1)*r+1:n*r
               if j>30 
               break;
               end
             h( 1, GDPCode( i,j )+1 ) = h( 1, GDPCode( i,j )+1 ) + 1;
          end
       end
%        disp(h);
       histo=[histo h];
       
    end
 end
result=histo;