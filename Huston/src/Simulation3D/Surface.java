package Simulation3D;

// This class represent surface at 3D space

import java.awt.*;
import java.util.Arrays;
import java.util.InputMismatchException;

public class Surface implements Comparable<Surface>{
	private Point3D [] _points;
	private Color _color;
	private Point3D _position = new Point3D(0,0,0);
	private String _name = "";
	private Color _edgeColor = Color.BLACK;
	
	public Surface(Point3D... points){
		setPoints(points);
	}
	
	public Surface(Point3D [] points,Color color){
		setPoints(points);
		setColor(color);
	}
	
	public Surface(double [] xCordinate,double [] yCordinate,double [] zCordinate,Color color){
		setPoints(xCordinate,yCordinate,zCordinate);
		setColor(color);
	}
	

	public void setPoints(Point3D [] points){
		if ( points.length < 3 ) 
			throw new InputMismatchException(); //surface must have at least three points.
		_points	= points;
	}
	
	public void setName(String name){
		_name = name;
	}
	
	public String getName(){
		return _name;
	}
	
	
	public void setPoints(double [] xCordinate,double [] yCordinate,double [] zCordinate){
		if ( (xCordinate.length != yCordinate.length) || (yCordinate.length != zCordinate.length) ||
				(xCordinate.length < 3) ) {
			// all points must have x,y,z cordinates, surface must have at least three points.
			throw new InputMismatchException(); 
				}
		_points = new Point3D[xCordinate.length];
		for (int i=0;i<xCordinate.length;i++){
			_points[i] = new Point3D(xCordinate[i],yCordinate[i],zCordinate[i]);
			
		}
	}
	
	public void setColor(Color color){
		_color = color;
	}
	
	public Point3D getPoint(int i) {
		return _points[i];
		
		
	}
	
	public Color getColor(){
		return _color;
	}
	
	public void setEdgeColor(Color color){
		_edgeColor = color;
	}
	
	public Color getEdgeColor(){
		return _edgeColor;
	}
	
	public int getNuberOfPoints(){
		return _points.length;
	}
	
	public void setPosition(double x,double y,double z){
		_position.setPoint(x, y, z);
	}
	
	public void setPosition(Point3D position){
		_position = position;
	}
	
	public Point3D getPosition(){
		return _position;
	}


	@Override
	public int compareTo(Surface obj) {
		Surface surface = (Surface)obj;
		double [] thisPointIndexes = this.getSrortedPointsDistanse();
		double [] surfacePointIndexes = surface.getSrortedPointsDistanse();
		for (int i=0;(i < thisPointIndexes.length) && ( i <surfacePointIndexes.length);i++) {
			if ( thisPointIndexes[i]  <  surfacePointIndexes[i] ) {
				return 1;
			}
			else  if ( thisPointIndexes[i]  >  surfacePointIndexes[i] ){
				return -1;
			}
		}
		return 0;
	}
	
	//rotate by Euiler angles with respect to the origin.
	public Surface rotate(double pitch,double roll,double yaw)
	{
		Point3D [] points = new Point3D[this.getNuberOfPoints()];
		for (int i=0;i<this.getNuberOfPoints();i++)  // move on every point inside the current surface and rotate it.
			points[i] = this.getPoint(i).rotate(pitch, roll, yaw);
		
		Surface rotatedSurface = new Surface(points);
		rotatedSurface.setColor(this.getColor());
		rotatedSurface.setPosition(this.getPosition());
		rotatedSurface.setName(this.getName());
		
		return rotatedSurface;
	}
	
	private double [] getSrortedPointsDistanse(){
		double [] srortedPointsDistanse = new double[this.getNuberOfPoints()];
		for (int i=0;i<this.getNuberOfPoints();i++)
			srortedPointsDistanse[i] = this.getPoint(i).getDistance(_position);
		
		Arrays.sort(srortedPointsDistanse);
		
		return srortedPointsDistanse;
	}

}
