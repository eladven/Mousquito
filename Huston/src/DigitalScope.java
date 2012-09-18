
import java.awt.Dimension;
import java.awt.GridLayout;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JTextField;

@SuppressWarnings("serial")
public class DigitalScope extends JPanel implements DataListener{

	private final static int NUM_OF_FIELDS = 5;
	private final int DATA_SIZE = 36; 
	private String [] _labels = new String[DATA_SIZE];
	private JTextField [] _texts = new JTextField[NUM_OF_FIELDS];
	private JComboBox<String> [] _lists = new JComboBox[NUM_OF_FIELDS];


	public DigitalScope(){

		setLayout(new GridLayout(NUM_OF_FIELDS,2,5,2));
		for (int i=0;i<DATA_SIZE;i++){
			_labels[i] = "data "+i;
		}
		
		for (int i=0;i<NUM_OF_FIELDS;i++){
			_texts[i] = new JTextField("");
			_texts[i].setEditable(false);
			_lists[i] = new JComboBox<String>(_labels);
			add(_texts[i]);
			add(_lists[i]);
		}
		
		_lists[0].setSelectedIndex(9);
		_lists[1].setSelectedIndex(10);
		_lists[2].setSelectedIndex(11);
		_lists[3].setSelectedIndex(20);
		_lists[4].setSelectedIndex(20);

		
	}

	public Dimension getPreferredSize(){
		return new Dimension(400,200);
	}

	public Dimension getMinimumSize(){
		return getPreferredSize();
	}



	@Override
	public void handleCommand(String[] operands) {
		if (operands[0].equals("GETDATALEBEL") ){
			int index;
			try{
				index = Integer.parseInt(operands[1]);		
			}
			catch (Exception e){
				e.printStackTrace();
				return;
			}
			if ( (index>=0) && (index < _labels.length) &&(operands.length>=2) ){
				_labels[index] = operands[2];
				for( int i=0;i<NUM_OF_FIELDS;i++)
					_lists[i].setModel(new DefaultComboBoxModel<String>(_labels));
				_lists[0].setSelectedIndex(9);
				_lists[1].setSelectedIndex(10);
				_lists[2].setSelectedIndex(11);
				_lists[3].setSelectedIndex(20);
				_lists[4].setSelectedIndex(20);
			}
		}

	}

	@Override
	public void handleData(int[] data) {
		for (int i=0;i<NUM_OF_FIELDS;i++){
			_texts[i].setText(""+data[_lists[i].getSelectedIndex()]);
		}
	}

}

