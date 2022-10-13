package SerialCommunicator;


public interface Logics {
	/**
	 * Funzione per l'invio della richiesta di assitenza
	 * @param status
	 * @throws Exception
	 */
	void SendChange(String status) throws Exception;
	
	void servoRotate() throws Exception;

	public int getSelfTestCount();

	public void setSelfTestCount(int selfTestCount);
	
	/**
	 * 0: Cioccolata <br>
	 * 1: Caffè <br>
	 * 2: Tè <br>
	 */
	public int getSpecifiedBeverageCount(String beverage) throws Exception ;

	public String getStatus();
	
	public void makeBevarage(String beverage);
	
	public void resetBeverageCount();
	
	public void resetSelfTestCount();
	
	/**
	 * Modificato dall'arduino
	 */
	public void setStatus(String status);

	void getChanges() throws Exception;

}
