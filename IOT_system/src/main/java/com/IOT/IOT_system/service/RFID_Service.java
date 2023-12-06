package com.IOT.IOT_system.service;


import com.IOT.IOT_system.model.RFID;

import java.util.List;


public interface RFID_Service {

    RFID save(RFID rfid);
    List<RFID> listAll();

}
