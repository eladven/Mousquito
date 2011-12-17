import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.LinkedList;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.JTextField;




@SuppressWarnings("serial")
public class GraphPanel extends JPanel implements DataListener{
	private final int DATA_SIZE = 36; 
	private final int DATA_WIDTH = 200; // new data available at 20 Hz, 200 is 20 sec. 
	private Graph _graph = new Graph();
	private JCheckBox [] _chooseData = new JCheckBox[DATA_SIZE]; 
	private Color [] _colors = new Color[DATA_SIZE]; 
	private ArrayList<LinkedList<Integer>>  _dataArrays = 
			new ArrayList<LinkedList<Integer>>();
	private static final int LABEL_GAP = 20;
	private double [] _factories = new double [DATA_SIZE];
	private JTextField _indexField = new JTextField("Enter index");
	private JTextField _factorField = new JTextField("Enter factor");
	
	public GraphPanel(){
		// build matrix of  DATA_SIZE x DATA_WIDTH initialized with zeros.
		for (int i=0;i<DATA_SIZE;i++){
			_dataArrays.add(new LinkedList<Integer>() );
			_factories[i] = 1.0;
		}
		for (int i=0;i<_dataArrays.size();i++){
			for (int j=0;j<DATA_WIDTH;j++)
				_dataArrays.get(i).add(0);
		}
		setLayout(new BorderLayout());
		add(_graph,BorderLayout.CENTER);
		
		_colors[0] = new Color(250,0,70);
		_colors[1] = new Color(250,0,150);
		_colors[2] = new Color(250,0,200);
		_colors[3] = new Color(250,90,70);
		_colors[4] = new Color(250,90,150);
		_colors[5] = new Color(250,90,200);
		_colors[6] = new Color(250,150,70);
		_colors[7] = new Color(250,150,150);
		_colors[8] = new Color(250,150,200);
		_colors[9] = Color.RED;
		_colors[10] =Color.BLUE;
		_colors[11] = Color.GREEN;
		_colors[12] = new Color(0,250,70);
		_colors[13] = new Color(100,250,70);
		_colors[14] = new Color(150,250,70);
		_colors[15] = new Color(200,250,70);
		_colors[16] = new Color(250,50,250);
		_colors[17] = new Color(250,100,250);
		_colors[18] = new Color(250,150,250);
		_colors[19] = new Color(250,200,250);
		for (int i=20;i<_colors.length;i++)
			_colors[i] = Color.GRAY;
		
		JPanel choosedataPanel = new JPanel();
		choosedataPanel.setLayout(new GridLayout(0,7));
		for (int i=0;i<DATA_SIZE;i++){
			_chooseData[i] = new JCheckBox("data "+i);
			_chooseData[i].setForeground(_colors[i]);
			choosedataPanel.add(_chooseData[i]);
		}
		add(choosedataPanel,BorderLayout.SOUTH);
		_chooseData[9].setSelected(true);
		_chooseData[10].setSelected(true);
		_chooseData[11].setSelected(true);
		
		JButton changeFactor = new JButton("Change Factor");
		choosedataPanel.add(changeFactor);
		
		choosedataPanel.add(_indexField);
		choosedataPanel.add(_factorField);
		changeFactor.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				int index = 0;
				try{
					index = Integer.parseInt( _indexField.getText());
					index  = ((index>=0) && (index < _factories.length))?index:0;
					if (!_factorField.getText().equals("")){
						double factor = Double.parseDouble(_factorField.getText());
						_factories[index] = factor;
					}
				}catch (Exception e) {
					// do nothing
				}
				_indexField.setText("" + index);
				_factorField.setText("" + _factories[index]);
				
			}
		});
		
	}
	
	@Override
	public void handleData(int[] data) {
		// for each field , implement FIFO policy.
		for (int i=0;i<_dataArrays.size();i++){
			_dataArrays.get(i).addFirst(data[i]);
			_dataArrays.get(i).removeLast();
		}
		_graph.repaint();
	}
	
	public Dimension getPreferredSize(){
		return new Dimension(1000,1000);
	}
	
	public Dimension getMinimumSize(){
		return getPreferredSize();
	}
	
	
	private class Graph extends JPanel{
		@Override
		public void paintComponent(Graphics g){
			super.paintComponent(g);
			int width = getWidth()-LABEL_GAP;
			int hight = getHeight()-LABEL_GAP;
			double interval = ((double)width)/DATA_WIDTH;
			
			setBackground(Color.BLACK);
			g.setColor(Color.DARK_GRAY);
			for (int i=-18;i<19;i+=2){
				g.drawLine(LABEL_GAP,hight/2-  hight*i*10/360, width,hight/2-  hight*i*10/360);
				g.drawString(""+(i*10), 0, hight/2-  hight*i*10/360);
			}
			for (int i=0;i<DATA_WIDTH;i+=10){
				g.drawLine(LABEL_GAP+(int)(i*interval),0,
						LABEL_GAP+(int)(i*interval),hight);
				g.drawString(""+i, LABEL_GAP+(int)(i*interval), hight+LABEL_GAP/2);
			}
			
			for (int i=0;i<DATA_SIZE;i++){
				if (!_chooseData[i].isSelected())
					continue;
				g.setColor(_colors[i]);
				for (int j=0;j<DATA_WIDTH-1;j++){
					g.drawLine(LABEL_GAP+(int)(j*interval),
							(int)(hight/2- (_factories[i]*hight*_dataArrays.get(i).get(j)/360) )
							, LABEL_GAP+(int)((j+1)*interval),
							(int)(hight/2- (_factories[i]*hight*_dataArrays.get(i).get(j+1)/360)) );
					
				}
			}
		}//end paintComponent
		
		
	}//end class Graph

	// empty method. used to implement DataListener interface.
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
			if ( (index>=0) && (index < _chooseData.length) &&(operands.length>=2) )
				_chooseData[index].setText(operands[2]);
		}
		
	}
	
}
