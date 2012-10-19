import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

import javax.swing.ButtonGroup;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JDesktopPane;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;

// This is ground station for the Mosquito project.
// created by Elad Venezian and Maor Mevorach.
// 123


@SuppressWarnings("serial")
public class Huston extends JFrame{
	
	private Terminal _terminal = new Terminal();
	private DataInterpater _interapter = new DataInterpater();
	
	private JPanel _guiTermilal = new GUITerminal(_terminal);
	private CommandScreen _commandScreen = new CommandScreen(_terminal);
	private DigitalScope _digitlScope = new DigitalScope();
	private MotorPanel _motorPanal  = new MotorPanel(_terminal);
	private ThreeDPanel _tdPanel = new ThreeDPanel();
	private FileHandler _filehandler = new FileHandler(_commandScreen);
	
	
	//GUI components 
	private JDesktopPane _deskTop = new JDesktopPane();
	
	public Huston(){
		super("Huston Ground Station");
		setSize(getMaximumSize());
		setVisible(true);
		
		try{  // set Nimbus-look-and-feel
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
			SwingUtilities.updateComponentTreeUI(this);
		}catch (Exception exception){exception.printStackTrace();}  // do noting. it's ok to don't have Nimbus look-and-feel.
		
		//functional
		_terminal.addPortListener((PortListener)_guiTermilal);
		_terminal.addNumericPortListener((NumericPortListener)_interapter);
		_interapter.addDataListener((DataListener)_commandScreen);
		_interapter.addDataListener((DataListener)_digitlScope);
		_interapter.addDataListener((DataListener)_tdPanel);
		_interapter.addDataListener((DataListener)_filehandler);
		
		
		add(_deskTop);
		
		// define the menu bar:
		JMenu fileMenu = new JMenu("File");
		fileMenu.setMnemonic('F');
		
		JMenuItem fileItem = new JMenuItem("File to save:");
		fileItem.setMnemonic('f');
		fileMenu.add(fileItem);
		fileItem.addActionListener( new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				_filehandler.setFileName();				
			}
		});
		
		JMenuItem exitItem = new JMenuItem("Exit");
		exitItem.setMnemonic('x');
		fileMenu.add(exitItem);
		exitItem.addActionListener( new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				_filehandler.finish();
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
		CreateFrameHandler createGraphHandler = new CreateFrameHandler("Mosquito Data",_commandScreen);
		CreateFrameHandler createDigitalScopeHandler = new CreateFrameHandler("Digital Scope",_digitlScope);
		CreateFrameHandler createMotorPanelHandler = new CreateFrameHandler("Motor panel",_motorPanal);
		CreateFrameHandler create3DHandler = new CreateFrameHandler("3D panel",_tdPanel);
		
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
		
		JCheckBoxMenuItem digitalItem = new JCheckBoxMenuItem("digital Scope");
		digitalItem.setMnemonic('p');
		windowMenu.add(digitalItem);
		digitalItem.addItemListener( createDigitalScopeHandler );
		
		JCheckBoxMenuItem motorItem = new JCheckBoxMenuItem("Motor Manual contro"); 
		motorItem.setMnemonic('p');
		windowMenu.add(motorItem);
		motorItem.addItemListener( createMotorPanelHandler );
		
		JCheckBoxMenuItem tdItem = new JCheckBoxMenuItem("3D panel"); 
		tdItem.setMnemonic('3');
		windowMenu.add(tdItem);
		tdItem.addItemListener( create3DHandler );
		
		JMenuBar bar = new JMenuBar();
		setJMenuBar(bar);
		bar.add(fileMenu);
		bar.add(commandMenu);
		bar.add(windowMenu);
		
		//open the needed frames:
		graphItem.setSelected(true);
		terminalItem.setSelected(true);
		
		JMenu recordMenu = new JMenu("Record");
		windowMenu.setMnemonic('R');
		
		String [] RecordingOptions = {"Record All","Record When Engion on","Stop Recording"};
		JRadioButtonMenuItem [] recordings = new JRadioButtonMenuItem[RecordingOptions.length];
		ButtonGroup recordingGroup = new ButtonGroup();
		RecordMenueHandler recordMenueHandler = new RecordMenueHandler();
		
		for (int i=0;i<RecordingOptions.length;i++){
			recordings[i] = new JRadioButtonMenuItem(RecordingOptions[i]);
			recordMenu.add(recordings[i]);
			recordingGroup.add(recordings[i]);
			recordings[i].addActionListener(recordMenueHandler);
		}
		recordings[2].setSelected(true);
		bar.add(recordMenu);
		
		
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
			for(int i=0;i<36;i++){
				_terminal.write("getdatalabel "+i);
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// do nothing.
				}
			}
		}
	}
	
	private class RecordMenueHandler implements ActionListener{
		@Override
		public void actionPerformed(ActionEvent e) {
			String itemLable =  ( (JRadioButtonMenuItem)(e.getSource()) ).getText();
			if ( itemLable == "Record All" )
				_filehandler.setRecordingMode(FileHandler.RecodingMode.ALWAYS);
			if ( itemLable == "Record When Engion on" )
				_filehandler.setRecordingMode(FileHandler.RecodingMode.ENGION_ON);
			if ( itemLable == "Stop Recording" )
				_filehandler.setRecordingMode(FileHandler.RecodingMode.STOP);
		}
	}
	
	public static void main(String [] args){
		Huston austin = new Huston();
		austin.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
	

