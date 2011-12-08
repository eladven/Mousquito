import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;


public class Terminal {
	// methods.
	private String _port; // the port name.
	private volatile boolean _isConnected;
	private int _baudrate = 115200;
	private OutputStream _outputStream;
	private InputStream _inputStream;
	private SerialPort _serialPort;
	private Thread _reader;

	public boolean isConnected(){
		return _isConnected;
	}

	public void setPort(String port){
		_port = port;
	}

	public String getPort(){
		return _port;
	}

	public void setBaudrate(int baudrate){
		_baudrate = baudrate;
	}

	public int getBaudrate(){
		return _baudrate;
	}

	// connect to the specify port.
	public void connect(){ 
		CommPortIdentifier portIdentifier;
		try {
			portIdentifier = CommPortIdentifier.getPortIdentifier(_port);
		} catch (NoSuchPortException e) {
			
			updateLog("connect()::ERROR -  NoSuchPortException as" + _port);
			return;
		}
		if ( portIdentifier.isCurrentlyOwned() ){
			updateLog("connect()::ERROR - Port is currently in use");
			return;
		}

		try {
			_serialPort = (SerialPort)portIdentifier.open(this.getClass().getName(),2000);
		} catch (PortInUseException e) {
			updateLog("connect()::ERROR - Port is currently in use");
			return;
		}

		try {
			_serialPort.setSerialPortParams(_baudrate,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
		} catch (UnsupportedCommOperationException e) {
			updateLog("connect()::ERROR - UnsupportedCommOperationException");
			disconnect();
			return;
		}

		try {
			_inputStream = _serialPort.getInputStream();
			_outputStream = _serialPort.getOutputStream();
			_reader = new Thread(new SerialReader(_inputStream));
			_reader.start();
			_isConnected = true;
			updateLog("CONNECT to "+_port);
		} catch (IOException e) {
			updateLog("connect()::ERROR - IOException");
			disconnect();
			return;
		}
	}     

	// disconnect form the port;
	public void disconnect(){
		try {
			_isConnected = false;
			_reader.join();
		} catch (InterruptedException e1) {
			updateLog("disconnect()::ERROR - InterruptedException");
		}catch (NullPointerException e) {
			updateLog("disconnect()::ERROR - NullPointerException");
		}
		try {
			_outputStream.close();
			_inputStream.close();
			_serialPort.close();
		} catch (IOException e) {
			updateLog("disconnect()::ERROR - IOException");
		}catch (NullPointerException e) {
			updateLog("disconnect()::ERROR - NullPointerException");
		}
		updateLog("DISCONNECT FROM "+_port);
	}


	// this methods update all clients log.
	private void updateLog(String newLine){
		System.out.println(newLine);
	}

	// this methods route the input to all clients.
	private void handleInput(String input){
		System.out.print(input);
	}

	// write char to the serial port
	public void write(int c){
		if (!_isConnected){
			updateLog("write()::ERROR - not connected");
			return;
		}
		try {
			_outputStream.write(c);
		} catch (IOException e) {
			updateLog("write()::ERROR - IOException");
			disconnect();
		}
	}
	
	// return list of all the available ports.
	public ArrayList<String> getAvailableports(){
		ArrayList<String>  portListString = new ArrayList<String>();
		@SuppressWarnings("unchecked")
		Enumeration<CommPortIdentifier> portList =  
			CommPortIdentifier.getPortIdentifiers();

		while (portList.hasMoreElements()) {
			CommPortIdentifier portId = (CommPortIdentifier) portList.nextElement();
			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				portListString.add(portId.getName());
			}
		}

		return portListString;
	}



	private class SerialReader implements Runnable {
		InputStream _in;

		public SerialReader ( InputStream in ){
			_in = in;
		}

		public void run (){
			byte[] buffer = new byte[1024];
			int len = -1;
			try{
				while (_isConnected && (( len = _in.read(buffer)) > -1) ){
					handleInput(new String(buffer,0,len));
				}
			}
			catch ( IOException e ){
				updateLog("SerialReader()::ERROR - IOException");;
			} 
		}
	}



}
