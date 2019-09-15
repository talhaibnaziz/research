function [ result ] = LDP_Edit( img )
%img = imresize( img, [100 100 ]);
%figure,imshow( img );
result = [];
[ r c k ] = size( img );
kmask = zeros( 3,3,8 );
kmask( :,:,1 ) = [ -3 -3 5 ; -3 0 5; -3 -3 5 ];
kmask( :,:,2 ) = [ -3 5 5 ; -3 0 5; -3 -3 -3 ];
kmask( :,:,3 ) = [ 5 5 5 ; -3 0 -3; -3 -3 -3 ];
kmask( :,:,4 ) = [ 5 5 -3 ; 5 0 -3; -3 -3 -3 ];
kmask( :,:,5 ) = [ 5 -3 -3 ; 5 0 -3; 5 -3 -3 ];
kmask( :,:,6 ) = [ -3 -3 -3 ; 5 0 -3; 5 5 -3 ];
kmask( :,:,7 ) = [ -3 -3 -3 ; -3 0 -3; 5 5 5 ];
kmask( :,:,8 ) = [ -3 -3 -3 ; -3 0 5; -3 5 5 ];


% var2 = rgb2gray( img ) ;
var2 = double(img);

% fid = fopen('out1.txt' , 'w' );
dx = [ 0, -1, -1, -1, 0, 1, 1, 1 ];
dy = [ -1, -1, 0, 1, 1, 1, 0, -1 ];

%Matrix for storing the result of LDP
ldpCode = zeros( r, c );

%For storing the 8 sorted masked values
stemp = zeros( 8,1);

%For storing masked value for each mask
temp = zeros( 8,1 );

%Mask Position
mr = 2;
mc = 2;
K = 3;
localWindow = 4;
ratio = r / localWindow;

%Loop for each window
for l = 1:localWindow
    %Local Window row start and end
    sr = ( ratio * ( l - 1 ) ) + 1;
    er = ratio * l ;
    for col = 1:localWindow
        %Local Window column start and end
        sc = ( ratio * ( col - 1 ) ) + 1;
        ec = ratio * col ;
        his = zeros( 1, 256 );
        cnt = 0;
        %Loops for a single local window
        for i = sr:er
            for j = sc:ec
                %Loop for selecting Kirsch Masks
                for k = 1:8
                    sum = 0;
                    %Loop for direction in each Mask
                    for z = 1:8
                        nr = i + dx( z );
                        nc = j + dy( z ) ;
                        if( nr >= 1 && nr <= r && nc >= 1 && nc <= c ) 
                            % nr and nc for image and mr and mc for mask
                            sum = sum + ( var2( nr,nc ) * kmask( mr + dx( z ) , mc + dy( z ) , k ) ) ;
                        end

                    end
                    temp( k,1 ) = sum;

                end
                stemp = sort( temp );
                s = 0;
                for z = 1:8
                    temp( z , 1 ) = temp( z, 1 ) - stemp( 3 );
                    if( temp(z,1) > 0 ) 
                        temp(z,1) = 1;
                    else temp( z,1) = 0;
                    end
                    %Need to check again
                    s = s + ( ( 2 .^ ( z - 1 ) )  * temp( z, 1 ) ) ;

                end
                ldpCode( i,j ) = s ;
                his( 1, ldpCode( i,j ) + 1 ) = his( 1, ldpCode( i,j ) + 1 ) + 1;
                
                % Count for each pixel of a window
                cnt = cnt + 1;

            end
        end
%         for in = 1:256
%             fprintf(fid, '%d ', his( in, 1 ) ) ;
%         end
        %disp(size(his));
        result = [result his];
    end
end
%ldpCode = uint8( ldpCode );
%figure,imshow( ldpCode );



