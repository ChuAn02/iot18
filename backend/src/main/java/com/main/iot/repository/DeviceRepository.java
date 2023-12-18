package com.main.iot.repository;

import java.util.List;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import com.main.iot.model.DeviceHistory;

public interface DeviceRepository extends JpaRepository<DeviceHistory, Integer> {
    @Query(nativeQuery = true, value = "SELECT * FROM device s WHERE s.type = :type ORDER BY date DESC LIMIT 30")
    public List<DeviceHistory> getDeviceHistories(@Param("type") String type);
}
