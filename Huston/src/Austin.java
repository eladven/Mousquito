import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ContainerEvent;
import java.awt.event.ContainerListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JDesktopPane;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;
import javax.swing.event.InternalFrameListener;

// This is ground station for the Mosquito project.
// created by Elad Venezian and Maor Mevorach.


@SuppressWarnings("serial")
public class Austin extends JFrame{
	private JDesktopPane _deskTop = new JDesktopPane();
	
	public Austin(){
		super("Austin Ground Station");
		//setSize(getMaximumSize());
		setSize(300,400);
		setVisible(true);
		
		
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
				terminalFrame.setVisible(true);	
				//terminalFrame.pack();
				terminalFrame.setSize(100,100);
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
		
		Terminal terminal = new Terminal();
		Austin austin = new Austin();
		austin.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		System.out.println(terminal.getAvailableports() );
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
	}
}
	

