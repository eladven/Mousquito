function [x]=calcArray(z) 
x = zeros(12,1);
max = 2^15;
concatBinString = '';
for index = 9:40
    temp = dec2bin(z(index) - 'A',8);
    temp =temp(3:8);
    concatBinString = [concatBinString  temp];
end
for index = 1:12
    x(index) = bin2dec(concatBinString( (index)*16-15:index*16 ) ) ;   
    if (x(index)  > max)
        x(index) = x(index) -  2*max;
    end
end
