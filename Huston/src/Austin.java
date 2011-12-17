import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JDesktopPane;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;

// This is ground station for the Mosquito project.
// created by Elad Venezian and Maor Mevorach.


@SuppressWarnings("serial")
public class Austin extends JFrame{
	
	Terminal _terminal = new Terminal();
	DataInterpater _interapter = new DataInterpater();
	
	JPanel _guiTermilal = new GUITerminal(_terminal);
	JPanel _graphPanel = new GraphPanel();
	
	//GUI components 
	private JDesktopPane _deskTop = new JDesktopPane();
	
	public Austin(){
		super("Austin Ground Station");
		setSize(getMaximumSize());
		setVisible(true);
		
		//functional
		_terminal.addPortListener((PortListener)_guiTermilal);
		_terminal.addNumericPortListener((NumericPortListener)_interapter);
		
		_interapter.addDataListener((DataListener)_graphPanel);
		
		
		add(_deskTop);
		
		// define the menu bar:
		JMenu fileMenu = new JMenu("File");
		fileMenu.setMnemonic('F');
		
		JMenuItem exitItem = new JMenuItem("Exit");
		exitItem.setMnemonic('x');
		fileMenu.add(exitItem);
		exitItem.addActionListener( new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				System.exit(0);
				
			}
		});
		
		
		JMenu commandMenu = new JMenu("Command");
		commandMenu.setMnemonic('C');
		
		JMenuItem startSendDataItem = new JMenuItem("start send data");
		startSendDataItem.setMnemonic('s');
		commandMenu.add(startSendDataItem);
		startSendDataItem.addActionListener( new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				_terminal.write("syncoutcoded 1");
				Thread labelGetter = new Thread(new LabelGetter());
				labelGetter.start();
			}
		});
		
		JMenuItem stopSendDataItem = new JMenuItem("stop send data");
		stopSendDataItem.setMnemonic('p');
		commandMenu.add(stopSendDataItem);
		stopSendDataItem.addActionListener( new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				_terminal.write("syncoutcoded 0");
				
			}
		});
		
		CreateFrameHandler createTerminalHandler = new CreateFrameHandler("Terminal",_guiTermilal);
		CreateFrameHandler createGraphHandler = new CreateFrameHandler("Mosquito Data",_graphPanel);
		
		JMenu windowMenu = new JMenu("Window");
		windowMenu.setMnemonic('W');
		
		JCheckBoxMenuItem terminalItem = new JCheckBoxMenuItem("terminal");
		terminalItem.setMnemonic('t');
		windowMenu.add(terminalItem);
		terminalItem.addItemListener( createTerminalHandler );
		
		JCheckBoxMenuItem graphItem = new JCheckBoxMenuItem("graph");
		graphItem.setMnemonic('g');
		windowMenu.add(graphItem);
		graphItem.addItemListener( createGraphHandler );
		
		JMenuBar bar = new JMenuBar();
		setJMenuBar(bar);
		bar.add(fileMenu);
		bar.add(commandMenu);
		bar.add(windowMenu);
		
		//open the needed frames:
		graphItem.setSelected(true);
		terminalItem.setSelected(true);
		
		
	}
	
	private class CreateFrameHandler implements ItemListener {
		private boolean _isFrameCreated = false;
		private JCheckBoxMenuItem _sourceItem;
		private String _title;
		private JPanel _panel;
		private JInternalFrame _newFrame;
		public CreateFrameHandler(String title,JPanel panel){
			_title = title;
			_panel = panel;
		}
		@Override
		public void itemStateChanged(ItemEvent event) {
			_sourceItem = (JCheckBoxMenuItem)event.getItem();
			
			if (!_isFrameCreated && _sourceItem.isSelected()){
				_isFrameCreated = true;
				CreateFrame();
				_sourceItem.setSelected(true);
			}
			if (_isFrameCreated && !_sourceItem.isSelected()){
				_sourceItem.setSelected(true);
			}
			if (_newFrame != null) {// put the frame at the front
				_newFrame.toFront();
				_newFrame.repaint();
			}
		
		}
		private void CreateFrame(){
			_newFrame = 
					new JInternalFrame(_title,true,true,true);
			_newFrame.setVisible(true);	
			_deskTop.add(_newFrame);
			_newFrame.add(_panel);
			_newFrame.setVisible(true);	
			_newFrame.pack();
			_newFrame.addInternalFrameListener(new InternalFrameAdapter() {
				@Override
				public void internalFrameClosed(InternalFrameEvent arg0) {
					_isFrameCreated = false;
					_sourceItem.setSelected(false);

					
				}	
			});
		}
	}
	
	

	private class LabelGetter implements Runnable {
		
		public void run (){
			for(int i=0;i<32;i++){
				_terminal.write("getdatalabel "+i);
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// do nothing.
				}
			}
		}
	}
	
	public static void main(String [] args){
		Austin austin = new Austin();
		austin.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
	

