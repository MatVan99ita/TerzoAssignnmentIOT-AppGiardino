package SerialCommunicator;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.jar.Attributes.Name;
import java.util.stream.IntStream;

import javax.lang.model.element.Element;

import SerialCommunicator.SerialCommChannel;
import jssc.SerialPortList;


public class LogicsImpl implements Logics {

	private static final String REFILL = "Refill";
	private static final String REPAIR = "Repair";
	private int selfTestCount;
	private String status;
	private String operation;
	private Map<String, BeverageImpl> beverages;
	private CommChannel channel;
	private String arduinoMsg;
	
	public LogicsImpl(String args) throws Exception {
		channel = new SerialCommChannel(args, 9600);
		
		this.beverages = Map.of(
				"Chocolate", new BeverageImpl("Chocolate"),
				"Tea", new BeverageImpl("Tea"),
				"Coffee", new BeverageImpl("Coffee"));
		resetSelfTestCount();
		setStatus("Avvio");
	}
	
	@Override
	public void SendChange(String status) throws Exception {
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");
		channel.sendMsg(REFILL);
	}
	
	@Override
	public void getChanges() throws Exception {
		System.out.println("Waiting Arduino for rebooting...");	
		Thread.sleep(4000);
		System.out.println("Ready.");
		int index = 0;
		this.arduinoMsg = channel.receiveMsg();
		Thread.sleep(500);
		String[] msgParsed = this.arduinoMsg.split("-");
		System.out.println("Received: " + this.arduinoMsg.toString());
		
		try {
			if(msgParsed.length == 5) {
				for (String string : msgParsed) {
					System.out.println(string);
					if(msgParsed.equals("")) { //Controllo su elementi che non vengono inviati
						getChanges();
					}
				}
				this.beverages.get("Chocolate").setQuantity(Integer.parseInt(msgParsed[0]));
				this.beverages.get("Tea").setQuantity(Integer.parseInt(msgParsed[1]));
				this.beverages.get("Coffee").setQuantity(Integer.parseInt(msgParsed[2]));
				this.selfTestCount = Integer.parseInt(msgParsed[3]);
				this.status = msgParsed[4];
			} else {
				getChanges();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	@Override
	public int getSelfTestCount() {
		return this.selfTestCount;
	}

	@Override
	public void setSelfTestCount(int selfTestCount) {
		this.selfTestCount = selfTestCount;
	}

	@Override
	public int getSpecifiedBeverageCount(String beverage) throws Exception{
		return this.beverages.get(beverage).getQuantity();
	}
	

	@Override
	public String getStatus() {
		return this.status;
	}

	@Override
	public void makeBevarage(final String beverage) {
		this.beverages.get(beverage).makeBeverage();
	}

	@Override
	public void resetBeverageCount() {
		this.beverages.forEach((s, bev) -> bev.refill());
	}

	@Override
	public void resetSelfTestCount() {
		this.selfTestCount = 0;
	}

	@Override
	public void setStatus(String status) {
		this.status = status;
	}

	@Override
	public void servoRotate() throws Exception {
		// TODO Auto-generated method stub
		int angle = 0;
		/* attesa necessaria per fare in modo che Arduino completi il reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");
		
		//Rotazione singola per sistemare in caso di problemi
		for(int i = 0; i < 180; i++) {
			channel.sendMsg(""+i);
			String msg = channel.receiveMsg();
			System.out.println(msg);		
			Thread.sleep(100);
		}
	}
	
}
