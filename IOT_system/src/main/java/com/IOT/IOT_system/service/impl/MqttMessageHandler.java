package com.IOT.IOT_system.service.impl;

import com.IOT.IOT_system.model.RFID;
import com.IOT.IOT_system.model.User;
import com.IOT.IOT_system.repository.RfidRepository;
import com.IOT.IOT_system.service.RFID_Service;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.integration.mqtt.support.MqttHeaders;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageHandler;
import org.springframework.messaging.MessagingException;
import org.springframework.stereotype.Service;

import java.util.Date;
import java.util.Optional;

@Service
@Slf4j
public class MqttMessageHandler implements MessageHandler {

    private final RFID_Service rfid_Service;
    @Autowired
    private RfidRepository rfidRepo;
    private final ObjectMapper mapper;

    public MqttMessageHandler(RFID_Service rfid_Service, ObjectMapper mapper) {
        this.rfid_Service = rfid_Service;
        this.mapper = mapper;
    }

    @Override
    public void handleMessage(Message<?> message) throws MessagingException {

        try {
            String topic = message.getHeaders().get(MqttHeaders.RECEIVED_TOPIC).toString();
            var rawData = message.getPayload().toString();
            String myMessage = mapper.readValue(rawData, String.class);
            Optional<RFID> RFID_O = rfidRepo.findByIDx(myMessage);
            if(RFID_O.isPresent()){
                rfidRepo.deleteRFIDBy(myMessage);
            }else{
                RFID rfid = new RFID();
                Date time = new Date();
                rfid.setDate(time);
                rfid.getIDx(myMessage);

                rfid_Service.save(rfid);
                log.info(rfid.toString());
            }
        } catch (Exception e) {
            log.error("something went wrong!");
        }
    }
}
