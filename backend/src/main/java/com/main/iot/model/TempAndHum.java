package com.main.iot.model;

import java.util.Map;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

public class TempAndHum {
    public double temp;
    public double hum;

    public TempAndHum(String jsonData) {
        try {
            ObjectMapper mapper = new ObjectMapper();
            var data = mapper.readValue(jsonData, Map.class);
            //
            temp = (double) data.get("temp");
            hum = (double) data.get("hum");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
    }

}
