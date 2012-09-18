package Simulation3D;

import java.awt.*;
import javax.swing.*;

//This class is used as the painting screen
@SuppressWarnings("serial")
public class TreeDFrame extends JPanel {
	
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

	public TreeDFrame(){
		_shapes[0] = new MultiSurface();
		Point3D a = new Point3D(100,-100,-100);
		Point3D b = new Point3D(-100,-100,-100);
		Point3D c = new Point3D(-100,100,-100);
		Point3D d = new Point3D(100,100,-100);
		Point3D e = new Point3D(100,-100,100);
		Point3D f = new Point3D(-100,-100,100);
		Point3D g = new Point3D(-100,100,100);
		Point3D h = new Point3D(100,100,100);
	
		
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
		front.setColor(Color.RED);
		

		up.setName("up-CYAN");
		dowen.setName("dowen-MAGENTA");
		front.setName("front-BLUE");
		back.setName("back-GREEN");
		right.setName("right-RED");
		left.setName("left-YELLO");
	
		
		_shapes[0].setSurfaces(up,dowen,front,back,left,right);
		_shapes[0].setZoom(1000);
		Point3D position = new Point3D(1000,0,0);
		_shapes[0].setPosition(position);
	
		
		
	}
	
	
	public void paintComponent( Graphics g ){
		super.paintComponent( g );
		this.setBackground(Color.LIGHT_GRAY);
		_shapes[0].draw(g,getWidth(),getHeight());
		
	}
	
	public void setAngles(double pitch,double roll,double yaw){
		_shapes[0].setAngles(pitch,- roll, yaw);
		this.repaint();
	}
	
	public void setPosition(int x,int y,int z){
		_shapes[0].setPosition(x, y, z);
		this.repaint();
	}
}
