package com.main.iot.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.*;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonMappingException;
import jakarta.annotation.PostConstruct;
import com.main.iot.connection.MqttConnection;
import com.main.iot.model.DeviceHistory;
import com.main.iot.model.Light;
import com.main.iot.repository.DeviceRepository;
import java.sql.Date;
import java.util.Calendar;
import java.util.Collections;
import java.util.List;
import static java.nio.charset.StandardCharsets.UTF_8;

@CrossOrigin
@RestController
@RequestMapping("/api/light")
public class LightController {
    private MqttConnection lightConn;
    @Autowired
    private SimpMessagingTemplate template;
    @Autowired
    private DeviceRepository rep;

    @PostConstruct
    public void init() {
        lightConn = new MqttConnection();
        lightConn.subscribe("/info/light");
        lightConn.subscribe("/light/save");
        lightConn.setCallback(publish -> {
            String topic = publish.getTopic().toString();
            if (topic.equals("/info/light")) {
                Light light = new Light(UTF_8.decode(publish.getPayload().get()).toString());
                template.convertAndSend("/topic/light", light);
                return;
            }
            if (topic.equals("/light/save")) {
                DeviceHistory deviceHistory = new DeviceHistory();
                deviceHistory.date = new Date(Calendar.getInstance().getTime().getTime());
                deviceHistory.type = "light";
                deviceHistory.time = Float.parseFloat(UTF_8.decode(publish.getPayload().get()).toString());
                saveData(deviceHistory);
                return;
            }
        });
    }

    @GetMapping("")
    public Light getLightData() throws InterruptedException, JsonMappingException, JsonProcessingException {
        lightConn.publish("/info", "light");
        String jsonData = lightConn.getMessage();
        return new Light(jsonData);
    }

    @PostMapping("/status/{stt}")
    public void turn(@PathVariable("stt") String stt) {
        lightConn.publish("/light/status", stt);
    }

    @PostMapping("/brightness/{value}")
    public void changeBrightness(@PathVariable("value") int value) {
        lightConn.publish("/light/brightness", "" + value);
    }

    @PostMapping("/auto/status/{stt}")
    public void auto(@PathVariable("stt") String stt) {
        lightConn.publish("/light/auto/status", stt);
    }

    @PostMapping("/schedule/status/{stt}")
    public void enableSchedule(@PathVariable("stt") String stt) {
        lightConn.publish("/light/schedule/status", stt);
    }

    @PostMapping("/schedule")
    public void setSchedule(@RequestBody String data) {
        lightConn.publish("/light/schedule", data.substring(1, data.length() - 1).replace("\\", ""));
    }

    @GetMapping("/chart")
    public List<DeviceHistory> getChartData() {
        var lst = rep.getDeviceHistories("light");
        Collections.reverse(lst);
        return lst;
    }

    @SendTo("/topic/light")
    public Light updateData(@Payload Light light) {
        return light;
    }

    public void saveData(DeviceHistory data) {
        rep.save(data);
    }
}
