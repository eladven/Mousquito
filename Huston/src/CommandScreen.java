
import javax.swing.JPanel;
//import javax.swing.JTextArea;
//import javax.swing.JTextField;


@SuppressWarnings("serial")
public class CommandScreen extends JPanel implements DataListener{
	private GraphPanel _graphPanel = new GraphPanel();
	private CommunicationScreen _comScreen;
	
	public CommandScreen(Terminal terminal){
		add(_graphPanel);
		_comScreen = new CommunicationScreen(terminal);
		add(_comScreen);
		
	}

	@Override
	public void handleData(int[] data) {
		_graphPanel.handleData(data);		
	}

	@Override
	public void handleCommand(String[] operands) {
		_graphPanel.handleCommand(operands);
		for (String operand:operands)
			_comScreen.appendScreen(operand);
		
	}
	
	public String getLabel(int i){
		return _graphPanel.getLabel(i);
	}
	
	public int getNumOfLabels(){
		return _graphPanel.getNumOfLabels();
	}

}
