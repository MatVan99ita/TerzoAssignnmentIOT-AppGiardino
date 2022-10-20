plugins {
    // Apply the java-library plugin to add support for Java Library
    java
}

repositories {
    mavenCentral()
}

dependencies {

    implementation ("io.vertx:vertx-core:4.2.6")
    implementation ("io.vertx:vertx-web:4.2.6")
    implementation ("io.vertx:vertx-web-client:4.2.6")
    implementation ("io.vertx:vertx-mqtt:4.2.6")

    // Use JUnit test framework
    testImplementation ("junit:junit:4.13.2")

	/* for logging */
	implementation ("org.slf4j:slf4j-api:1.7.25")
	implementation ("org.slf4j:slf4j-jdk14:1.7.36")

    implementation ("com.ngrok:ngrok-api-java:0.6.0")
	

}
