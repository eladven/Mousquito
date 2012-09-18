package Simulation3D;

import java.awt.Graphics;
import java.util.Arrays;

// This class represent 3D object, which composed of surfaces (e.g. cube prism ...)

public class MultiSurface  implements Comparable<MultiSurface>{
	//data members
	private Surface [] _surfaces;
	private Surface [] _rotatedsurfaces;
	private double  _pitch,_roll,_yaw;
	private Point3D _position = new Point3D(10000,0,0);
	private int _zoom;
	
	//methods
		MultiSurface(Surface... surfaces){
			setSurfaces(surfaces);
			
	}
		
	public int getZoom() {
		return _zoom;
	}

	public void setZoom(int zoom) {
		this._zoom = zoom;
	}

	
	public void setSurfaces(Surface... surfaces){
		_surfaces = new Surface[surfaces.length];
		for (int i=0;i<surfaces.length;i++){
			_surfaces[i] = surfaces[i];
			_surfaces[i].setPosition(_position);
		}
		_rotatedsurfaces = new Surface[_surfaces.length];
	}
	
	public void setPosition(double x,double y,double z){
		_position.setPoint(x, y, z);
		for (Surface surface : _surfaces)
			surface.setPosition(_position);
	}
	
	public void setPosition(Point3D position){
		_position = (Point3D)position.clone();
		for (Surface surface : _surfaces)
			surface.setPosition(_position);
	}
	
	public void movePosition(Point3D position){
		_position.setX( position.getX() + _position.getX() );
		_position.setY( position.getY() + _position.getY());
		_position.setZ( position.getZ() + _position.getZ());
		for (Surface surface : _surfaces)
			surface.setPosition(_position);
	}
	
	public void movePosition(double x,double y,double z){
		System.out.println("here position " + _position);
		System.out.println("position "+ _position);
		setPosition(x + _position.getX(),y + _position.getY() ,z + _position.getZ());
		for (Surface surface : _surfaces)
			surface.setPosition(_position);
		System.out.println("here(2) position " + _position +"\n\n");
	}
	
	public Point3D getPosition(){
		return _position;
	}
	
	public void setAngles(double pitch,double roll,double yaw){
		_pitch = pitch;
		_roll = roll;
		_yaw = yaw;
	}
	
	public void moveAngles(double pitch,double roll,double yaw){
		_pitch += pitch;
		_roll += roll;
		_yaw += yaw;
	}
	
	public void draw( Graphics g,int width,int height){
		// rotate the surface from the body frame (his cordinates ) to inertial frame (the observer cordinates).
		rotate(_pitch,_roll,_yaw);
		// draw the surfaces from the farthest to the closest so closer surface will hide farer surface.
		Arrays.sort(_rotatedsurfaces);

		// draw each surface on the screen
		for (int i=0;i<_rotatedsurfaces.length;i++)
		{
			// project the 3D body to 2D screen
			int [] x = new int[_rotatedsurfaces[i].getNuberOfPoints()];
			int [] y = new int[_rotatedsurfaces[i].getNuberOfPoints()];
			for (int j=0;j<x.length;j++){
				//calculate the angle that the observer will measure. i.e the x,y cordinate of the screen 
				x[j]=width/2+(int)(_zoom* Math.atan2( ( _rotatedsurfaces[i].getPoint(j).getY()+_position.getY()),( _rotatedsurfaces[i].getPoint(j).getX()+_position.getX())) );   
				y[j]=height/2-(int)(_zoom*Math.atan2( ( _rotatedsurfaces[i].getPoint(j).getZ()+_position.getZ()),( _rotatedsurfaces[i].getPoint(j).getX()+_position.getX())) );
			}
			// draw the polygon that represents the surface.
			g.setColor(_rotatedsurfaces[i].getColor());
			g.fillPolygon(x, y, x.length);
			g.setColor(_rotatedsurfaces[i].getEdgeColor());
			g.drawPolygon(x, y, y.length);
		}
	}
	
	//rotate by Euiler angles with respect to the origin.
	private void rotate(double pitch,double roll,double yaw){
		for(int i=0;i<_surfaces.length;i++) // move on each surface
			_rotatedsurfaces[i] = _surfaces[i].rotate(pitch, roll, yaw);
	}
	
	@Override
	public int compareTo(MultiSurface obj) {
		MultiSurface multiSurface = (MultiSurface)obj;
		Point3D position = new Point3D(0,0,0);
		if ( this.getPosition().getDistance(position)  <  multiSurface.getPosition().getDistance(position) ) 
				return 1;
		else  if ( this.getPosition().getDistance(position)  >  multiSurface.getPosition().getDistance(position) ) 
			return -1;
		return 0;
	}
	

}
