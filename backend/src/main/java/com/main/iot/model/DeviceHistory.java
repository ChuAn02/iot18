package com.main.iot.model;

import java.sql.Date;
import jakarta.persistence.*;

@Table(name = "device")
@Entity
public class DeviceHistory {
	@Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    public int id;
    public Date date;
    public float time;
    public String type;
}
