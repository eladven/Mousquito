import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;


@SuppressWarnings("serial")
public class CommandScreen extends JPanel{
	public CommandScreen(){
		JTextField output = new JTextField();
		JTextArea screen = new JTextArea();
		
		add(output);
		add(screen);
		
	}

}
