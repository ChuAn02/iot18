package com.main.iot.controller;

import jakarta.annotation.PostConstruct;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.main.iot.connection.MqttConnection;
import com.main.iot.model.TempAndHum;
import static java.nio.charset.StandardCharsets.UTF_8;

@CrossOrigin
@RestController
@RequestMapping("/api/temphum")
public class TempAndHumController {
    private MqttConnection temphumConn;
    @Autowired
    private SimpMessagingTemplate template;

    @PostConstruct
    public void init() {
        temphumConn = new MqttConnection();
        temphumConn.subscribe("/info/temphum");
        temphumConn.setCallback(publish -> {
            TempAndHum temphum = new TempAndHum(UTF_8.decode(publish.getPayload().get()).toString());
            template.convertAndSend("/topic/temphum", temphum);
        });
    }

    @GetMapping("")
    public TempAndHum getTempAndHum() throws InterruptedException, JsonMappingException, JsonProcessingException {
        temphumConn.publish("/info", "temphum");
        String jsonData = temphumConn.getMessage();
        return new TempAndHum(jsonData);
    }

    @SendTo("/topic/temphum")
    public TempAndHum updateData(@Payload TempAndHum temphum) {
        return temphum;
    }
}
