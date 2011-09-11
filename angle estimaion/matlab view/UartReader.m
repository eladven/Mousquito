clear all;
close all;

phi =pi*0;  %define Euiler angel as 0;
teta=pi*0;
psi=pi*0;

    C=instrfind('Port','COM10');
    if (~isempty(C))
        fclose(C);
    end
    delete (C);
    
    C = serial('COM10');
    set(C,'BaudRate',115200);
    set(C,'FlowControl','hardware')
    set(C,'InputBufferSize',10000);
    fopen(C);
angle = zeros(5,1);
disp('Starting');
index = 1;


% now ther is section that need to get a key pressed, and update the angale
% state.
but='n';
while (but  ~= 'q')    %% loop and read chars 
  %  [xi,yi,but] = ginput(1);
    figure(1);
   
    w=fread(C,5);
    p = w(1)-100
     t = w(2)-100
     ps = w(3)-100

      printe(p,t,ps) ;
end
close all
fclose(C);

