package Simulation3D;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;


public class Run {
	private static double _pitch = 0;
	private static double _roll = 0;
	private static double _yaw = 0;
	private static int _x = 0;
	private static int _y = 0;
	private static int _z = 0;
	private static Full3DFrame drawingPanel = new Full3DFrame();
	public static void main(String[] args) {
		
		
		JFrame application = new JFrame();
		application.add(drawingPanel);
		application.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		application.setSize(1000,1000);
		application.setVisible(true);
		application.addKeyListener(
				new KeyListener() {
					
					@Override
					public void keyTyped(KeyEvent event) {
						
					}
					
					@Override
					public void keyReleased(KeyEvent event) {
						
					}
					
					@Override
					public void keyPressed(KeyEvent event) {
						//JOptionPane.showMessageDialog(null,"The Key code is "+
						//		KeyEvent.getKeyText(event.getKeyCode()));
						if (KeyEvent.getKeyText(event.getKeyCode()) == "Left")
							_roll += 0.05;
						if (KeyEvent.getKeyText(event.getKeyCode()) == "Right")
							_roll -= 0.05;
						if (KeyEvent.getKeyText(event.getKeyCode()) == "Up")
							_pitch += 0.05;
						if (KeyEvent.getKeyText(event.getKeyCode()) == "Down")
							_pitch -= 0.05;
						if (event.getKeyChar() == 'z'){
							drawingPanel.moveYaw(0.01);
						}
						if (event.getKeyChar() == 'x'){
							drawingPanel.moveYaw(-0.01);
						}
						
					
						if (event.getKeyChar() == 'r'){
							drawingPanel.moveForward(5);
						}
						if (event.getKeyChar() == 'c'){
							drawingPanel.moveForward(-5);
						}
						if (event.getKeyChar() == 'd'){
							_y += 1;
						}
						if (event.getKeyChar() == 'f'){
							_y -= 1;
						}
						if (event.getKeyChar() == 't'){
							_z += 1;
						}
						if (event.getKeyChar() == 'g'){
							_z -= 1;
						}
						//drawingPanel.setAngles(_pitch, _roll, _yaw);
						//drawingPanel.movePosition(_x, _y, _z);
					}
				}
			);
		

	}

}
