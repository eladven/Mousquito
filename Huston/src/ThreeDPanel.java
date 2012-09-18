import java.awt.BorderLayout;
import java.awt.Dimension;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.SwingConstants;

import Simulation3D.TreeDFrame;


@SuppressWarnings("serial")
public class ThreeDPanel extends JPanel implements DataListener{
	private TreeDFrame _tdPanel = new TreeDFrame();
	private JSlider _slider = new JSlider(SwingConstants.HORIZONTAL,-180,180,0);

	
	public ThreeDPanel(){
		setLayout(new BorderLayout());
		add(_tdPanel);	
		_slider.setPaintTicks(true);
		_slider.setPaintLabels(true);
		_slider.setPaintTrack(true);
		_slider.setMajorTickSpacing(20);
		add(_slider,BorderLayout.SOUTH);
	}
	
	public Dimension getPreferredSize(){
		return new Dimension(400,400);
	}

	public Dimension getMinimumSize(){
		return getPreferredSize();
	}

	@Override
	public void handleData(int[] data) {
		int pitch = data[9];
		int roll = data[10];
		int yaw = data[11];
		_tdPanel.setAngles(pitch*Math.PI/180, roll*Math.PI/180, (yaw+ _slider.getValue())*Math.PI/180 );
		
	}

	@Override
	public void handleCommand(String[] operands) {
		// TODO Auto-generated method stub
		
	}

}
