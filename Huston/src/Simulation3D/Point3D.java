package Simulation3D;


// These class represents 3D point
public class Point3D implements Cloneable{
	//data members
	private double _x;
	private double _y;
	private double _z;
	
	//methods
	public Point3D(){
		_x = 0;
		_y = 0;
		_z = 0;
	}
	public Point3D(double x,double y,double z){
		setPoint(x,y,z);
	}
	public void setPoint(double x,double y,double z){
		_x = x;
		_y = y;
		_z = z;	
	}
	
	public void setX(double x){
		_x = x;
	}
	
	public void setY(double y){
		_y = y;
	}
	
	public void setZ(double z){
		_z = z;
	}
	
	public double getX(){
		return _x;
	}
	public double getY(){
		return _y;
	}
	public double getZ(){
		return _z;
	}
	
	public double getDistance(Point3D position){
		double x = _x + position.getX();
		double y = _y + position.getY();
		double z = _z + position.getZ();
		return Math.sqrt(x*x + y*y + z*z);
	}
	
	@Override
	public String toString()
	{
		return String.format("Point3D(%0.2f,%0.2f,%0.2f)",getX(),getY(),getZ());
	}
	
	//rotate by Euiler angles with respect to the origin.
	// The input is the point at the body frame, the output is the point at the inertial frame.
	public Point3D rotate(double pitch,double roll,double yaw)
	{
		//define the matrix R (the rotation matrix) to rotate the point. 
		// Pi = R*Pb
		//    pitch = teta, roll = phi, yaw = psi.
		//Rmatrix=[cos(teta)*cos(psi) ,sin(phi)*sin(teta)*cos(psi)-cos(phi)*sin(psi) ,cos(phi)*sin(teta)*cos(psi)+sin(phi)*sin(psi)
		//         cos(teta)*sin(psi) ,sin(phi)*sin(teta)*sin(psi)+cos(phi)*cos(psi) ,cos(phi)*sin(teta)*sin(psi)-sin(phi)*cos(psi)
		//          -sin(teta)        ,sin(phi)*cos(teta)                            , cos(phi)*cos(teta)];

		double x = (Math.cos(pitch)*Math.cos(yaw))*this.getX() + (Math.sin(roll)*Math.sin(pitch)*Math.cos(yaw)-Math.cos(roll))*Math.sin(yaw)*this.getY() +
				(Math.cos(roll)*Math.sin(pitch)*Math.cos(yaw)+Math.sin(roll)*Math.sin(yaw))*this.getZ();
		double y = (Math.cos(pitch)*Math.sin(yaw))*this.getX() + (Math.sin(roll)*Math.sin(pitch)*Math.sin(yaw)+Math.cos(roll)*Math.cos(yaw))*this.getY() +
				(Math.cos(roll)*Math.sin(pitch)*Math.sin(yaw)-Math.sin(roll)*Math.cos(yaw))*this.getZ();
		double z = (-Math.sin(pitch))*this.getX() + (Math.sin(roll)*Math.cos(pitch))*this.getY() + (Math.cos(roll)*Math.cos(pitch))*this.getZ();
		Point3D rotatedPoint = new Point3D(x,y,z);
		return rotatedPoint;

	}
	
	@Override
	public Object clone() {
		Point3D point = new Point3D(this.getX(),this.getY(),this.getZ());
		return point;
	}


}
