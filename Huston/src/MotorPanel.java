import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;


@SuppressWarnings("serial")
public class MotorPanel extends JPanel implements ChangeListener,ActionListener{
	private static final int NUM_OF_MOTORS = 4;
	private JSlider _sliders [] = new JSlider[NUM_OF_MOTORS];
	private JTextField _texts [] = new JTextField[NUM_OF_MOTORS];
	private JCheckBox _checkBox = new JCheckBox("Menual motors control ");
	private Terminal _terminal;

	public MotorPanel(Terminal terminal){
		_terminal = terminal;
		setLayout(new BorderLayout());
		JPanel slidersPanel = new JPanel(new GridLayout(1,NUM_OF_MOTORS,50,30));
		for (int i=0;i<NUM_OF_MOTORS;i++){
			_sliders[i] = new JSlider(SwingConstants.VERTICAL,0,249,0);
			_sliders[i].setPaintTicks(true);
			_sliders[i].setPaintLabels(true);
			_sliders[i].setPaintTrack(true);
			_sliders[i].setMajorTickSpacing(50);
			_texts[i] = new JTextField("0");
			JPanel panel = new JPanel(new BorderLayout());
			panel.add(_sliders[i]);
			panel.add(_texts[i],BorderLayout.NORTH);
			panel.add(new JLabel("Motor "+i),BorderLayout.SOUTH);
			slidersPanel.add(panel);
			_sliders[i].addChangeListener(this);
			_texts[i].addActionListener(this);
		}
		add(slidersPanel);
		add(_checkBox,BorderLayout.SOUTH);
		_checkBox.addActionListener( new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				for (int i=0;i<NUM_OF_MOTORS;i++){
					_terminal.write("motor "+i+" 0");
					_sliders[i].setValue(0);
				}
				
				if (_checkBox.isSelected()){
					_terminal.write("menualcontrol 1");
				}else
					_terminal.write("menualcontrol 0");
				for (int i=0;i<NUM_OF_MOTORS;i++){
					_terminal.write("motor "+i+" 0");
					_sliders[i].setValue(0);
				}
			}
		});
		
		
	}

	public Dimension getPreferredSize(){
		return new Dimension(400,200);
	}

	public Dimension getMinimumSize(){
		return getPreferredSize();
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		for (int i=0;i<NUM_OF_MOTORS;i++)
			if (_sliders[i] == e.getSource()){
				_texts[i].setText("" + ((JSlider)e.getSource()).getValue() );
				if (_checkBox.isSelected()){
					_terminal.write("motor "+i+" "+((JSlider)e.getSource()).getValue());
				}
			}
		
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		for (int i=0;i<NUM_OF_MOTORS;i++)
			if (_texts[i] == e.getSource())
				try{
				_sliders[i].setValue(Integer.parseInt(_texts[i].getText()) );
				} catch (NumberFormatException e2) {
					_texts[i].setText("0" );
				} catch (ClassCastException e2) {
					_texts[i].setText("0" );
				}
		
	}
	




}

