package HTTP;

public class Communication implements CommChannel {

    @Override
    public void sendMsg(String msg) {

    }

    @Override
    public String receiveMsg() throws InterruptedException {
        return null;
    }

    @Override
    public boolean isMsgAvailable() {
        return false;
    }
}
