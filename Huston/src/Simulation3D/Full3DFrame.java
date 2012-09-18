package Simulation3D;

import java.awt.*;
import java.util.Arrays;

import javax.swing.*;

//This class is used as the painting screen
@SuppressWarnings("serial")
public class Full3DFrame extends JPanel {
	double _pitch,_roll,_yaw;
	
	public MultiSurface [] _shapes = new MultiSurface[2];
	//            ^
	//    -> x    |	y   . z
	//	
	//	    down:   
	//	c            d
	//		
	//	b            a
	//
	//		up:
	//	g            h
	//		 
	//	f            e

	public Full3DFrame(){
		_shapes[0] = new MultiSurface();
		Point3D a = new Point3D(200.0,-200.0,-200.0);
		Point3D b = new Point3D(-200.0,-200.0,-200.0);
		Point3D c = new Point3D(-200.0,200.0,-200.0);
		Point3D d = new Point3D(200.0,200.0,-200.0);
		Point3D e = new Point3D(200.0,-200.0,200.0);
		Point3D f = new Point3D(-200.0,-200.0,200.0);
		Point3D g = new Point3D(-200.0,200.0,200.0);
		Point3D h = new Point3D(200.0,200.0,200.0);
	
		
		Surface up = new Surface(e,f,g,h);
		//up.setColor(Color.CYAN);
		up.setColor(Color.BLUE);
		Surface dowen = new Surface(a,b,c,d);
		//dowen.setColor(Color.MAGENTA);
		dowen.setColor(Color.BLUE);
		Surface right = new Surface(a,b,f,e);
		//right.setColor(Color.YELLOW);
		right.setColor(Color.BLUE);
		Surface left = new Surface(c,d,h,g);
		//left.setColor(Color.RED);
		left.setColor(Color.BLUE);
		Surface back = new Surface(e,h,d,a);
		//back.setColor(Color.GREEN);
		back.setColor(Color.BLUE);
		Surface front = new Surface(f,g,c,b);
		front.setColor(Color.BLUE);
		

		up.setName("up-CYAN");
		dowen.setName("dowen-MAGENTA");
		front.setName("front-BLUE");
		back.setName("back-GREEN");
		right.setName("right-RED");
		left.setName("left-YELLO");
	
		
		_shapes[0].setSurfaces(up,dowen,front,back,left,right);
		_shapes[0].setZoom(1000);
		Point3D position = new Point3D(1000.0,0.0,0.0);
		_shapes[0].setPosition(position);
		
		
		_shapes[1] = new MultiSurface();
		Point3D a2 = new Point3D(250.0,-250.0,0.0);
		Point3D b2 = new Point3D(-250.0,-250.0,0.0);
		Point3D c2 = new Point3D(-250.0,250.0,0.0);
		Point3D d2 = new Point3D(250.0,250.0,0.0);
		Point3D e2 = new Point3D(0.0,0.0,200.0);
	
		Surface dowen2 = new Surface(a2,b2,c2,d2);
		dowen2.setColor(Color.RED);
		Surface s1 = new Surface(a2,b2,e2);
		s1.setColor(Color.RED);
		Surface s2 = new Surface(b2,c2,e2);
		s2.setColor(Color.RED);
		Surface s3 = new Surface(c2,d2,e2);
		s3.setColor(Color.RED);
		Surface s4 = new Surface(d2,a2,e2);
		s4.setColor(Color.RED);
		
		_shapes[1].setSurfaces(dowen2,s1,s2,s3,s4);
		_shapes[1].setZoom(1000);
		position.setPoint(1000.0, 0.0, 200.0);
		_shapes[1].setPosition(position);
	
	
		
	}
	
	
	public void paintComponent( Graphics g ){
		super.paintComponent( g );
		this.setBackground(Color.LIGHT_GRAY);
		Arrays.sort(_shapes);
		for (int i=0;i<_shapes.length;i++)
			_shapes[i].draw(g,getWidth(),getHeight());	
	}
	
	public void setAngles(double pitch,double roll,double yaw){
		for (int i=0;i<_shapes.length;i++)
			_shapes[i].setAngles(pitch, roll, yaw);
		
	}
	
	public void movePosition(double x,double y,double z){
		for (int i=0;i<_shapes.length;i++)
			_shapes[i].movePosition(-x,-y,-z);
		this.repaint();
	}
	
	public void moveForward(double step){
		Point3D Xdir = new Point3D(100,0,0);
		Xdir = Xdir.rotate(_pitch, _roll, _yaw);
		//this.movePosition( (Xdir.getX()*step)/100, (Xdir.getY()*step)/100, (Xdir.getZ()*step)/100);
		this.movePosition(step, 0, 0);
	}
	
	public void moveYaw(double yaw){
		_yaw += yaw;
		for (int i=0;i<_shapes.length;i++){
			_shapes[i].moveAngles(0, 0, +yaw);
			double r = Math.sqrt( Math.pow(_shapes[i].getPosition().getX(),2) + Math.pow(_shapes[i].getPosition().getY(),2) );
			_shapes[i].setPosition((int)(r*Math.cos(_yaw)),
					(int)(r*Math.sin(_yaw)),_shapes[i].getPosition().getZ());
		}
		this.repaint();
	}
}
