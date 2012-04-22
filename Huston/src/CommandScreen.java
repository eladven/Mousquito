import java.awt.GridLayout;

import javax.swing.JPanel;
//import javax.swing.JTextArea;
//import javax.swing.JTextField;


@SuppressWarnings("serial")
public class CommandScreen extends JPanel implements DataListener{
	GraphPanel _graphPanel = new GraphPanel();
	
	public CommandScreen(){
	//	JTextField output = new JTextField();
	//	JTextArea screen = new JTextArea();
		setLayout(new GridLayout(1,2));
		
		//add(output);
		//add(screen);
		add(_graphPanel);
		
	}

	@Override
	public void handleData(int[] data) {
		_graphPanel.handleData(data);		
	}

	@Override
	public void handleCommand(String[] operands) {
		_graphPanel.handleCommand(operands);
		
	}

}
