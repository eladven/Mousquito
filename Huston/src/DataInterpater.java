
import java.util.ArrayList;
import java.util.Arrays;


public class DataInterpater implements NumericPortListener{
	
	private int _index = 0;
	private byte [] _prevChar = new byte[3];
	private final int RAW_DATA_SIZE = 36; //T + data (32) + ZZZ
	private final int DATA_SIZE = 12; //T + data (32) + ZZZ
	private byte [] _rawData = new byte[RAW_DATA_SIZE];
	private int [] _data = new int[DATA_SIZE];
	private ArrayList<DataListener> dataLitenersList = new ArrayList<DataListener>();
	
	public void addData(DataListener listener){
		dataLitenersList.add(listener);
	}
	
	public void removeData(DataListener listener){
		dataLitenersList.remove(listener);
	}
	

	private void handleNewData(){
		for (DataListener listener:dataLitenersList)
			listener.handleData(_data);
	}

	@Override
	public void handleInput(byte [] input) {
		for (int j=0;j<input.length;j++){ // move on the input
			for (int i=_prevChar.length-1;i>0;i--)  // remember the last chars
				_prevChar[i] = _prevChar[i-1];
			_prevChar[0] = input[j];
	
			if (_prevChar[2]=='O' && _prevChar[1]=='U' && _prevChar[0]=='T')// if it is start signal
				_index =0;
			if (_index<_rawData.length)
				_rawData[_index++] = _prevChar[0];
			if (_index == _rawData.length){
				_index++; 
				if (_prevChar[2]=='Z' && _prevChar[1]=='Z' && _prevChar[0]=='Z')// if it is stop signal
					convertRawData();
			}
			
		}
		
	}
	
	private void convertRawData(){
		System.out.println("new data: "+Arrays.toString(_rawData));
		String concatedBinary = "";
		for (int i=1;i<_rawData.length-3;i++){  // move on the raw data.
			// first, build string that represent each byte as 6 bits binary
			int data = (_rawData[i]<0)?(_rawData[i]+256):_rawData[i];
			data = data - 'A';
			String st = Integer.toBinaryString(data);
			// concat '0' so st will be at 6 size.
			while (st.length() <6)
				st = "0" +st;
			// concat it into big string.
			concatedBinary +=st;
		}
		// take any group of 8 bits, and convert it to one int.
		for (int j=0;j<DATA_SIZE;j++){ 
			_data[j] = Integer.parseInt(concatedBinary.substring(j*16, j*16+16), 2);
			_data[j] = (_data[j]>32768)?(_data[j]-65536):_data[j];
		}
		handleNewData();
	}

}
