package com.main.iot.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.web.bind.annotation.*;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonMappingException;
import jakarta.annotation.PostConstruct;
import static java.nio.charset.StandardCharsets.UTF_8;
import java.sql.Date;
import java.util.Calendar;
import java.util.Collections;
import java.util.List;
import com.main.iot.connection.MqttConnection;
import com.main.iot.model.DeviceHistory;
import com.main.iot.model.Fan;
import com.main.iot.repository.DeviceRepository;

@CrossOrigin
@RestController
@RequestMapping("/api/fan")
public class FanController {
    private MqttConnection fanConn;
    @Autowired
    private SimpMessagingTemplate template;
    @Autowired
    private DeviceRepository rep;

    @PostConstruct
    public void init() {
        fanConn = new MqttConnection();
        fanConn.subscribe("/info/fan");
        fanConn.subscribe("/fan/save");
        fanConn.setCallback(publish -> {
            String topic = publish.getTopic().toString();
            if (topic.equals("/info/fan")) {
                Fan fan = new Fan(UTF_8.decode(publish.getPayload().get()).toString());
                template.convertAndSend("/topic/fan", fan);
                return;
            }
            if (topic.equals("/fan/save")) {
                DeviceHistory deviceHistory = new DeviceHistory();
                deviceHistory.date = new Date(Calendar.getInstance().getTime().getTime());
                deviceHistory.type = "fan";
                deviceHistory.time = Float.parseFloat(UTF_8.decode(publish.getPayload().get()).toString());
                saveData(deviceHistory);
                return;
            }
        });
    }

    @GetMapping("")
    public Fan getFanData() throws InterruptedException, JsonMappingException, JsonProcessingException {
        fanConn.publish("/info", "fan");
        String jsonData = fanConn.getMessage();
        return new Fan(jsonData);
    }

    @PostMapping("/status/{stt}")
    public void turn(@PathVariable("stt") String stt) {
        fanConn.publish("/fan/status", stt);
    }

    @PostMapping("/setting")
    public void setAutoSetting(@RequestBody String data) {
        fanConn.publish("/fan/setting", data.substring(1, data.length() - 1).replace("\\", ""));
    }

    @PostMapping("/level/{value}")
    public void changeBrightness(@PathVariable("value") int value) {
        fanConn.publish("/fan/level", "" + value);
    }

    @PostMapping("/auto/status/{stt}")
    public void auto(@PathVariable("stt") String stt) {
        fanConn.publish("/fan/auto/status", stt);
    }

    @GetMapping("/chart")
    public List<DeviceHistory> getChartData() {
        var lst = rep.getDeviceHistories("fan");
        Collections.reverse(lst);
        return lst;
    }

    @SendTo("/topic/fan")
    public Fan updateData(@Payload Fan Fan) {
        return Fan;
    }

    public void saveData(DeviceHistory data) {
        rep.save(data);
    }
}
