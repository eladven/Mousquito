import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;


@SuppressWarnings("serial")
public class CommunicationScreen extends JPanel{
	private JTextArea _inputScreen = new JTextArea();
	private JTextField _outputScreen = new JTextField();
	private Terminal _terminal;

	public CommunicationScreen(Terminal terminal){
		_terminal = terminal;
		setLayout(new GridLayout(2,1));
		add(new JScrollPane(_inputScreen));
		_inputScreen.setBackground(new Color(0,40,140));
		_inputScreen.setFont(new Font("MonoSpace", Font.PLAIN, 16));
		_inputScreen.setForeground(new Color(170,190,230));
		_inputScreen.setEditable(false);
		
		add(new JScrollPane(_outputScreen));
		_outputScreen.setBackground(new Color(0,40,140));
		_outputScreen.setFont(new Font("MonoSpace", Font.PLAIN, 16));
		_outputScreen.setForeground(new Color(170,190,230));
		_outputScreen.setEditable(true);
		
		_outputScreen.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				_inputScreen.append(_outputScreen.getText()+"\n");  //when the user press enter , send the massage and show it.
				_terminal.write(_outputScreen.getText());
				_outputScreen.setText("");
			}
		});
		
	}
	
	public Dimension getPreferredSize(){
		return new Dimension(400,300);
	}
	
	public Dimension getMinimumSize(){
		return getPreferredSize();
	}
	
	public void appendScreen(String msg){
		_inputScreen.append(msg+" ");
		_inputScreen.setCaretPosition(_inputScreen.getText().length()); // Scroll to the bottom
	}
}



