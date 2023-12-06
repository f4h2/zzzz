package com.IOT.IOT_system.controller;

import com.IOT.IOT_system.model.RFID;
import com.IOT.IOT_system.repository.RfidRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;


@RestController
@CrossOrigin
@RequestMapping("/RFID")
public class RFID_Controller {
    @Autowired
    private RfidRepository rfid_repo;

    @GetMapping(path = "/listRFID")
    public List<RFID> getAllRFID() {
        return rfid_repo.findAll();
    }

}