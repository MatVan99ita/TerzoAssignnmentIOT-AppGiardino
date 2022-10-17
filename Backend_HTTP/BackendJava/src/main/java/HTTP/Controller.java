package HTTP;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.EventListener.*;
import java.util.SerialPortEventListener;

import SerialCommunicator.SerialCommChannel;
import jssc.*;

public class Controller {
    static final String MSG_REFILLED 	= "fi";
    static final String MSG_RECOVERED	= "re";

    SerialCommChannel channel;
    SmartCMView view;

    public Controller(String port, SmartCMView view, LogView logger) throws Exception {
        this.view = view;
        channel = new SerialCommChannel(port,115200);
        new MonitoringAgent(channel,view,logger).start();

        /* attesa necessaria per fare in modo che Arduino completi il reboot */

        System.out.println("Waiting Arduino for rebooting...");
        Thread.sleep(4000);
        System.out.println("Ready.");

    }

    public void notifyRefilled() {
        channel.sendMsg(MSG_REFILLED);
    }

    public void notifyRecovered() {
        channel.sendMsg(MSG_RECOVERED);
    }
}
