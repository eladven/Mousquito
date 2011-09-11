function [x]=printe(p,t,ps) 

   phi =p*pi/180;
  teta=t*pi/180;
   psi=ps*pi/180;
   
MY_POSITION=[7 0 0];   %define the position of the observer (place the camera)

% now lets place the 3d bar. the cordinats are at Fb system.
Bar_b = zeros(3,8);
Bar_b(:,1) = [ 0.7, 0.2,-0.05];                  % down:   
Bar_b(:,2) = [ 0.7,-0.2,-0.05];                  %          3            4
Bar_b(:,3) = [-0.7,-0.2,-0.05];                  %
Bar_b(:,4) = [-0.7, 0.2,-0.05];                  %          2            1
Bar_b(:,5) = [ 0.7, 0.2, 0.05 ];                  %  up:
Bar_b(:,6) = [ 0.7, -0.2,0.05 ];                  %       7             8
Bar_b(:,7) = [-0.7,-0.2, 0.05 ];                  %
Bar_b(:,8) = [-0.7,0.2,  0.05 ];                  %       6             5



    %define the matrix R (the rotation matrix) to rotate our bar.  we will rotate Fb as it showen 
    % at the inertial system Fi
    %
Rmatrix=[cos(teta)*cos(psi)      , sin(phi)*sin(teta)*cos(psi)-cos(phi)*sin(psi)          ,cos(phi)*sin(teta)*cos(psi)+sin(phi)*sin(psi);...
                      cos(teta)*sin(psi)      ,sin(phi)*sin(teta)*sin(psi)+cos(phi)*cos(psi)          ,cos(phi)*sin(teta)*sin(psi)-sin(phi)*cos(psi);...
                       -sin(teta)                       ,sin(phi)*cos(teta)                                                                       , cos(phi)*cos(teta)];
 
 
 Bar_i = zeros(3,8);
 for k=1:8
     Bar_i(:,k) = Rmatrix * Bar_b(:,k);
 end
 
 
 %now ,project the 3d bar, on a 2d (virtual screen)
 x=zeros(1,8);
 y=zeros(1,8);
 for k=1:8
     x(k)=atan( ( Bar_i(2,k)-MY_POSITION(2)) /  ( Bar_i(1,k)-MY_POSITION(1))  );   %calc the angle that the observer will mesure. i.e the x,y cordinate of the screen 
     y(k)=atan( ( Bar_i(3,k)-MY_POSITION(3)) /  ( Bar_i(1,k)-MY_POSITION(1))  );
 end
surface = zeros(6,3,5) ; %6 serfuces  , 2D + deph, 5 point that define the serfuce
serfuce(1,1:2,:) = [x(1) x(2) x(3) x(4) x(1);y(1) y(2) y(3) y(4) y(1) ];
serfuce(1,3,1) = Bar_i(1,1) +Bar_i(1,2) +Bar_i(1,3) +Bar_i(1,4);
serfuce(2,1:2,:) = [x(5) x(6) x(7) x(8) x(5);y(5) y(6) y(7) y(8) y(5) ];
serfuce(2,3,1) = Bar_i(1,5) +Bar_i(1,6) +Bar_i(1,7) +Bar_i(1,8);
serfuce(3,1:2,:) = [x(1) x(5) x(8) x(4) x(1);y(1) y(5) y(8) y(4) y(1) ];
serfuce(3,3,1) = Bar_i(1,1) +Bar_i(1,5) +Bar_i(1,8) +Bar_i(1,4);
serfuce(4,1:2,:) = [x(1) x(2) x(6) x(5) x(1);y(1) y(2) y(6) y(5) y(1) ];
serfuce(4,3,1) = Bar_i(1,1) +Bar_i(1,2) +Bar_i(1,6) +Bar_i(1,5);
serfuce(5,1:2,:) = [x(4) x(8) x(7) x(3) x(4);y(4) y(8) y(7) y(3) y(4) ];
serfuce(5,3,1) = Bar_i(1,4) +Bar_i(1,8) +Bar_i(1,7) +Bar_i(1,3);
serfuce(6,1:2,:) = [x(2) x(3) x(7) x(6) x(2);y(2) y(3) y(7) y(6) y(2) ];
serfuce(6,3,1) = Bar_i(1,2) +Bar_i(1,3) +Bar_i(1,7) +Bar_i(1,6);
%  %% sort the serfuces by they depth (the x value  )
  sort=zeros(2,6);
  for k=1:6
      sort(1,k)= serfuce(k,3,1);
      sort(2,k)=k;
  end

   for m=2:6
         key =    sort(1,m);
         index=m;
         k= m-1; 
         while( k > 0  &&  sort(1,k)> key)
                sort(1,k+1) = sort(1,k); 
                 sort(2,k+1) = sort(2,k); 
                 k = k-1; 
                 sort(1,k+1) = key;
                 sort(2,k+1) = index;
            end
   end
   
   
   
   
plot(0,0);
hold on
for k=1:6
      X= zeros(1,5);
      Y= zeros(1,5);
      for m=1:5
          X(m)  = serfuce(sort(2,k),1,m);
          Y(m)  = serfuce(sort(2,k),2,m);
      end
    fill(X,Y,sort(2,k))
end

 axis([-0.15 0.15  -0.15  0.15]);
 hold off


