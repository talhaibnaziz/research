function [ result ] = LDP( img )
img = imresize( img, [100 100 ]);
%figure,imshow( img );
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
ldpCode = zeros( r, c );
stemp = zeros( 8,1);
temp = zeros( 8,1 );
mr = 2;
mc = 2;
K = 3;
localWindow = 5;
ratio = r / localWindow;

for l = 1:localWindow
    sr = ( ratio * ( l - 1 ) ) + 1;
    er = ratio * l ;
    for col = 1:localWindow
        sc = ( ratio * ( col - 1 ) ) + 1;
        ec = ratio * col ;
        his = zeros( 256,1 );
        cnt = 0;
        for i = sr:er
            for j = sc:ec
                for k = 1:8
                    sum = 0;
                    for z = 1:8
                        nr = i + dx( z );
                        nc = j + dy( z ) ;
                        if( nr >= 1 && nr <= r && nc >= 1 && nc <= c ) 
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
                    s = s + ( ( 2 .^ ( z - 1 ) )  * temp( z, 1 ) ) ;

                end
                ldpCode( i,j ) = s ;
                his( ldpCode( i,j ) + 1, 1 ) = his( ldpCode( i,j ) + 1, 1 ) + 1;
                cnt = cnt + 1;

            end
        end
%         for in = 1:256
%             fprintf(fid, '%d ', his( in, 1 ) ) ;
%         end
    end
end
ldpCode = uint8( ldpCode );
%figure,imshow( ldpCode );
result=ldpCode;



