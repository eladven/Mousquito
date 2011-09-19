
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


angles = zeros(3,100);
angle = zeros(3,1);
z = zeros(1,100);
zero = zeros(1,100);
while (1)    
    w=fread(C,1);
    z(index) = w;
  
    if ((index > 4) && (z(index) == 'L')   && (z(index-1) == 'D')&& (z(index-2) == 'N') && (z(index-3) == 'E'))
        index = 1;
        x =calcArray(z); 
         angle(1) = x(1);
         angle(2) = x(2);
         angle(3)= x(3);
         
           angles = [angle angles(:,1:99)];
       figure(1);
       subplot(2,1,1); 
      printe(angle(1),angle(2),angle(3)) ;
      subplot(2,1,2); 
      plot(zero,'y');
      hold on
      plot(angles(1,:));
      plot(angles(2,:),'r');
      plot(angles(3,:),'g');
      hold off
    else
          index = index+1;
    end
 
   
        
end

fclose(C);

