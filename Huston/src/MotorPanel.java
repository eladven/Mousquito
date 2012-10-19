import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTabbedPane;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;


@SuppressWarnings("serial")
public class MotorPanel extends JPanel implements ChangeListener,ActionListener{
	private static final int NUM_OF_MOTORS = 4;
	private JSlider _motorSliders [] = new JSlider[NUM_OF_MOTORS];
	private JSlider _stickSliders [] = new JSlider[4];
	private JTextField _texts [] = new JTextField[NUM_OF_MOTORS];
	private JCheckBox _isMotorContolBox = new JCheckBox("Menual motors control ");
	private JCheckBox _isVirtualStickBox = new JCheckBox("Virtual stick control ");
	private Terminal _terminal;

	public MotorPanel(Terminal terminal){
		JTabbedPane tabbedPane = new JTabbedPane();
		JPanel motorPanel = new JPanel();
		_terminal = terminal;
		JPanel slidersPanel = new JPanel(new GridLayout(1,NUM_OF_MOTORS,50,30));
		for (int i=0;i<NUM_OF_MOTORS;i++){
			_motorSliders[i] = new JSlider(SwingConstants.VERTICAL,0,249,0);
			_motorSliders[i].setPaintTicks(true);
			_motorSliders[i].setPaintLabels(true);
			_motorSliders[i].setPaintTrack(true);
			_motorSliders[i].setMajorTickSpacing(50);
			_texts[i] = new JTextField("0");
			JPanel panel = new JPanel(new BorderLayout());
			panel.add(_motorSliders[i]);
			panel.add(_texts[i],BorderLayout.NORTH);
			panel.add(new JLabel("Motor "+i),BorderLayout.SOUTH);
			slidersPanel.add(panel);
			_motorSliders[i].addChangeListener(this);
			_texts[i].addActionListener(this);
		}
		motorPanel.setLayout(new BorderLayout());
		motorPanel.add(slidersPanel);
		motorPanel.add(_isMotorContolBox,BorderLayout.SOUTH);
		
		tabbedPane.addTab("MotorPanel",null, motorPanel,"MotorPanel");
		
		/////////////////////////////// virtual joystick //////////////////////
		JPanel stickPanel = new JPanel(new BorderLayout());
		tabbedPane.addTab("virtualJoystick",null, stickPanel,"virtualJoystick");
		JPanel stickSlidersPanel = new JPanel(new GridLayout(1,NUM_OF_MOTORS,50,30));
		for (int i=0;i<NUM_OF_MOTORS;i++){
			_stickSliders[i] = new JSlider(SwingConstants.VERTICAL,-128,127,0);
			_stickSliders[i].setPaintTicks(true);
			_stickSliders[i].setPaintLabels(true);
			_stickSliders[i].setPaintTrack(true);
			_stickSliders[i].setMajorTickSpacing(50);
			stickSlidersPanel.add(_stickSliders[i]);
			_stickSliders[i].addChangeListener(this);
		}
		stickPanel.add(stickSlidersPanel,BorderLayout.CENTER);
		stickPanel.add(_isVirtualStickBox,BorderLayout.SOUTH);
		///////////////////////////////////////////////////////////////////////
		add(tabbedPane);
		
		_isMotorContolBox.addActionListener( this);
		_isVirtualStickBox.addActionListener( this);
		
		
	}

	public Dimension getPreferredSize(){
		return new Dimension(350,300);
	}

	public Dimension getMinimumSize(){
		return getPreferredSize();
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		for (int i=0;i<NUM_OF_MOTORS;i++)
			if (_motorSliders[i] == e.getSource()){
				_texts[i].setText("" + ((JSlider)e.getSource()).getValue() );
				if (_isMotorContolBox.isSelected()){
					_terminal.write("motor "+i+" "+((JSlider)e.getSource()).getValue());
				}
			}
		for (int i=0;i<4;i++)
			if (_stickSliders[i] == e.getSource()){
				_texts[i].setText("" + ((JSlider)e.getSource()).getValue() );
				if (_isVirtualStickBox.isSelected()){
					_terminal.write("setcontrol "+i+" "+((JSlider)e.getSource()).getValue());
				}
			}
		
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		for (int i=0;i<NUM_OF_MOTORS;i++)
			if (_texts[i] == e.getSource())
				try{
				_motorSliders[i].setValue(Integer.parseInt(_texts[i].getText()) );
				} catch (NumberFormatException e2) {
					_texts[i].setText("0" );
				} catch (ClassCastException e2) {
					_texts[i].setText("0" );
				}
		
		if (_isMotorContolBox == e.getSource()){

			if (_isMotorContolBox.isSelected()){
				_terminal.write("menualcontrol 1");
			}else
				_terminal.write("menualcontrol 0");
		}
		
		else if (_isVirtualStickBox == e.getSource()){
			if (_isVirtualStickBox.isSelected()){
				_terminal.write("menualcontrol 2");
			}else
				_terminal.write("menualcontrol 0");
		}
		
	}
	




}

