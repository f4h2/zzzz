package com.IOT.IOT_system.repository;

import com.IOT.IOT_system.model.RFID;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.Optional;

public interface RfidRepository extends JpaRepository<RFID,Integer> {
    @Query(value = "SELECT * FROM RFID WHERE IDx=?",nativeQuery = true)
    Optional<RFID> findByIDx(String IDx);
    @Query(value = "DELETE FROM RFID WHERE IDx = ?",nativeQuery = true)
    Optional<RFID> deleteRFIDBy(String IDx);
}