package com.main.iot.model;

import java.util.ArrayList;
import java.util.Map;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

public class Light {
    public boolean status;
    public boolean auto;
    public int brightness;
    public boolean schedule;
    public ArrayList<Integer> day;
    public String startTime;
    public String endTime;

    @SuppressWarnings("unchecked")
    public Light(String jsonData){
        try {
            ObjectMapper mapper = new ObjectMapper();
            var data = mapper.readValue(jsonData, Map.class);
            //
            status = data.get("status").equals(1);
            auto = data.get("auto").equals(1);
            schedule = data.get("schedule").equals(1);
            brightness = (int) data.get("brightness");
            startTime = data.get("startTime").toString();
            endTime = data.get("endTime").toString();
            day=(ArrayList<Integer>) data.get("day");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
    }
}
