
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;


@SuppressWarnings("serial")
public class GUITerminal extends JPanel implements PortListener{
	private Terminal _terminal;  // reference to terminal.
	//GUI components
	private JComboBox<String> _availablePortsCombo;
	private JTextArea _screen = new JTextArea("");
	private JTextField _input = new JTextField (10);
	private JButton _connectButton = new JButton("Connect");
	
	private int MAX_SCREEN_SIZE = 2000;

	// constructor.
	public GUITerminal(Terminal terminal){
		// set the network.
		_terminal = terminal; 
		
		//set the GUI display
		setVisible(true);
		ArrayList<String> availablePortsArray = _terminal.getAvailablePorts();
		_availablePortsCombo = 
				new JComboBox<String>(availablePortsArray.
						toArray(new String[availablePortsArray.size()] ));
		setLayout(new BorderLayout());
		add(new JScrollPane(_screen),BorderLayout.CENTER);
		_screen.setEditable(false);
		add(_input,BorderLayout.NORTH);
		JPanel controlPanel = new JPanel();
		add(controlPanel,BorderLayout.SOUTH);
		controlPanel.add(_connectButton);
		controlPanel.add(_availablePortsCombo);
		_connectButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				if (!_terminal.isConnected()){
					_terminal.setPort((String)_availablePortsCombo.getSelectedItem());
					if (_terminal.connect() ) {
						_connectButton.setText("Disconnect");
					}
				} 
				else {
					_terminal.disconnect();
					_connectButton.setText("Connect");
				}	
			}
		});
		
		_input.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				
				_screen.append(_input.getText()+"\n");  //when the user press enter , send the massage and show it.
				if (_screen.getText().length() > MAX_SCREEN_SIZE)
					_screen.replaceRange("",0,
							_screen.getText().length()-MAX_SCREEN_SIZE); //remove the start of the string.
				_screen.setCaretPosition(_screen.getText().length()); // Scroll to the bottom
				for(int i=0;i<_input.getText().length();i++)
					_terminal.write(_input.getText().charAt(i));
				_terminal.write(13); // write enter at the end of the command.
				_input.setText("");
			}
		});
		
	} // end of constructor.

	// implement PortListener interface
	public void handleInput(String input){
			_screen.append(input); 
			if (_screen.getText().length() > MAX_SCREEN_SIZE)
				_screen.replaceRange("",0,
						_screen.getText().length()-MAX_SCREEN_SIZE); //remove the start of the string.
			_screen.setCaretPosition(_screen.getText().length()); // Scroll to the bottom

	}
	
	public Dimension getPreferredSize(){
		return new Dimension(300,300);
	}
	
	public Dimension getMinimumSize(){
		return getPreferredSize();
	}

}
