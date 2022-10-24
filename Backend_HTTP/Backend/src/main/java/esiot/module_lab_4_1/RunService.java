package esiot.module_lab_4_1;

import io.vertx.core.Vertx;

/*
 * Data Service as a vertx event-loop 
 */
public class RunService {


	public static void main(String[] args) {
		Vertx vertx = Vertx.vertx();
		DataService service = new DataService(443);
		vertx.deployVerticle(service);
	}
}