package com.main.iot.connection;

import java.util.function.Consumer;
import com.hivemq.client.mqtt.MqttClient;
import com.hivemq.client.mqtt.mqtt5.Mqtt5BlockingClient;
import com.hivemq.client.mqtt.mqtt5.message.publish.Mqtt5Publish;
import static com.hivemq.client.mqtt.MqttGlobalPublishFilter.SUBSCRIBED;
import static java.nio.charset.StandardCharsets.UTF_8;

public class MqttConnection {
    private static final String username = "spring";
    private static final String password = "012345Aa";
    private static final String MQTT_SERVER_ADDRESS = "8e4fc9a27eb74ad9a56752fb668f38ba.s1.eu.hivemq.cloud";
    private static final int MQTT_SERVER_PORT = 8883;
    private Mqtt5BlockingClient conn;

    public MqttConnection() {
        conn = MqttClient.builder()
                .useMqttVersion5()
                .serverHost(MQTT_SERVER_ADDRESS)
                .serverPort(MQTT_SERVER_PORT)
                .sslWithDefaultConfig()
                .buildBlocking();
        conn.connectWith()
                .simpleAuth()
                .username(username)
                .password(password.getBytes())
                .applySimpleAuth()
                .send();
        // conn.toAsync().publishes(SUBSCRIBED, publish -> {
        //     System.out.println("Received message: " +
        //             publish.getTopic() + " -> " +
        //             UTF_8.decode(publish.getPayload().get()).toString());
        // });
    }
    public void setCallback(Consumer<Mqtt5Publish> callback){
        conn.toAsync().publishes(SUBSCRIBED, callback);
    }

    public void subscribe(String topic) {
        conn.subscribeWith()
                .topicFilter(topic)
                .send();
    }

    public void publish(String topic, String msg) {
        conn.publishWith()
                .topic(topic)
                .payload(UTF_8.encode(msg))
                .send();
    }

    public String getMessage() throws InterruptedException {
        return new String(conn.publishes(SUBSCRIBED).receive().getPayloadAsBytes());
    }
}