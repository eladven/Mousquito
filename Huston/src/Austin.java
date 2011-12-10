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
	
	//GUI components 
	private JDesktopPane _deskTop = new JDesktopPane();
	
	public Austin(){
		super("Austin Ground Station");
		setSize(getMaximumSize());
		setVisible(true);
		
		//functional
		_terminal.addPortListener((PortListener)_guiTermilal);
		_terminal.addNumericPortListener((NumericPortListener)_interapter);
		
		
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
		
		JMenu windowMenu = new JMenu("Window");
		fileMenu.setMnemonic('W');
		
		JCheckBoxMenuItem terminalItem = new JCheckBoxMenuItem("terminal");
		terminalItem.setMnemonic('t');
		windowMenu.add(terminalItem);
		terminalItem.addItemListener( new ItemListener() {
			private boolean _isFrameCreated = false;
			private JCheckBoxMenuItem _sourceItem;
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
				
			
			}
			private void CreateFrame(){
				JInternalFrame terminalFrame = 
						new JInternalFrame("Terminal",true,true,true);
				terminalFrame.setVisible(true);	
				_deskTop.add(terminalFrame);
				terminalFrame.add(_guiTermilal);
				terminalFrame.setVisible(true);	
				terminalFrame.pack();
				terminalFrame.addInternalFrameListener(new InternalFrameAdapter() {
					@Override
					public void internalFrameClosed(InternalFrameEvent arg0) {
						_isFrameCreated = false;
						_sourceItem.setSelected(false);

						
					}	
				});
			}
		});
		
		JMenuBar bar = new JMenuBar();
		setJMenuBar(bar);
		bar.add(fileMenu);
		bar.add(windowMenu);
		
		
	}
	
	public static void main(String [] args){
		Austin austin = new Austin();
		austin.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		/*
		Terminal terminal = new Terminal();
		System.out.println(terminal.getAvailablePorts() );
		terminal.setPort("COM9");
	
		terminal.connect();

		
		try
		{                
			int c = 0;
			while ( ( c = System.in.read()) > -1 )
			{
				if (c=='d')
					terminal.disconnect();
				if (c=='c')
					terminal.connect();
				
				terminal.write(c);
			}                
		}
		catch ( IOException e )
		{
			e.printStackTrace();
		} 
		terminal.disconnect();
		*/
	}
}
	

