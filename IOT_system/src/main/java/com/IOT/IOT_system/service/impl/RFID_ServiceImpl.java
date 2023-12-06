package com.IOT.IOT_system.service.impl;

import com.IOT.IOT_system.model.RFID;
import com.IOT.IOT_system.repository.RfidRepository;
import com.IOT.IOT_system.service.RFID_Service;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

@Service
public class RFID_ServiceImpl implements RFID_Service {

    private final RfidRepository DataRepository;


    public RFID_ServiceImpl(
            RfidRepository DataRepository) {
        this.DataRepository = DataRepository;
    }

    public RFID save(RFID data) {
        var saved = DataRepository.save(data);
//        stateCacheService.addToCache(new StateDataCache(saved));
        return saved;
    }
    public List<RFID> listAll() {
        return DataRepository.findAll();
    }
    public Optional<RFID> deleteRFID(String IDx){

        return DataRepository.deleteRFIDBy(IDx);
    }

}
