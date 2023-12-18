package com.main.iot.model;

import java.util.Map;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

public class Fan {
    public boolean status;
    public boolean auto;
    public int level;
    public int maxTemp;
    public int maxHum;

    public Fan(String jsonData) {
        try {
            ObjectMapper mapper = new ObjectMapper();
            var data = mapper.readValue(jsonData, Map.class);
            //
            status = data.get("status").equals(1);
            auto = data.get("auto").equals(1);
            maxTemp = (int) data.get("maxTemp");
            maxHum = (int) data.get("maxHum");
            level = (int) data.get("level");
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
    }
}
